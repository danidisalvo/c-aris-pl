/*
 * © 2023-2025 Daniele Di Salvo
 */
#ifndef ARIS_PL_LEXICAL_ANALYZER_H
#define ARIS_PL_LEXICAL_ANALYZER_H

#include <glib.h>

#define MAX_LINE 1024

GQueue *tokenize(FILE *file, char** error);

GQueue *tokenize_string(const char* str, char** error);

#endif
