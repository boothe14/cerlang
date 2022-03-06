#include <stddef.h>
#include <stdio.h>
#include "lexer.h"
#include <stdlib.h>

int main(void)
{
	lexer_init();
	lexer_load_program_from_file("examples/hello.lang");

	token_T *tokens = malloc(sizeof(token_T)*LEXER_TOKEN_CAP);
	size_t tokens_len = 0;

	lexer_run(&tokens, &tokens_len);
	debug_tokens(tokens, tokens_len);
	printf("End of file\n");

	return 0;
}
