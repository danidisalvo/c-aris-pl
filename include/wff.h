/*
 * © 2023-2025 Daniele Di Salvo
 */
#ifndef ARIS_PL_WFF_H
#define ARIS_PL_WFF_H

#include <stdbool.h>

typedef enum {
    WFF_ARGUMENT,
    WFF_ATOMIC_CONDITION,
    WFF_BINARY_CONDITION,
    WFF_CONDITIONAL,
    WFF_CONJUNCTION,
    WFF_DISJUNCTION,
    WFF_NEGATION,
} WffType;

typedef struct WellFormedFormula Wff;

// Not all fields of a wff must be used at the same time:
// The unary operator WFF_NEGATION wff has just the left wff
// The binary operators, like WFF_CONJUNCTION, have the left wff and the right wff
// WFF_ARGUMENT has the left wff, i.e., the conclusion, and a list of premises
//
// In the case of an atomic condition, wff1 represent the unitary condition
// In the case of a binary condition, wff1 and wff2 represent the two conditions
struct WellFormedFormula {
    WffType type;
    Wff *wff1;
    Wff *wff2;
    GSList *premises;
    GSList *falsehood_conditions;
    GSList *truth_conditions;
    char *value;
    bool b_value;
};

// allocate

Wff *create_argument();

Wff *create_atomic_condition(const char *value);

Wff *create_binary_condition(Wff *wff1, Wff *wff2);

Wff *create_conditional(Wff *wff1, Wff *wff2);

Wff *create_conjunction(Wff *wff1, Wff *wff2);

Wff *create_disjunction(Wff *wff1, Wff *wff2);

Wff *create_negation(Wff *wff1);

// free

void free_wff(gpointer data);

// other methods

void add_premise_to_argument(Wff *argument, Wff *premise);

GSList *determine_falsehood_conditions(Wff *wff);

GSList *determine_truthness_conditions(Wff *wff);

bool is_tautology(Wff *wff, char **error);

bool is_valid(Wff *arg, char **error);

Wff *parse_wff(GQueue *tokens, char **error);

bool valuate(Wff *wff, GHashTable *map, char **error);

char *wff_to_string(Wff *wff);

#endif //ARIS_PL_WFF_H
