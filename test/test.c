#include <printf.h>
#include "lexical_analyzer/test_lexical_analizer.h"
#include "parser/test_parser.h"

int main() {
    printf("Running test_tokenize\n");
    test_tokenize();

    printf("\nRunning test_tokenize_fails\n");
    test_tokenize_fails();

    printf("\nRunning test_determine_truth_conditions\n");
    test_determine_truth_conditions();

    printf("\nRunning test_embedded_operators\n");
    test_embedded_operators();

    printf("\nRunning test_missing_symbol\n");
    test_missing_symbol();

    printf("\nRunning test_is_tautology\n");
    test_is_tautology();

    printf("\nRunning test_is_valid\n");
    test_is_valid();

    printf("\nRunning test_valuation\n");
    test_valuation();
}
