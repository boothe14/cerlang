#include "lexer.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>

int lexer_advance()
{
	if(++lexer.pos < (int)strlen(lexer.text))
	{
		lexer.ch = lexer.text[lexer.pos];
		return 1;
	}

	return 0;
}

void lexer_init()
{
	lexer.pos = -1;
	lexer.ch = '\0';
}

void lexer_load_program_from_file(const char *filepath)
{
	assert(lexer.pos == -1);

	FILE *fp;
	fopen_s(&fp, filepath, "r");

	char *str = malloc(512*512);
	str[0] = '\0';

	char *buffer = malloc(512);

	while(fgets(buffer, 512, fp))
	{
		buffer[strcspn(buffer, "\n")] = 0;

		const char *const format = str[0] == 0 ? "%s%s" : "%s %s";
		snprintf(str, 512*512, format, str, buffer); 
	}

	lexer.text = str;
}

bool isspecial(char ch)
{
	switch(ch)
	{
		case '!': case '@':
		case '#': case '$':
		case '%': case '^':
		case '&': case '*':
		case '(': case ')':
		case '{': case '}':
		case '-': case '+':
		case '/': return 1;

		default: return 0;
	}
}

const char *lexer_make_word()
{
	char *str = malloc(512);
	str[0] = 0;

	for(;(lexer.pos < (int)strlen(lexer.text)) && !isspace(lexer.ch);lexer_advance())
	{
		char cur_chr = lexer.ch;
		snprintf(str, 512, "%s%c", str, cur_chr);
	}

	return str;
}

token_T lexer_make_number()
{
	char *str = malloc(512);
	str[0] = 0;

	bool isfloat = false;

	for(;lexer.pos < (int)strlen(lexer.text); lexer_advance())
	{
		char cur_chr = lexer.ch;
		if(isspace(cur_chr)) break;
		if(isspecial(cur_chr)) break;

		if(isdigit(cur_chr) || cur_chr == '.') {
			if(cur_chr == '.') isfloat = 1;

			snprintf(str, 512, "%s%c", str, cur_chr);
		} else {
			fprintf(stderr, "error: invalid character in number literal '%c' at index %d\n", cur_chr, lexer.pos);
			exit(-1);
		}
	}

	lexer.pos -= 1;

	int type = isfloat ? TOKEN_FLOAT : TOKEN_INT;
	return (token_T){.type=type, .value=str};
}

const char *lexer_make_string()
{
	char *str = malloc(512);
	str[0] = 0;

	for(;lexer.pos < (int)strlen(lexer.text) && lexer.ch != '"'; lexer_advance())
	{
		snprintf(str, 512, "%s%c", str, lexer.ch);
	}

	return str;
}

const char *toktype_as_str(int type)
{
	switch(type) {
		case TOKEN_INT: return "INT";
		case TOKEN_FLOAT: return "FLOAT";
		case TOKEN_OPERATOR: return "OPERATOR";
		case TOKEN_IDENTIFIER: return "IDENTIFIER";
		case TOKEN_STRING: return "STR";
		case TOKEN_KEYWORD: return "KEYWORD";

		default: {
			fprintf(stderr, "error: in toktype_as_str(), exhaustive switch handling\n");
			exit(-1);
		}
		break;
	}
}

const char *tok_as_str(const token_T tok)
{
	char *str = malloc(1024);
	str[0] = 0;

	snprintf(str, 1024, "%s: %s", toktype_as_str(tok.type), tok.value);
	return str;
}

token_T lexer_make_token()
{
	char cur_chr = lexer.ch;

	switch(cur_chr)
	{
		case '+': {
			return (token_T){.type=TOKEN_OPERATOR, .value="+"};
		}
		break;

		case '"': {
			lexer_advance();
			return (token_T){.type=TOKEN_STRING,. value=_strdup(lexer_make_string())};
		}
		break;

		case '-': {
			return (token_T){.type=TOKEN_OPERATOR, .value="-"};
		}
		break;

		case '/': {
			return (token_T){.type=TOKEN_OPERATOR, .value="/"};
		}
		break;

		case '*': {
			return (token_T){.type=TOKEN_OPERATOR, .value="*"};
		}
		break;

		case '=': {
			return (token_T){.type=TOKEN_OPERATOR, .value="="};
		}
		break;

		default: {
			if(isdigit(cur_chr)) {
				return lexer_make_number();
			} else {
				const char *word = lexer_make_word();
				if(word_in_keywords(word)) {
					return (token_T){.type=TOKEN_KEYWORD, .value=_strdup(word)};
				} else {
					return (token_T){.type=TOKEN_IDENTIFIER, .value=_strdup(word)};
				}
			}
		}
		break;
	}

	return (token_T){.type=TOKEN_INT, .value="null"};
}

bool word_in_keywords(const char *str)
{
	for(size_t i = 0; i < ARRAY_SIZE(KEYWORDS); ++i)
	{
		if(!strcmp(str, KEYWORDS[i])) return true;
	}

	return false;
}

void lexer_run(token_T **desttokens, size_t *destsize)
{
	token_T *tokens = malloc(LEXER_TOKEN_CAP);
	size_t tokens_len = 0;

	while(lexer_advance())
	{
		if(!isspace(lexer.ch)) tokens[tokens_len++] = lexer_make_token();
	}

	*desttokens = tokens;
	*destsize = tokens_len;
}

void debug_tokens(token_T *tokens, size_t destsize)
{
	for(size_t i = 0; i < destsize; ++i)
	{
		printf("%s\n", tok_as_str(tokens[i]));
	}
}
