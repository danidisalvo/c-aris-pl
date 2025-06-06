#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "token.h"

#define GREEN   "\x1b[32m"
#define RED     "\x1b[31m"
#define RESET   "\x1b[0m"

// a test case to be executed in test_is_tautology and test_is_valid methods
typedef struct {
    Wff *argument;
    bool result;
} BTestCase;

// a test case to be executed in test_determine_truth_conditions method
typedef struct {
    Wff *argument;
    GSList *truth_conditions;
} TruthTestCase;

// a test case to be executed in test_valuation method
typedef struct {
    Wff *wff;
    GHashTable *values;
    bool result;
} VTestCase;

bool compare_atomic_conditions(int i, Wff *e, Wff *a) {
    if (strcmp(a->value, e->value) != 0 || a->b_value != e->b_value) {
        printf(RED "✘ Test case %d failed: expected (%s, %d) got (%s, %d)\n" RESET,
               i + 1, a->value, a->b_value, e->value, e->b_value);
        return FALSE;
    }
    return TRUE;
}

void compare_conditions(int i, GSList *expected, GSList *actual) {
    while (expected != NULL && actual != NULL) {
        Wff *e = (Wff *) expected->data;
        Wff *a = (Wff *) actual->data;

        if (e->type == WFF_ATOMIC_CONDITION) {
            if (a->type == WFF_ATOMIC_CONDITION) {
                if (!compare_atomic_conditions(i, e, a)) return;
            } else {
                printf(RED "✘ Test case %d failed: expected ATOMIC_CONDITION got '%d'\n" RESET,
                       i + 1, a->type);
                return;
            }
        } else {
            if (a->type == WFF_BINARY_CONDITION) {
                if (!compare_atomic_conditions(i, e->wff1, a->wff1)) return;
                if (!compare_atomic_conditions(i, e->wff2, a->wff2)) return;
            } else {
                printf(RED "✘ Test case %d failed: expected BINARY_CONDITION got '%d'\n" RESET,
                       i + 1, a->type);
                return;
            }
        }

        expected = expected->next;
        actual = actual->next;
    }
    if (expected == NULL && actual == NULL) {
        printf(GREEN "✘ Test case %d passed\n" RESET, i + 1);
    } else {
        printf(RED "✘ Test case %d failed: the list don't contains the same number of elements\n" RESET,
               i + 1);
    }
}

void test_determine_truth_conditions() {
    Wff *p = create_atomic_condition("P");
    Wff *q = create_atomic_condition("Q");
    Wff *r = create_atomic_condition("R");

    Wff *not_p = create_negation(p);
    Wff *not_q = create_negation(q);
    Wff *not_r = create_negation(r);

    Wff *p_false = create_atomic_condition("P");
    p_false->b_value = FALSE;

    Wff *p_true = create_atomic_condition("P");
    p_true->b_value = TRUE;

    Wff *q_false = create_atomic_condition("Q");
    q_false->b_value = FALSE;

    Wff *q_true = create_atomic_condition("Q");
    q_true->b_value = TRUE;

    Wff *r_false = create_atomic_condition("R");
    r_false->b_value = FALSE;

    Wff *r_true = create_atomic_condition("R");
    r_true->b_value = TRUE;

    const int num_tests = 12;

    TruthTestCase test_cases[num_tests];

    int i = 0;

    test_cases[i].argument = p;
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions, p_true);

    i++;
    test_cases[i].argument = not_p;
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions, p_false);

    i++;
    test_cases[i].argument = create_negation(not_q);
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions, q_true);

    i++;
    test_cases[i].argument = create_negation(create_negation(not_r));
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions, r_false);

    i++;
    test_cases[i].argument = create_conjunction(p, q);
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_true));

    i++;
    test_cases[i].argument = create_conjunction(p, not_q);
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_false));

    i++;
    test_cases[i].argument = create_conjunction(not_p, not_q);
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_false));

    i++;
    test_cases[i].argument = create_negation(create_conjunction(p, q));
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_false));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_true));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_false));

    i++;
    test_cases[i].argument = create_negation(create_conjunction(not_p, not_q));
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_true));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_false));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_true));

    i++;
    test_cases[i].argument = create_disjunction(p, q);
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_false));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_true));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_true));

    i++;
    test_cases[i].argument = create_negation(create_disjunction(p, q));
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_false));

    i++;
    test_cases[i].argument = create_conditional(p, q);
    test_cases[i].truth_conditions = NULL;
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_false));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_false, q_true));
    test_cases[i].truth_conditions = g_slist_append(
            test_cases[i].truth_conditions,
            create_binary_condition(p_true, q_true));

    for (i = 0; i < num_tests; i++) {
        GSList *truth_conditions = determine_truthness_conditions(
                test_cases[i].argument);
        compare_conditions(i, test_cases[i].truth_conditions, truth_conditions);
    }
}

