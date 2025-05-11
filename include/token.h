/*
 * © 2023-2025 Daniele Di Salvo
 */
#include <glib.h>
#include <stdbool.h>

#ifndef ARIS_PL_TOKEN_H
#define ARIS_PL_TOKEN_H

typedef enum {
    TOKEN_AND,
    TOKEN_ARGUMENT,
    TOKEN_ASSERT,
    TOKEN_ASSIGN,
    TOKEN_ATOM,
    TOKEN_COMMA,
    TOKEN_FALSE,
    TOKEN_IDENTIFIER,
    TOKEN_LEFT_BRACKET,
    TOKEN_MATERIAL_IMPLICATION,
    TOKEN_NOT,
    TOKEN_OR,
    TOKEN_PRINT,
    TOKEN_RIGHT_BRACKET,
    TOKEN_STRING,
    TOKEN_THEREFORE,
    TOKEN_TRUE,
    TOKEN_VALIDATE,
    TOKEN_VALUATE
} TokenType;

typedef struct {
    TokenType type;
    char *value;
} Token;

Token *create_and_token();

Token *create_argument_token();

Token *create_assert_token();

Token *create_assign_token();

Token *create_atom_token(const char *value);

Token *create_comma_token();

Token *create_false_token();

Token *create_identifier_token(const char *value);

Token *create_left_bracket_token();

Token *create_material_implication_token();

Token *create_not_token();

Token *create_or_token();

Token *create_print_token();

Token *create_right_bracket_token();

Token *create_string_token(const char *value);

Token *create_therefore_token();

Token *create_true_token();

Token *create_validate_token();

Token *create_valuate_token();

bool can_follow(const Token *current, const Token *next);

void free_token(Token *token);

void free_token_func(gpointer data);

const char *token_type_to_str(TokenType type);

#endif
