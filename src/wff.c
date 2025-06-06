/*
 * © 2023-2025 Daniele Di Salvo
 */
#include <glib.h>
#include <stdio.h>
#include "../include/wff.h"
#include "../include/token.h"

// allocate

static Wff *create_wff(WffType type) {
    Wff *f = malloc(sizeof(Wff));
    f->type = type;
    f->wff1 = NULL;
    f->wff2 = NULL;
    f->premises = NULL;
    f->falsehood_conditions = NULL;
    f->truth_conditions = NULL;
    f->value = NULL;
    f->b_value = FALSE;
    return f;
}

Wff *create_argument() {
    return create_wff(WFF_ARGUMENT);
}

Wff *create_atomic_condition(const char *value) {
    Wff *f = create_wff(WFF_ATOMIC_CONDITION);
    f->value = strdup(value);
    return f;
}

Wff *create_binary_condition(Wff *wff1, Wff *wff2) {
    Wff *f = create_wff(WFF_BINARY_CONDITION);
    f->wff1 = wff1;
    f->wff2 = wff2;
    return f;
}

Wff *create_conditional(Wff *wff1, Wff *wff2) {
    Wff *f = create_wff(WFF_CONDITIONAL);
    f->wff1 = wff1;
    f->wff2 = wff2;
    return f;
}

Wff *create_conjunction(Wff *wff1, Wff *wff2) {
    Wff *f = create_wff(WFF_CONJUNCTION);
    f->wff1 = wff1;
    f->wff2 = wff2;
    return f;
}

Wff *create_disjunction(Wff *wff1, Wff *wff2) {
    Wff *f = create_wff(WFF_DISJUNCTION);
    f->wff1 = wff1;
    f->wff2 = wff2;
    return f;
}

Wff *create_negation(Wff *wff1) {
    Wff *f = create_wff(WFF_NEGATION);
    f->wff1 = wff1;
    return f;
}

// free

void free_wff(gpointer data) {
    Wff *wff = (Wff *)data;

    g_free(wff->value);
    g_free(wff->wff1);
    g_free(wff->wff2);
    g_slist_free(wff->premises);
    g_slist_free(wff->falsehood_conditions);
    g_slist_free(wff->truth_conditions);

    g_free(wff);
}

// other

void add_premise_to_argument(Wff *arg, Wff *premise) {
    arg->premises = g_slist_append(arg->premises, premise);
}

GSList *determine_falsehood_conditions(Wff *wff) {
    if (wff->type == WFF_NEGATION) {
        return determine_truthness_conditions(wff->wff1);
    }

    GSList *list = NULL;
    if (wff->type == WFF_ATOMIC_CONDITION) {
        Wff *f = create_atomic_condition(wff->value);
        f->b_value = FALSE;
        list = g_slist_append(list, f);
    } else if (wff->type == WFF_CONDITIONAL) {
        GSList *wff1_t_conds = determine_truthness_conditions(wff->wff1);
        GSList *wff2_f_conds = determine_falsehood_conditions(wff->wff2);

        list = g_slist_append(list, create_binary_condition(wff1_t_conds->data, wff2_f_conds->data));
    } else if (wff->type == WFF_CONJUNCTION) {
        GSList *wff1_f_conds = determine_falsehood_conditions(wff->wff1);
        GSList *wff1_t_conds = determine_truthness_conditions(wff->wff1);
        GSList *wff2_f_conds = determine_falsehood_conditions(wff->wff2);
        GSList *wff2_t_conds = determine_truthness_conditions(wff->wff2);

        list = g_slist_append(list, create_binary_condition(wff1_f_conds->data, wff2_f_conds->data));
        list = g_slist_append(list, create_binary_condition(wff1_f_conds->data, wff2_t_conds->data));
        list = g_slist_append(list, create_binary_condition(wff1_t_conds->data, wff2_f_conds->data));
    } else if (wff->type == WFF_DISJUNCTION) {
        GSList *wff1_f_conds = determine_falsehood_conditions(wff->wff1);
        GSList *wff2_f_conds = determine_falsehood_conditions(wff->wff2);

        list = g_slist_append(list, create_binary_condition(wff1_f_conds->data, wff2_f_conds->data));
    }
    return list;
}