void test_embedded_operators() {
    GQueue *tokens = g_queue_new();
    g_queue_push_tail(tokens, create_left_bracket_token());
    g_queue_push_tail(tokens, create_left_bracket_token());
    g_queue_push_tail(tokens, create_left_bracket_token());
    g_queue_push_tail(tokens, create_atom_token("P"));
    g_queue_push_tail(tokens, create_and_token());
    g_queue_push_tail(tokens, create_atom_token("Q"));
    g_queue_push_tail(tokens, create_right_bracket_token());
    g_queue_push_tail(tokens, create_or_token());
    g_queue_push_tail(tokens, create_left_bracket_token());
    g_queue_push_tail(tokens, create_atom_token("Q"));
    g_queue_push_tail(tokens, create_and_token());
    g_queue_push_tail(tokens, create_atom_token("R"));
    g_queue_push_tail(tokens, create_right_bracket_token());
    g_queue_push_tail(tokens, create_right_bracket_token());
    g_queue_push_tail(tokens, create_and_token());
    g_queue_push_tail(tokens, create_left_bracket_token());
    g_queue_push_tail(tokens, create_left_bracket_token());
    g_queue_push_tail(tokens, create_atom_token("P"));
    g_queue_push_tail(tokens, create_or_token());
    g_queue_push_tail(tokens, create_atom_token("Q"));
    g_queue_push_tail(tokens, create_right_bracket_token());
    g_queue_push_tail(tokens, create_and_token());
    g_queue_push_tail(tokens, create_left_bracket_token());
    g_queue_push_tail(tokens, create_atom_token("Q"));
    g_queue_push_tail(tokens, create_or_token());
    g_queue_push_tail(tokens, create_atom_token("R"));
    g_queue_push_tail(tokens, create_right_bracket_token());
    g_queue_push_tail(tokens, create_right_bracket_token());
    g_queue_push_tail(tokens, create_right_bracket_token());
    g_queue_push_tail(tokens, create_right_bracket_token());

    char *error = NULL;
    Wff *wff = parse_wff(tokens, &error);
    if (error) {
        printf(RED "✘ Test case failed: %s\n" RESET, error);
        return;
    }

    const char *exp = "(((P & Q) | (Q & R)) & ((P | Q) & (Q | R)))";
    char *str = wff_to_string(wff);
    if (strcmp(exp, str) != 0) {
        printf(RED "✘ Test case failed: expected '%s', got '%s'\n" RESET, exp,
               str);
        return;
    }

    printf(GREEN "✔ Test case passed\n" RESET);
}

