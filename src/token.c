/*
 * © 2023-2025 Daniele Di Salvo
 */
#include <string.h>
#include <stdbool.h>
#include "../include/token.h"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#define IS_VALID_TOKEN(arr, type) is_token((arr), ARRAY_SIZE(arr), (type))

static bool is_token(const TokenType *tokens, size_t count, TokenType type) {
    for (size_t i = 0; i < count; i++) {
        if (tokens[i] == type) {
            return true;
        }
    }
    return false;
}

static const TokenType and_tokens[] = {
        TOKEN_ATOM,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT
};

static const TokenType argument_tokens[] = {
        TOKEN_IDENTIFIER
};

static const TokenType assert_tokens[] = {
        TOKEN_IDENTIFIER
};

static const TokenType assign_tokens[] = {
        TOKEN_ATOM,
        TOKEN_FALSE,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT,
        TOKEN_TRUE
};

static const TokenType atom_tokens[] = {
        TOKEN_AND,
        TOKEN_ASSIGN,
        TOKEN_COMMA,
        TOKEN_MATERIAL_IMPLICATION,
        TOKEN_LEFT_BRACKET,
        TOKEN_OR,
        TOKEN_RIGHT_BRACKET,
        TOKEN_THEREFORE
};

static const TokenType comma_tokens[] = {
        TOKEN_ATOM,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT
};

static const TokenType identifier_tokens[] = {
        TOKEN_ASSIGN
};

static const TokenType left_bracket_tokens[] = {
        TOKEN_ATOM,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT
};

static const TokenType material_implication_tokens[] = {
        TOKEN_ATOM,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT
};

static const TokenType not_tokens[] = {
        TOKEN_ATOM,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT
};

static const TokenType or_tokens[] = {
        TOKEN_ATOM,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT
};

static const TokenType print_tokens[] = {
        TOKEN_STRING
};

static const TokenType right_bracket_tokens[] = {
        TOKEN_AND,
        TOKEN_COMMA,
        TOKEN_MATERIAL_IMPLICATION,
        TOKEN_OR,
        TOKEN_RIGHT_BRACKET,
        TOKEN_THEREFORE
};

static const TokenType therefore_tokens[] = {
        TOKEN_ATOM,
        TOKEN_LEFT_BRACKET,
        TOKEN_NOT
};

static const TokenType validate_tokens[] = {
        TOKEN_IDENTIFIER
};

static const TokenType valuate_tokens[] = {
        TOKEN_IDENTIFIER
};

Token *create_and_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_AND;
    t->value = strdup("∧");
    return t;
}

Token *create_argument_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_ARGUMENT;
    t->value = strdup("argument");
    return t;
}


Token *create_assert_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_ASSERT;
    t->value = strdup("assert");
    return t;
}

Token *create_assign_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_ASSIGN;
    t->value = strdup(":=");
    return t;
}

Token *create_atom_token(const char *value) {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_ATOM;
    t->value = strdup(value);
    return t;
}

Token *create_comma_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_COMMA;
    t->value = strdup(",");
    return t;
}

Token *create_false_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_FALSE;
    t->value = strdup("false");
    return t;
}

Token *create_identifier_token(const char *value) {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_IDENTIFIER;
    t->value = strdup(value);
    return t;
}

Token *create_left_bracket_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_LEFT_BRACKET;
    t->value = strdup("(");
    return t;
}

Token *create_material_implication_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_MATERIAL_IMPLICATION;
    t->value = strdup("→");
    return t;
}

Token *create_not_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_NOT;
    t->value = strdup("¬");
    return t;
}

Token *create_or_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_OR;
    t->value = strdup("∨");
    return t;
}

Token *create_print_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_PRINT;
    t->value = strdup("print");
    return t;
}

Token *create_right_bracket_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_RIGHT_BRACKET;
    t->value = strdup(")");
    return t;
}

Token *create_string_token(const char *value) {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_STRING;
    t->value = strdup(value);
    return t;
}

Token *create_therefore_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_THEREFORE;
    t->value = strdup("∴");
    return t;
}

Token *create_true_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_TRUE;
    t->value = strdup("true");
    return t;
}

Token *create_validate_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_VALIDATE;
    t->value = strdup("validate");
    return t;
}

Token *create_valuate_token() {
    Token *t = malloc(sizeof(Token));
    t->type = TOKEN_VALUATE;
    t->value = strdup("valuate");
    return t;
}

bool can_follow(const Token *current, const Token *next) {
    switch (current->type) {
        case TOKEN_AND:
            return IS_VALID_TOKEN(and_tokens, next->type);
        case TOKEN_ARGUMENT:
            return IS_VALID_TOKEN(argument_tokens, next->type);
        case TOKEN_ASSERT:
            return IS_VALID_TOKEN(assert_tokens, next->type);
        case TOKEN_ASSIGN:
            return IS_VALID_TOKEN(assign_tokens, next->type);
        case TOKEN_ATOM:
            return IS_VALID_TOKEN(atom_tokens, next->type);
        case TOKEN_COMMA:
            return IS_VALID_TOKEN(comma_tokens, next->type);
        case TOKEN_IDENTIFIER:
            return IS_VALID_TOKEN(identifier_tokens, next->type);
        case TOKEN_LEFT_BRACKET:
            return IS_VALID_TOKEN(left_bracket_tokens, next->type);
        case TOKEN_MATERIAL_IMPLICATION:
            return IS_VALID_TOKEN(material_implication_tokens, next->type);
        case TOKEN_NOT:
            return IS_VALID_TOKEN(not_tokens, next->type);
        case TOKEN_OR:
            return IS_VALID_TOKEN(or_tokens, next->type);
        case TOKEN_PRINT:
            return IS_VALID_TOKEN(print_tokens, next->type);
        case TOKEN_RIGHT_BRACKET:
            return IS_VALID_TOKEN(right_bracket_tokens, next->type);
        case TOKEN_THEREFORE:
            return IS_VALID_TOKEN(therefore_tokens, next->type);
        case TOKEN_VALIDATE:
            return IS_VALID_TOKEN(validate_tokens, next->type);
        case TOKEN_VALUATE:
            return IS_VALID_TOKEN(valuate_tokens, next->type);
        default:
            return true;
    }
}

void free_token(Token *token) {
    free(token->value);
    free(token);
}

void free_token_func(gpointer data) {
    free_token((Token *) data);
}