GSList *determine_truthness_conditions(Wff *wff) {
    if (wff->type == WFF_NEGATION) {
        return determine_falsehood_conditions(wff->wff1);
    }

    GSList *list = NULL;
    if (wff->type == WFF_ATOMIC_CONDITION) {
        Wff *f = create_atomic_condition(wff->value);
        f->b_value = TRUE;
        list = g_slist_append(list, f);
    } else if (wff->type == WFF_CONDITIONAL) {
        GSList *wff1_f_conds = determine_falsehood_conditions(wff->wff1);
        GSList *wff1_t_conds = determine_truthness_conditions(wff->wff1);

        GSList *wff2_f_conds = determine_falsehood_conditions(wff->wff2);
        GSList *wff2_t_conds = determine_truthness_conditions(wff->wff2);

        if (wff1_f_conds) {
            list = g_slist_append(list, create_binary_condition(wff1_f_conds->data, wff2_f_conds->data));
            list = g_slist_append(list, create_binary_condition(wff1_f_conds->data, wff2_t_conds->data));
            list = g_slist_append(list, create_binary_condition(wff1_t_conds->data, wff2_t_conds->data));
        }
    } else if (wff->type == WFF_CONJUNCTION) {
        GSList *wff1_t_conds = determine_truthness_conditions(wff->wff1);
        GSList *wff2_t_conds = determine_truthness_conditions(wff->wff2);

        list = g_slist_append(list, create_binary_condition(wff1_t_conds->data, wff2_t_conds->data));
    } else if (wff->type == WFF_DISJUNCTION) {
        GSList *wff1_f_conds = determine_falsehood_conditions(wff->wff1);
        GSList *wff1_t_conds = determine_truthness_conditions(wff->wff1);

        GSList *wff2_f_conds = determine_falsehood_conditions(wff->wff2);
        GSList *wff2_t_conds = determine_truthness_conditions(wff->wff2);

        if (wff1_f_conds) {
            list = g_slist_append(list, create_binary_condition(wff1_t_conds->data, wff2_f_conds->data));
            list = g_slist_append(list, create_binary_condition(wff1_f_conds->data, wff2_t_conds->data));
            list = g_slist_append(list, create_binary_condition(wff1_t_conds->data, wff2_t_conds->data));
        }
    }
    return list;
}

GSList* generate_truth_table(GHashTable *vars) {
    GSList *varsList = (GSList *) g_hash_table_get_keys(vars);
    GSList *result = NULL;
    size_t n = g_slist_length(varsList);
    int i = 0;
    int max = 1 << n;
    while (n > 0 && i != max) {

        char *bits = malloc(n + 1);
        bits[n] = '\0';
        int v = i;
        for (int j = n - 1; j >= 0; j--) {
            bits[j] = (v & 1) ? '1' : '0';
            v >>= 1;
        }

        GHashTable *map = g_hash_table_new(g_str_hash, g_str_equal);
        int j = 0;
        for (GSList *list = varsList; list != NULL; list = list->next, j++) {
            g_hash_table_insert(map, (char *) list->data, GINT_TO_POINTER(bits[j] == '1'));
        }
        result = g_slist_append(result, map);
        i++;
        free(bits);
    }

    return result;
}

GSList *get_atomic_conditions(Wff *wff) {
    GSList *list = NULL;
    if (wff->type == WFF_ATOMIC_CONDITION) {
        list = g_slist_append(list, wff);
    } else if (wff->type == WFF_BINARY_CONDITION) {
        list = g_slist_concat(list, get_atomic_conditions(wff->wff1));
        list = g_slist_concat(list, get_atomic_conditions(wff->wff2));
    }
    return list;
}

bool is_tautology(Wff *argument, char **error) {
    if (argument->type != WFF_ARGUMENT || argument->wff1 != NULL || g_slist_length(argument->premises) != 1) {
        *error = strdup("this method can be only invoked on arguments without conclusion and exactly one premise");
        return FALSE;
    }

    Wff *wff = (Wff *) argument->premises->data;
    GHashTable* vars = g_hash_table_new(g_str_hash, g_str_equal);
    for (GSList *clist = determine_truthness_conditions(wff); clist != NULL; clist = clist->next) {
        for (GSList *alist = get_atomic_conditions((Wff *)clist->data); alist != NULL; alist = alist->next) {
            g_hash_table_insert(vars, ((Wff *)alist->data)->value, NULL);
        }
    }

    for (GSList *list = generate_truth_table(vars); list != NULL; list = list->next) {
        if (!valuate(wff, ( GHashTable *) list->data, error)) {
            return false;
        }
    }

    return TRUE;
}

bool is_valid(Wff *arg, char **error) {
    GSList *formulae = NULL;
    formulae = g_slist_append(formulae, create_negation(arg->wff1));
    formulae = g_slist_concat(formulae, arg->premises);

    GHashTable* vars = g_hash_table_new(g_str_hash, g_str_equal);
    for (GSList *flist = formulae; flist != NULL; flist = flist->next) {
        for (GSList *clist = determine_truthness_conditions((Wff *) flist->data); clist != NULL; clist = clist->next) {
            for (GSList *alist = get_atomic_conditions((Wff *) clist->data); alist != NULL; alist = alist->next) {
                g_hash_table_insert(vars, ((Wff *) alist->data)->value, NULL);
            }
        }
    }

    // iterate over all possible input variables configurations
    for (GSList *vlist = generate_truth_table(vars); vlist != NULL; vlist = vlist->next) {
        bool valid = TRUE;
        // valuate all formulae for a given input variables configuration
        for (GSList *flist = formulae; flist != NULL; flist = flist->next) {
            // if one formula is false, then move on to the next input variables configuration
            if (!valuate(flist->data, (GHashTable *) vlist->data, error)) {
                valid = FALSE;
                break;
            }
        }
        // an input variables configuration which makes the premises and the negated conclusion all true was found
        if (valid) {
            return FALSE;
        }
    }

    // no configuration was found, hence the argument is valid
    return TRUE;
}