void test_is_tautology() {
    Wff *p = create_atomic_condition("P");
    Wff *not_p = create_negation(p);

    Wff *q = create_atomic_condition("Q");
    Wff *not_q = create_negation(q);

    Wff *r = create_atomic_condition("R");
    Wff *not_r = create_negation(r);

    const int num_tests = 7;

    BTestCase test_cases[num_tests];

    int i = 0;

    // P
    test_cases[i].argument = create_argument();
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises, p);
    test_cases[i].result = FALSE;

    // P & P
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conjunction(p, p));
    test_cases[i].result = FALSE;

    // P & !P
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conjunction(p, not_p));
    test_cases[i].result = FALSE;

    // P | P
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(p, p));
    test_cases[i].result = FALSE;

    // P | !P
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(p, not_p));
    test_cases[i].result = TRUE;

    // ((P ∧ (Q ∨ R)) ∨ (¬P ∧ (¬Q ∧ ¬R)))
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(
                    create_conjunction(p,
                                       create_disjunction(q, r)),
                    create_conjunction(not_p,
                                       create_conjunction(not_q, not_r))));
    test_cases[i].result = FALSE;

    // (¬(¬(P ∧ Q) ∧ ¬(P ∧ R)) ∨ ¬(P ∧ (Q ∨ R)))
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(
                    create_negation(create_conjunction(
                            create_negation(create_conjunction(p, q)),
                            create_negation(create_conjunction(p, r)))),
                    create_negation(
                            create_conjunction(p, create_disjunction(q, r)))));
    test_cases[i].result = TRUE;

    for (i = 0; i < num_tests; i++) {
        char *error = NULL;
        bool b = is_tautology(test_cases[i].argument, &error);
        if (error) {
            printf(RED "✘ Test case %d failed: %s\n" RESET, i + 1, error);
            free(error);
            continue;
        }
        if (test_cases[i].result != b) {
            printf(RED "✘ Test case %d failed: expected '%s', got '%s'\n" RESET,
                   i + 1,
                   test_cases[i].result ? "true" : "false",
                   b ? "true" : "false");
            continue;
        }

        printf(GREEN "✔ Test case %d passed\n" RESET, i + 1);
    }
}

void test_is_valid() {
    Wff *p = create_atomic_condition("P");
    Wff *not_p = create_negation(p);

    Wff *q = create_atomic_condition("Q");
    Wff *not_q = create_negation(q);

    Wff *r = create_atomic_condition("R");
    Wff *not_r = create_negation(r);

    Wff *p1 = create_atomic_condition("P1");
    Wff *s = create_atomic_condition("S");
    Wff *not_s = create_negation(s);

    const int num_tests = 11;

    BTestCase test_cases[num_tests];

    int i = 0;

    // !P, !Q, !R => S
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = s;
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises, not_p);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises, not_q);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises, not_r);
    test_cases[i].result = FALSE;

    // (P ∧ ¬Q), (R ∧ ¬S) ∴ (Q ∨ S)
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_disjunction(q, s);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conjunction(p, not_q));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conjunction(r, not_s));
    test_cases[i].result = FALSE;

    // (P ∧ ¬Q) ∴ ¬(Q ∧ R)
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_negation(create_conjunction(q, r));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conjunction(p, not_q));
    test_cases[i].result = TRUE;

    // (P ∨ Q) ∴ P
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = p;
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(p, q));
    test_cases[i].result = FALSE;

    // P, ¬(P ∧ ¬Q) ∴ Q
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = q;
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises, p);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_negation(
                    create_conjunction(p, not_q)));
    test_cases[i].result = TRUE;

    // (P → Q), (Q → R) ∴ (P → R)
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_conditional(p, r);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conditional(p, q));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conditional(q, r));
    test_cases[i].result = TRUE;

    // ((P ∧ Q) ∨ R) ∴ ¬(¬P ∨ ¬R)
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_disjunction(not_p, not_r);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(
                    create_conjunction(p, q), r));
    test_cases[i].result = FALSE;

    // ((P ∧ Q) ∧ R) ∴ (Q ∧ (R ∧ P))
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_conjunction(q,
                                                      create_conjunction(r, p));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conjunction(
                    create_conjunction(p, q), r));
    test_cases[i].result = TRUE;

    // ¬(P ∧ ¬Q), ¬(Q ∧ R) ∴ ¬(R ∧ ¬P)
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_negation(
            create_conjunction(r, not_p));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_negation(
                    create_conjunction(p, not_q)));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_negation(
                    create_conjunction(q, r)));
    test_cases[i].result = FALSE;

    // (¬P ∨ R), (P ∨ Q), ¬(Q ∧ ¬S) ∴ (R ∨ S)
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_disjunction(r, s);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(not_p, r));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_disjunction(p, q));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_negation(
                    create_conjunction(q, not_s)));
    test_cases[i].result = TRUE;

    // ((P → Q) → (R → ¬S)), (¬R → (Q ∧ P1)), (¬P → P1) ∴ (S → P1)
    i++;
    test_cases[i].argument = create_argument();
    test_cases[i].argument->wff1 = create_conditional(s, p1);
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conditional(create_conditional(p, q),
                               create_conditional(r, not_s)));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises, create_conditional(
                    not_r, create_conjunction(q, p1)));
    test_cases[i].argument->premises = g_slist_append(
            test_cases[i].argument->premises,
            create_conditional(not_p, p1));
    test_cases[i].result = FALSE;

    for (i = 0; i < num_tests; i++) {
        char *error = NULL;
        bool b = is_valid(test_cases[i].argument, &error);
        if (error) {
            printf(RED "✘ Test case %d failed: %s\n" RESET, i + 1, error);
            free(error);
            continue;
        }
        if (test_cases[i].result != b) {
            printf(RED "✘ Test case %d failed: expected '%s', got '%s'\n" RESET,
                   i + 1,
                   test_cases[i].result ? "true" : "false",
                   b ? "true" : "false");
            continue;
        }

        printf(GREEN "✔ Test case %d passed\n" RESET, i + 1);
    }
}

