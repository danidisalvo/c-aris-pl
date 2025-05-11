#include <stdio.h>
#include <string.h>
#include "lexical_analyzer.h"
#include "token.h"

#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

typedef struct {
    const char *line;
    Token **tokens;
    size_t n_tokens;
} TestCase;

typedef struct {
    const char *line;
    const char *error;
} FailedTestCase;

const char *token_type_to_str(TokenType type) {
    switch (type) {
        case TOKEN_AND:
            return "END";
        case TOKEN_ARGUMENT:
            return "ARGUMENT";
        case TOKEN_ASSERT:
            return "ASSERT";
        case TOKEN_ASSIGN:
            return "ASSIGN";
        case TOKEN_ATOM:
            return "ATOM";
        case TOKEN_COMMA:
            return "COMMA";
        case TOKEN_FALSE:
            return "FALSE";
        case TOKEN_IDENTIFIER:
            return "IDENTIFIER";
        case TOKEN_LEFT_BRACKET:
            return "LEFT_BRACKET";
        case TOKEN_MATERIAL_IMPLICATION:
            return "MATERIAL_IMPLICATION";
        case TOKEN_NOT:
            return "NOT";
        case TOKEN_OR:
            return "OR";
        case TOKEN_PRINT:
            return "PRINT";
        case TOKEN_RIGHT_BRACKET:
            return "RIGHT_BRACKET";
        case TOKEN_STRING:
            return "STRING";
        case TOKEN_THEREFORE:
            return "THEREFORE";
        case TOKEN_TRUE:
            return "TRUE";
        case TOKEN_VALIDATE:
            return "VALIDATE";
        case TOKEN_VALUATE:
            return "VALUATE";
        default:
            return "UNKNOWN_TOKEN";
    }
}

void test_tokenize() {
    const int num_tests = 12;

    TestCase test_cases[num_tests];

    int i = 0;
    int j = 0;

    test_cases[i].line = "print \"Hello, Aris!\"";
    test_cases[i].n_tokens = 2;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_print_token();
    test_cases[i].tokens[j] = create_string_token("Hello, Aris!");

    i++;
    j = 0;
    test_cases[i].line = "print\n";
    test_cases[i].n_tokens = 1;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j] = create_print_token();

    i++;
    test_cases[i].line = "// here we go!";
    test_cases[i].n_tokens = 0;
    test_cases[i].tokens = NULL;

    i++;
    j = 0;
    test_cases[i].line = "P := \t true\n";
    test_cases[i].n_tokens = 3;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_atom_token("P");
    test_cases[i].tokens[j++] = create_assign_token();
    test_cases[i].tokens[j] = create_true_token();

    i++;
    j = 0;
    test_cases[i].line = " Q := true\n";
    test_cases[i].n_tokens = 3;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_atom_token("Q");
    test_cases[i].tokens[j++] = create_assign_token();
    test_cases[i].tokens[j] = create_true_token();

    i++;
    j = 0;
    test_cases[i].line = "R := true\n";
    test_cases[i].n_tokens = 3;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_atom_token("R");
    test_cases[i].tokens[j++] = create_assign_token();
    test_cases[i].tokens[j] = create_true_token();

    i++;
    j = 0;
    test_cases[i].line = "argument arg1 := (P & Q) => R\n";
    test_cases[i].n_tokens = 10;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_argument_token();
    test_cases[i].tokens[j++] = create_identifier_token("arg1");
    test_cases[i].tokens[j++] = create_assign_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_atom_token("P");
    test_cases[i].tokens[j++] = create_and_token();
    test_cases[i].tokens[j++] = create_atom_token("Q");
    test_cases[i].tokens[j++] = create_right_bracket_token();
    test_cases[i].tokens[j++] = create_therefore_token();
    test_cases[i].tokens[j] = create_atom_token("R");

    i++;
    j = 0;
    test_cases[i].line = "valuate arg1\n";
    test_cases[i].n_tokens = 2;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_valuate_token();
    test_cases[i].tokens[j] = create_identifier_token("arg1");

    i++;
    j = 0;
    test_cases[i].line = "argument arg2 := P, !(P & !Q) => Q\n";
    test_cases[i].n_tokens = 14;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_argument_token();
    test_cases[i].tokens[j++] = create_identifier_token("arg2");
    test_cases[i].tokens[j++] = create_assign_token();
    test_cases[i].tokens[j++] = create_atom_token("P");
    test_cases[i].tokens[j++] = create_comma_token();
    test_cases[i].tokens[j++] = create_not_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_atom_token("P");
    test_cases[i].tokens[j++] = create_and_token();
    test_cases[i].tokens[j++] = create_not_token();
    test_cases[i].tokens[j++] = create_atom_token("Q");
    test_cases[i].tokens[j++] = create_right_bracket_token();
    test_cases[i].tokens[j++] = create_therefore_token();
    test_cases[i].tokens[j] = create_atom_token("Q");

    i++;
    j = 0;
    test_cases[i].line = "validate arg2\n";
    test_cases[i].n_tokens = 2;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_validate_token();
    test_cases[i].tokens[j] = create_identifier_token("arg2");

    i++;
    j = 0;
    test_cases[i].line = "argument arg3 := (!(!(P & Q) & !(P & R)) | !(P & (Q | R)))\n";
    test_cases[i].n_tokens = 32;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_argument_token();
    test_cases[i].tokens[j++] = create_identifier_token("arg3");
    test_cases[i].tokens[j++] = create_assign_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_not_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_not_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_atom_token("P");
    test_cases[i].tokens[j++] = create_and_token();
    test_cases[i].tokens[j++] = create_atom_token("Q");
    test_cases[i].tokens[j++] = create_right_bracket_token();
    test_cases[i].tokens[j++] = create_and_token();
    test_cases[i].tokens[j++] = create_not_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_atom_token("P");
    test_cases[i].tokens[j++] = create_and_token();
    test_cases[i].tokens[j++] = create_atom_token("R");
    test_cases[i].tokens[j++] = create_right_bracket_token();
    test_cases[i].tokens[j++] = create_right_bracket_token();
    test_cases[i].tokens[j++] = create_or_token();
    test_cases[i].tokens[j++] = create_not_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_atom_token("P");
    test_cases[i].tokens[j++] = create_and_token();
    test_cases[i].tokens[j++] = create_left_bracket_token();
    test_cases[i].tokens[j++] = create_atom_token("Q");
    test_cases[i].tokens[j++] = create_or_token();
    test_cases[i].tokens[j++] = create_atom_token("R");
    test_cases[i].tokens[j++] = create_right_bracket_token();
    test_cases[i].tokens[j++] = create_right_bracket_token();
    test_cases[i].tokens[j] = create_right_bracket_token();

    i++;
    j = 0;
    test_cases[i].line = "assert arg3\n";
    test_cases[i].n_tokens = 2;
    test_cases[i].tokens = malloc(test_cases[i].n_tokens * sizeof(Token *));
    test_cases[i].tokens[j++] = create_assert_token();
    test_cases[i].tokens[j] = create_identifier_token("arg3");

    for (i = 0; i < num_tests; i++) {
        char *error = NULL;
        GQueue *all_tokens = tokenize_string(test_cases[i].line, &error);

        if (error) {
            printf(RED "✘ Test case %d failed: %s\n" RESET, i + 1, error);
            free(error);
            continue;
        }
        if (all_tokens == NULL) {
            if (test_cases[i].n_tokens == 0) {
                printf(GREEN "✔ Test case %d passed\n" RESET, i + 1);
            } else {
                printf(RED "✘ Test case %d failed: expected %zu tokens, got NULL\n" RESET,
                       i + 1, test_cases[i].n_tokens);
            }
            continue;
        }

        GQueue *tokens = g_queue_pop_head(all_tokens);
        if (tokens == NULL) {
            if (test_cases[i].tokens != NULL) {
                printf(RED "✘ Test case %d failed: expected %zu tokens, got NULL\n" RESET,
                       i + 1, test_cases[i].n_tokens);
            }
            continue;
        }

        guint n = g_queue_get_length(tokens);
        if (n != test_cases[i].n_tokens) {
            printf(RED "✘ Test case %d failed: expected %zu tokens, got %d\n" RESET,
                   i + 1, test_cases[i].n_tokens, n);
            continue;
        }

        j = 0;
        bool failed = false;
        for (Token *token; (token = (Token *) g_queue_pop_head(tokens)) != NULL; j++) {
            if (test_cases[i].tokens[j]->type != token->type) {
                printf(RED "✘ Test case %d failed: wrong token's type - expected %s got %s\n" RESET,
                       i + 1, token_type_to_str(test_cases[i].tokens[j]->type),
                       token_type_to_str(token->type));
                failed = true;
                break;
            }
            if (strcmp(test_cases[i].tokens[j]->value, token->value) != 0) {
                printf(RED "✘ Test case %d failed: wrong token's value - expected %s, got %s\n" RESET,
                       i + 1, test_cases[i].tokens[j]->value, token->value);
                failed = true;
                break;
            }
        }
        if (failed) continue;

        printf(GREEN "✔ Test case %d passed\n" RESET, i + 1);
    }
}