void *parse_binary(GQueue *tokens, char **error) {
    Wff *wff1 = (Wff *) parse_wff(tokens, error);
    if (wff1 == NULL) {
        *error = strdup("wff1 is null");
        return NULL;
    }

    Token *t = g_queue_pop_head(tokens);

    Wff *wff2 = (Wff *) parse_wff(tokens, error);
    if (wff2 == NULL) {
        *error = strdup("wff2 is null");
        return NULL;
    }

    g_queue_pop_head(tokens);
    if (t->type == TOKEN_AND) {
        return create_conjunction(wff1, wff2);
    }
    if (t->type == TOKEN_OR) {
        return create_disjunction(wff1, wff2);
    }
    return create_conditional(wff1, wff2);
}

Wff *parse_wff(GQueue *tokens, char **error) {
    Token *token = g_queue_peek_head(tokens);
    if (token != NULL) {
        if (token->type == TOKEN_COMMA || token->type == TOKEN_RIGHT_BRACKET || token->type == TOKEN_THEREFORE) {
            return NULL;
        }
        token = g_queue_pop_head(tokens);
        if (token->type == TOKEN_ATOM) {
            return create_atomic_condition(token->value);
        }
        if (token->type == TOKEN_NOT) {
            return create_negation(parse_wff(tokens, error));
        }
        if (token->type == TOKEN_LEFT_BRACKET) {
            return parse_binary(tokens, error);
        }
    }
    *error = strdup(token == NULL ? "null" : token->value);
    return NULL;
}

bool valuate(Wff *wff, GHashTable *values, char **error) {
    if (wff->type == WFF_ARGUMENT) {
        bool p = TRUE;
        for (GSList *premises = wff->premises; premises != NULL; premises = premises->next) {
           p = valuate((Wff *) premises->data, values, error);
           if (!p) break;
        }
        return p && valuate(wff->wff1, values, error);
    }

    if (wff->type == WFF_ATOMIC_CONDITION) {
        if (!g_hash_table_contains(values, wff->value)) {
            asprintf(error, "Missing symbol %s", wff->value);
            return FALSE;
        }
        return GPOINTER_TO_INT(g_hash_table_lookup(values, wff->value));
    }

    if (wff->type == WFF_CONDITIONAL) {
        return !valuate(wff->wff1, values, error) || valuate(wff->wff2, values, error);
    }

    if (wff->type == WFF_CONJUNCTION) {
        return valuate(wff->wff1, values, error) & valuate(wff->wff2, values, error);
    }

    if (wff->type == WFF_DISJUNCTION) {
        return valuate(wff->wff1, values, error) || valuate(wff->wff2, values, error);
    }

    if (wff->type == WFF_NEGATION) {
        return !valuate(wff->wff1, values, error);
    }

    // this should never happen
    return FALSE;
}

char **gslist_to_strings(GSList *list) {
    size_t n = g_slist_length(list);
    char **array = g_new(char*, n + 1);

    GSList *l = list;
    for (int i = 0; i < n; i++, l = l->next) {
        array[i] = wff_to_string((Wff *) l->data);
    }
    array[n] = NULL;
    return array;
}

char *wff_to_string(Wff *wff) {
    char *premises = NULL;
    char *str = NULL;
    switch (wff->type) {
        case WFF_ATOMIC_CONDITION:
            asprintf(&str, "%s", wff->value);
            return str;
        case WFF_CONDITIONAL:
            asprintf(&str, "(%s -> %s)", wff_to_string(wff->wff1), wff_to_string(wff->wff2));
            return str;
        case WFF_CONJUNCTION:
            asprintf(&str, "(%s & %s)", wff_to_string(wff->wff1), wff_to_string(wff->wff2));
            return str;
        case WFF_DISJUNCTION:
            asprintf(&str, "(%s | %s)", wff_to_string(wff->wff1), wff_to_string(wff->wff2));
            return str;
        case WFF_NEGATION:
            asprintf(&str, "!%s", wff_to_string(wff->wff1));
            return str;
        case WFF_ARGUMENT:
            premises = g_strjoinv(", ", (char **)gslist_to_strings(wff->premises));
            asprintf(&str, "%s => %s", premises, wff_to_string(wff->wff1));
            g_free(premises);
            return str;
        default:
            return NULL;
    }
}