void test_missing_symbol() {
    Wff *wff = create_negation(create_atomic_condition("P"));
    GHashTable *values = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(values, "Q", GINT_TO_POINTER(TRUE));
    char *error = NULL;
    valuate(wff, values, &error);
    if (error) {
        if (strcmp("Missing symbol P", error) == 0) {
            printf(GREEN "✔ Test case passed\n" RESET);
        } else {
            printf(RED "✘ Test case failed: expected 'Missing symbol P', got '%s'\n" RESET,
                   error);
        }
    } else {
        printf(RED "✘ Test case failed: expected 'Missing symbol', got NULL\n" RESET);
    }
}

void test_valuation() {
    Wff *p = create_atomic_condition("P");
    Wff *not_p = create_negation(p);

    Wff *q = create_atomic_condition("Q");
    Wff *not_q = create_negation(q);

    Wff *dis_not_p_not_q = create_disjunction(not_p, not_q);

    const int num_tests = 4;

    VTestCase test_cases[num_tests];

    int i = 0;

    // !P with P true
    test_cases[i].wff = not_p;
    test_cases[i].values = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(test_cases[i].values, g_strdup("P"), GINT_TO_POINTER(TRUE));
    test_cases[i].result = FALSE;

    // !P with P false
    i++;
    test_cases[i].wff = not_p;
    test_cases[i].values = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(test_cases[i].values, g_strdup("P"), GINT_TO_POINTER(FALSE));
    test_cases[i].result = TRUE;

    // !P | !Q with P true and Q false
    i++;
    test_cases[i].wff = dis_not_p_not_q;
    test_cases[i].values = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(test_cases[i].values, g_strdup("P"), GINT_TO_POINTER(TRUE));
    g_hash_table_insert(test_cases[i].values, g_strdup("Q"), GINT_TO_POINTER(FALSE));
    test_cases[i].result = TRUE;

    // !P | !Q with P true and Q true
    i++;
    test_cases[i].wff = dis_not_p_not_q;
    test_cases[i].values = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(test_cases[i].values, g_strdup("P"), GINT_TO_POINTER(TRUE));
    g_hash_table_insert(test_cases[i].values, g_strdup("Q"), GINT_TO_POINTER(TRUE));
    test_cases[i].result = FALSE;

    for (i = 0; i < num_tests; i++) {
        char *error = NULL;
        bool b = valuate(test_cases[i].wff, test_cases[i].values, &error);
        if (error) {
            printf(RED "✘ Test case %d failed: %s\n" RESET, i + 1, error);
            free(error);
            continue;
        }
        if (test_cases[i].result != b) {
            printf(RED "✘ Test case %d failed: expected '%s', got '%s'\n" RESET,
                   i + 1,
                   test_cases[i].result ? "true" : "false",
                   b ? "true" : "false");
            continue;
        }

        printf(GREEN "✔ Test case %d passed\n" RESET, i + 1);
    }
}
