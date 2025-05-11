/*
 * © 2023-2025 Daniele Di Salvo
 */
#include <glib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../include/lexical_analyzer.h"
#include "../include/token.h"

static const char k_argument[] = "argument";
static const char k_assert[] = "assert";
static const char k_false[] = "false";
static const char k_print[] = "print";
static const char k_therefore[] = "therefore";
static const char k_true[] = "true";
static const char k_validate[] = "validate";
static const char k_valuate[] = "valuate";

GQueue* g_queue_copy(GQueue* src) {
    GQueue* dest = g_queue_new();
    for (GList* l = src->head; l != NULL; l = l->next) {
        g_queue_push_tail(dest, l->data);
    }
    return dest;
}

GQueue *tokenize_string(const char *str, char **error) {
    FILE *f = fmemopen((void *) str, strlen(str), "r");
    if (!f) {
        perror("fmemopen failed");
        exit(EXIT_FAILURE);
    }
    GQueue *tokens = tokenize(f, error);
    fclose(f);
    return tokens;
}

GQueue *tokenize(FILE *file, char **error) {
    GQueue *tokens = g_queue_new();
    GString *gs = g_string_new("");

    char line[MAX_LINE];

    while (fgets(line, sizeof(line), file) != NULL) {
        GQueue *queue = g_queue_new();
        if (queue == NULL) {
            *error = strdup("Failed to create the queue");
            return NULL;
        }

        bool in_atom = false;
        bool in_identifier = false;
        bool in_string = false;
        int n_argument = 0;
        int n_print = 0;
        int n_therefore = 0;
        int n_valuate = 0;
        g_string_truncate(gs, 0);

        const char *ptr = strstr(line, "//");
        if (ptr) {
            line[ptr - line] = '\0';
        }

        for (int i = 0; line[i] != '\0' && line[i] != '\n'; i++) {
            char c = line[i];

            // first we deal with some special cases
            if (in_atom) {
                if (c == ' ' || c == '\t' || c == '&' || c == ')' || c == ',' ||
                    c == ':' || c == '-' || c == '|') {
                    g_queue_push_tail(queue, create_atom_token(gs->str));
                    in_atom = false;
                } else if ((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') ||
                           c == '_' || c == '\'') {
                    g_string_append_c(gs, c);
                    continue;
                } else {
                    asprintf(error,
                             "Unexpected character '%c' at position %d of line '%s'",
                             c, i, line);
                    return NULL;
                }
            }

            if (in_identifier) {
                if (c == ' ' || c == '\t') {
                    g_queue_push_tail(queue, create_identifier_token(gs->str));
                    in_identifier = false;
                } else if (c >= 'a' && c <= 'z' || c >= '0' && c <= '9' ||
                           c == '_') {
                    g_string_append_c(gs, c);
                    continue;
                } else {
                    asprintf(error,
                             "Unexpected character '%c' at position %d of line '%s'",
                             c, i, line);
                    return NULL;
                }
            }

            if (in_string) {
                if (c == '"') {
                    g_queue_push_tail(queue, create_string_token(gs->str));
                    in_string = false;
                } else {
                    if (i == strlen(line) - 1) {
                        asprintf(error,
                                 "Unexpected character '%c' at position %d of line '%s'",
                                 c, i, line);
                        return NULL;
                    }
                    g_string_append_c(gs, c);
                }
                continue;
            }

            // spaces are ignored except when parsing a string
            if (c == ' ' || c == '\t') {
                continue;
            }

            if (c == '"') {
                in_string = true;
                g_string_truncate(gs, 0);
            } else if (c == '(') {
                g_queue_push_tail(queue, create_left_bracket_token());
            } else if (c == ')') {
                g_queue_push_tail(queue, create_right_bracket_token());
            } else if (c == '&') {
                g_queue_push_tail(queue, create_and_token());
            } else if (c == ',') {
                g_queue_push_tail(queue, create_comma_token());
            } else if (c == '!' || c == '~') {
                g_queue_push_tail(queue, create_not_token());
            } else if (c == '|') {
                g_queue_push_tail(queue, create_or_token());
            } else if (c == ':') {
                if (line[++i] == '=') {
                    g_queue_push_tail(queue, create_assign_token());
                } else {
                    asprintf(error,
                             "Unexpected character '%c' at position %d of line '%s'",
                             line[i], i, line);
                    return NULL;
                }
            } else if (c == '-') {
                if (line[++i] == '>') {
                    g_queue_push_tail(queue,
                                      create_material_implication_token());
                } else {
                    asprintf(error,
                             "Unexpected character '%c' at position %d of line '%s'",
                             line[i], i, line);
                    return NULL;
                }
            } else if (c == '=') {
                if (line[++i] == '>') {
                    g_queue_push_tail(queue, create_therefore_token());
                } else {
                    asprintf(error,
                             "Unexpected character '%c' at position %d of line '%s'",
                             line[i], i, line);
                    return NULL;
                }
            } else if (c >= 'A' && c <= 'Z') {
                in_atom = true;
                g_string_truncate(gs, 0);
                g_string_append_c(gs, c);
            } else if (c >= 'a' && c <= 'z') {
                // search for a keyword
                bool found = false;
                if (strncmp(line + i, k_argument, strlen(k_argument)) == 0) {
                    if (n_argument == 1) {
                        asprintf(error,
                                 "Unexpected symbol 'argument' at position %d of line '%s'",
                                 i, line);
                        return NULL;
                    } else {
                        n_argument++;
                    }
                    g_queue_push_tail(queue, create_argument_token());
                    i += strlen(k_argument);
                    found = true;
                } else if (strncmp(line + i, k_assert, strlen(k_assert)) == 0) {
                    g_queue_push_tail(queue, create_assert_token());
                    i += strlen(k_assert);
                    found = true;
                } else if (strncmp(line + i, k_false, strlen(k_false)) == 0) {
                    g_queue_push_tail(queue, create_false_token());
                    i += strlen(k_false);
                    found = true;
                } else if (strncmp(line + i, k_print, strlen(k_print)) == 0) {
                    if (n_print == 1) {
                        asprintf(error,
                                 "Unexpected symbol 'print' at position %d of line '%s'",
                                 i, line);
                        return NULL;
                    } else {
                        n_print++;
                    }
                    g_queue_push_tail(queue, create_print_token());
                    i += strlen(k_print);
                    found = true;
                } else if (
                        strncmp(line + i, k_therefore, strlen(k_therefore)) ==
                        0) {
                    if (n_therefore == 1) {
                        asprintf(error,
                                 "Unexpected symbol 'therefore' at position %d of line '%s'",
                                 i, line);
                        return NULL;
                    } else {
                        n_therefore++;
                    }
                    g_queue_push_tail(queue, create_therefore_token());
                    i += strlen(k_therefore);
                    found = true;
                } else if (strncmp(line + i, k_true, strlen(k_true)) == 0) {
                    g_queue_push_tail(queue, create_true_token());
                    i += strlen(k_true);
                    found = true;
                } else if (strncmp(line + i, k_validate, strlen(k_validate)) ==
                           0) {
                    g_queue_push_tail(queue, create_validate_token());
                    i += strlen(k_validate);
                    found = true;
                } else if (strncmp(line + i, k_valuate, strlen(k_valuate)) ==
                           0) {
                    if (n_valuate == 1) {
                        asprintf(error,
                                 "Unexpected symbol 'valuate' at position %d of line '%s'",
                                 i, line);
                        return NULL;
                    } else {
                        n_valuate++;
                    }
                    g_queue_push_tail(queue, create_valuate_token());
                    i += strlen(k_valuate);
                    found = true;
                }
                if (found) {
                    if (line[i] != ' ' && line[i] != '\t' && line[i] != '\n') {
                        asprintf(error,
                                 "Unexpected character '%c' at position %d of line '%s'",
                                 line[i], i, line);
                        return NULL;
                    }
                } else {
                    in_identifier = true;
                    g_string_truncate(gs, 0);
                    g_string_append_c(gs, c);
                }
            } else {
                asprintf(error,
                         "Unexpected character '%c' at position %d of line '%s'",
                         c, i, line);
                return NULL;
            }
        }

        // an atom or an identifier can be the last item of a line
        if (in_atom) {
            g_queue_push_tail(queue, create_atom_token(gs->str));
        } else if (in_identifier) {
            g_queue_push_tail(queue, create_identifier_token(gs->str));
        }

        if (queue->length == 0) continue;

        // validates the line's syntax
        GQueue *list = g_queue_new();
        Token *token;
        for (int i = 1; (token = (Token *) g_queue_pop_head(queue)) != NULL; i++) {
            if (g_queue_is_empty(list)) {
                Token *next_token = (Token *) g_queue_peek_head(queue);
                if (token->type != TOKEN_PRINT && next_token == NULL) {
                    asprintf(error, "Unexpected end of line at line '%s'",
                             line);
                    return NULL;
                }

                if (token->type == TOKEN_ATOM) {
                    if (next_token->type != TOKEN_ASSIGN) {
                        asprintf(error,
                                 "Unexpected symbol '%s' at position %d of line '%s'",
                                 next_token->value, i, line);
                        return NULL;
                    }
                } else if (!(token->type == TOKEN_ARGUMENT ||
                             token->type == TOKEN_ASSERT ||
                             token->type == TOKEN_PRINT ||
                             token->type == TOKEN_VALUATE ||
                             token->type == TOKEN_VALIDATE)
                        ) {
                    asprintf(error,
                             "Unexpected symbol '%s' at position %d of line '%s'",
                             token->value, i, line);
                    return NULL;
                }
            }
            Token *next_token = (Token *) g_queue_peek_head(queue);
            if (next_token == NULL || can_follow(token, next_token)) {
                g_queue_push_tail(list, token);
            } else {
                asprintf(error,
                         "Unexpected symbol '%s' at position %d of line '%s'",
                         next_token->value, i, line);
                return NULL;
            }
        }

        g_queue_push_tail(tokens, g_queue_copy(list));

        g_queue_clear(list);
        g_queue_clear(queue);
    }

    fclose(file);

    g_string_free(gs, TRUE);

    *error = NULL;
    return tokens;
}