void test_tokenize_fails() {
    FailedTestCase test_cases[] = {
            {
                    "P* := true",
                    "Unexpected character '*' at position 1 of line 'P* := true'"
            },
            {
                    "argument arg' !P therefore Q",
                    "Unexpected character ''' at position 12 of line 'argument arg' !P therefore Q'"
            },
            {
                    "(P >Q)",
                    "Unexpected character '>' at position 3 of line '(P >Q)'"
            },
            {
                    "P thereforeQ",
                    "Unexpected character 'Q' at position 11 of line 'P thereforeQ'"
            },
            {
                    "Ptherefore Q",
                    "Unexpected character 't' at position 1 of line 'Ptherefore Q'"
            },
            {
                    "P is \"All men are mortal",
                    "Unexpected character 'l' at position 23 of line 'P is \"All men are mortal'"
            },
            {
                    "P :- true",
                    "Unexpected character '-' at position 3 of line 'P :- true'"
            },
            {
                    "P :",
                    "Unexpected character '\0' at position 2 of line 'P :'"
            },
            {
                    "P Q",
                    "Unexpected symbol 'Q' at position 1 of line 'P Q'"
            },
            {
                    "(P & Q)",
                    "Unexpected symbol '(' at position 1 of line '(P & Q)'"
            },
            {
                    "!P therefore Q therefore R",
                    "Unexpected symbol 'therefore' at position 15 of line '!P therefore Q therefore R'"
            },
            {
                    "P",
                    "Unexpected end of line at line 'P'"
            }
    };

    size_t max = sizeof(test_cases) / sizeof(test_cases[0]);

    for (size_t i = 0; i < max; i++) {
        char *error = NULL;

        FailedTestCase *tc = &test_cases[i];
        GQueue *tokens = tokenize_string(tc->line, &error);

        if (tokens != NULL) {
            printf(RED "✘ Test case %zu failed: expected NULL\n" RESET, i + 1);
            continue;
        }

        if (strcmp(tc->error, error) != 0) {
            printf(RED "✘ Test case %zu failed: expected '%s', got '%s'\n" RESET,
                   i + 1, tc->error, error);
            continue;
        }

        printf(GREEN "✔ Test case %zu passed\n" RESET, i + 1);
    }
}
