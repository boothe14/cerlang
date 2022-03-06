#ifndef LEXER_H
#define LEXER_H
#include <stdbool.h>

typedef enum {
	TOKEN_INT,
	TOKEN_FLOAT,
	TOKEN_OPERATOR,
	TOKEN_IDENTIFIER,
	TOKEN_STRING,
	TOKEN_KEYWORD
} tokentype_T;

typedef struct {
	int type;
	char *value;
} token_T;

typedef struct {
	int pos;
	char ch;
	char *text;
} lexer_T;

static lexer_T lexer = {0};

int lexer_advance();
void lexer_load_program_from_file(const char *filepath);
void lexer_init();
void lexer_run(token_T **desttokens, size_t *destsize);
token_T lexer_make_token();
token_T lexer_make_number();
const char *lexer_make_word();
const char *lexer_make_string();
bool isspecial(char ch);
const char *toktype_as_str(int type);
const char *tok_as_str(const token_T tok);
void debug_tokens(token_T *tokens, size_t destsize);
bool word_in_keywords(const char *str);

#define LEXER_TOKEN_CAP 1024
#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0]))

static const char *KEYWORDS[] = {
	"write",
	"int",
	"float"
};

#endif
