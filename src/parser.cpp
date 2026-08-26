#include "comp.h"
#include "parser.h"
#include "types.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define PARSER_BUFFER 128
#define TOKEN_BUFFER 128

// TODO: parser tests
// TODO: FSM graph (hand drawn)

enum token_type {
	NONE,
	NUMBER,
	NUMBER_DOT,
	NUMBER_DOT_NUMBER,
	VARIABLE,
	PLUS,
	MINUS,
	POWER,
	EQUAL,
};

struct token {
	enum token_type type;
	char data[PARSER_BUFFER];
};

int
parse_equation(const char *equation, size_t len, struct coeffs *dst)
{
#define PUSHC(c)                                        \
	do {                                            \
		if (buf_ptr >= PARSER_BUFFER - 1) {     \
			return -1;                      \
		}                                       \
		buf[buf_ptr++] = c;                     \
	} while(0)

#define PUSHT(t)                                \
	do {                                    \
		if (tok_ptr >= TOKEN_BUFFER) {  \
			return -1;              \
		}                               \
		strncpy(t.data, buf, buf_ptr);  \
		tok_buf[tok_ptr++] = t;         \
		t = {};                         \
		buf_ptr = 0;                    \
	} while(0)

	assert(NULL != equation);
	assert(NULL != dst);

	char buf[PARSER_BUFFER] = {};
	token tok_buf[TOKEN_BUFFER] = {};
	size_t buf_ptr = 0;
	size_t tok_ptr = 0;

	enum token_type s = NONE;
	struct token tok = {};
	for (size_t i = 0; i < len; i++) {
		char c = equation[i];
		switch (s) {
		case NONE:
			if (isspace(c)) {
				break;
			}
			PUSHC(c);
			if (isdigit(c)) {
				tok.type = NUMBER;
				s = NUMBER;
				break;
			}
			// TODO: use switch
			if ('x' == c) {
				tok.type = VARIABLE;
				s = VARIABLE;
				break;
			}
			if ('=' == c) {
				tok.type = EQUAL;
				s = EQUAL;
				break;
			}
			if ('+' == c) {
				tok.type = PLUS;
				s = PLUS;
				break;
			}
			if ('-' == c) {
				tok.type = MINUS;
				s = MINUS;
				break;
			}
			if ('^' == c) {
				tok.type = POWER;
				s = POWER;
				break;
			}
			if ('.' == c) {
				return -1;
			}
			break;
		case NUMBER:
			if ('.' == c) {
				PUSHC(c);
				s = NUMBER_DOT;
				break;
			}
			if (isdigit(c)) {
				PUSHC(c);
				break;
			}
			if (isspace(c)) {
				PUSHT(tok);
				tok.type = NONE;
				s = NONE;
				break;
			}
			if ('=' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = EQUAL;
				s = EQUAL;
				break;
			}
			if ('-' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = MINUS;
				s = MINUS;
				break;
			}
			if ('+' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = PLUS;
				s = PLUS;
				break;
			}
			if ('x' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = VARIABLE;
				s = VARIABLE;
				break;
			}
			if ('^' == c) {
				return -1;
			}
			break;
		case NUMBER_DOT:
			if ('.' == c) {
				return -1;
			}
			if (isdigit(c)) {
				PUSHC(c);
				tok.type = NUMBER_DOT_NUMBER;
				s = NUMBER_DOT_NUMBER;
				break;
			}
			break;
		case NUMBER_DOT_NUMBER:
			if (isspace(c)) {
				PUSHT(tok);
				tok.type = NONE;
				s = NONE;
				break;
			}
			if (isdigit(c)) {
				PUSHC(c);
				break;
			}
			if ('.' == c) {
				return -1;
			}
			if ('x' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = VARIABLE;
				s = VARIABLE;
				break;
			}
			if ('=' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = EQUAL;
				s = EQUAL;
				break;
			}
			if ('-' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = MINUS;
				s = MINUS;
				break;
			}
			if ('+' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = PLUS;
				s = PLUS;
				break;
			}
			if ('^' == c) {
				return -1;
			}
			break;
		case VARIABLE:
			if (isdigit(c)) {
				return -1;
			}
			if (isspace(c)) {
				PUSHT(tok);
				tok.type = NONE;
				s = NONE;
				break;
			}
			if ('+' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = PLUS;
				s = PLUS;
				break;
			}
			if ('-' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = MINUS;
				s = MINUS;
				break;
			}
			if ('^' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = POWER;
				s = POWER;
				break;
			}
			if ('=' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = EQUAL;
				s = EQUAL;
				break;
			}
			if ('.' == c) {
				return -1;
			}
			break;
		case PLUS:
			if (isspace(c)) {
				PUSHT(tok);
				tok.type = NONE;
				s = NONE;
				break;
			}
			if ('x' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = VARIABLE;
				s = VARIABLE;
				break;
			}
			if (isdigit(c)) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = NUMBER;
				s = NUMBER;
				break;
			}
			return -1;
		case MINUS:
			if (isspace(c)) {
				PUSHT(tok);
				tok.type = NONE;
				s = NONE;
				break;
			}
			if ('x' == c) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = VARIABLE;
				s = VARIABLE;
				break;
			}
			if (isdigit(c)) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = NUMBER;
				s = NUMBER;
				break;
			}
			return -1;
		case POWER:
			if (isspace(c)) {
				PUSHT(tok);
				tok.type = NONE;
				s = NONE;
				break;
			}
			if (isdigit(c)) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = NUMBER;
				s = NUMBER;
				break;
			}
			return -1;
		case EQUAL:
			if (isspace(c)) {
				PUSHT(tok);
				tok.type = NONE;
				s = NONE;
				break;
			}
			if (isdigit(c)) {
				PUSHT(tok);
				PUSHC(c);
				tok.type = NUMBER;
				s = NUMBER;
				break;
			}
			return -1;
		default:
			assert("Unreachable" && 0);
		}
	}
	if (NONE != s) {
		PUSHT(tok);
	}
	// validate token placement
	for (size_t i = 0; i < tok_ptr - 1; i++) {
		if ((EQUAL == tok_buf[i].type ||
		     PLUS == tok_buf[i].type || MINUS == tok_buf[i].type ||
		     POWER == tok_buf[i].type || NUMBER == tok_buf[i].type ||
		     NUMBER_DOT_NUMBER == tok_buf[i].type) &&
		    tok_buf[i].type == tok_buf[i + 1].type) {
			return -1;
		}
	}
	// glue signs to numbers
	struct token final_tokens[TOKEN_BUFFER] = {};
	size_t token_len = 0;
	{
		size_t i = 0;
		while (i < tok_ptr) {
			switch (tok_buf[i].type) {
			case MINUS:
			case PLUS: {
				if (NUMBER != tok_buf[i + 1].type &&
				    NUMBER_DOT_NUMBER != tok_buf[i + 1].type) {
					return -1;
				}
				struct token t = {};
				if (strlen(tok_buf[i].data) +
				    strlen(tok_buf[i + 1].data) >= 128) {
					return -1;
				}
				t.type = NUMBER;
				if (MINUS == tok_buf[i].type) {
					strcat(t.data, tok_buf[i].data);
				}
				strcat(t.data, tok_buf[i + 1].data);
				final_tokens[token_len++] = t;
				i += 2;
				break;
			}
			case NUMBER:
			case NUMBER_DOT_NUMBER:
				final_tokens[token_len] = tok_buf[i];
				final_tokens[token_len].type = NUMBER;
				token_len++;
				i += 1;
				break;
			case VARIABLE:
			case EQUAL:
			case POWER:
				final_tokens[token_len++] = tok_buf[i];
				i += 1;
				break;
			case NUMBER_DOT:
			case NONE:
			default:
				assert("Unreachable" && 0);
			}
		}
	}
	enum token_type correct[] = {
		NUMBER, VARIABLE, POWER, NUMBER, NUMBER, VARIABLE, NUMBER,
		EQUAL, NUMBER
	}; // ax^2 + bx + c = 0
	if (sizeof(correct) / sizeof(enum token_type) != token_len) {
		return -1;
	}
	// compare with the reference
	for (size_t i = 0; i < sizeof(correct) / sizeof(enum token_type); i++) {
		if (correct[i] != final_tokens[i].type) {
			return -1;
		}
	}
	// ax^p + bx + c = f
	double a = 0, p = 0, b = 0, c = 0, f = 0;
	if (1 != sscanf(final_tokens[0].data, "%lf", &a)) {
		return -1;
	}
	if (1 != sscanf(final_tokens[3].data, "%lf", &p)) {
		return -1;
	}
	if (1 != sscanf(final_tokens[4].data, "%lf", &b)) {
		return -1;
	}
	if (1 != sscanf(final_tokens[6].data, "%lf", &c)) {
		return -1;
	}
	if (1 != sscanf(final_tokens[8].data, "%lf", &f)) {
		return -1;
	}
	if (!isequal(p, 2) || !isequal(f, 0)) {
		return -1;
	}
	*dst = (struct coeffs) {
		.a = a,
		.b = b,
		.c = c,
	};
	return 0;
}
