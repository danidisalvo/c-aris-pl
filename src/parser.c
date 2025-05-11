/*
 * © 2023-2025 Daniele Di Salvo
 */
#include <glib.h>
#include <stdbool.h>
#include <printf.h>
#include "../include/parser.h"
#include "../include/token.h"

int parse(GQueue *all_tokens) {
    GHashTable *arguments = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_wff);
    GHashTable *assertions = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *validations = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *valuations = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *values = g_hash_table_new(g_str_hash, g_str_equal);

    char *error = NULL;
    for (GQueue *tokens; (tokens = (GQueue *) g_queue_pop_head(all_tokens)) != NULL;) {
        Token *token = (Token *) g_queue_pop_head(tokens);

        if (token->type == TOKEN_PRINT) {
            if (g_queue_peek_head(tokens) != NULL) {
                printf("%s\n", ((Token *) g_queue_pop_head(tokens))->value);
            } else {
                printf("\n");
            }
            continue;
        }

        Token *next_token = (Token *) g_queue_pop_head(tokens);
        if (token->type == TOKEN_ATOM) {
            if (next_token->type == TOKEN_ASSIGN) {
                Token *t = (Token *) g_queue_pop_head(tokens);
                g_hash_table_insert(values, g_strdup(token->value), GINT_TO_POINTER(t->type == TOKEN_TRUE));
            }
        } else if (token->type == TOKEN_ARGUMENT) {
            g_queue_pop_head(tokens);
            Wff *argument = create_argument();
            while (!g_queue_is_empty(tokens)) {
                Wff *formula = parse_wff(tokens, &error);
                if (error) {
                    fprintf(stderr, "Unexpected symbol '%s'\n", error);
                    return EXIT_FAILURE;
                }
                if (formula == NULL) {
                    Token *t = (Token *) g_queue_pop_head(tokens);
                    if (t->type != TOKEN_RIGHT_BRACKET && argument->wff1 != NULL) {
                        fprintf(stderr, "Unexpected symbol\n");
                        return EXIT_FAILURE;
                    }
                    if (t->type == TOKEN_THEREFORE) {
                        argument->wff1 = parse_wff(tokens, &error);
                        if (error) {
                            fprintf(stderr, "Unexpected symbol '%s'\n", error);
                            return EXIT_FAILURE;
                        }
                    }
                } else {
                    add_premise_to_argument(argument, formula);
                }
            }
            g_hash_table_insert(arguments, g_strdup(next_token->value), argument);
        } else if (token->type == TOKEN_ASSERT) {
            Wff *arg = (Wff *) g_hash_table_lookup(arguments, next_token->value);
            bool b = is_tautology(arg, &error);
            if (error) {
                fprintf(stderr, "Unexpected symbol '%s'\n", error);
                return EXIT_FAILURE;
            }
            g_hash_table_insert(assertions, next_token->value, GINT_TO_POINTER(b));
            printf("argument \"%s\" is %sa tautology\n", wff_to_string((Wff *) ((GSList *) arg->premises)->data), b ? "" : "not ");
        } else if (token->type == TOKEN_VALIDATE) {
            char *id = next_token->value;
            Wff *arg = (Wff *) g_hash_table_lookup(arguments, id);
            bool b = is_valid(arg, &error);
            g_hash_table_insert(validations, id, GINT_TO_POINTER(b));
            printf("argument \"%s\" is %s\n", wff_to_string(arg), b ? "valid" : "invalid");
        } else if (token->type == TOKEN_VALUATE) {
            char *id = next_token->value;
            Wff *arg = (Wff *) g_hash_table_lookup(arguments, id);
            bool b = valuate(arg, values, &error);
            if (error) {
                fprintf(stderr, "%s\n", error);
                return EXIT_FAILURE;
            }
            g_hash_table_insert(valuations, id, GINT_TO_POINTER(b));
            printf("argument \"%s\" is %s\n", wff_to_string(arg), b ? "true" : "false");
        }
    }

    return EXIT_SUCCESS;
}
