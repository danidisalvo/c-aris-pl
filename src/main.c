/*
 * Aris is a propositional logic interpreter.
 *
 * © 2023-2025 Daniele Di Salvo
 */
#include <stdio.h>
#include <stdlib.h>
#include "../include/lexical_analyzer.h"
#include "../include/parser.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: aris-pl <filename>\n");
        return EXIT_FAILURE;
    }

    FILE *file = fopen(argv[1], "r");
    if (!file) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    char *error = NULL;
    GQueue *tokens = tokenize(file, &error);

    if (error) {
        fprintf(stderr, "%s\n", error);
        free(error);
        return EXIT_FAILURE;
    }

    fclose(file);

    if (tokens != NULL) {
        parse(tokens);
    }

    return EXIT_SUCCESS;
}
