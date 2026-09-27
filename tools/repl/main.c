#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../src/frontend/parser/parser.h"
#include "../../src/frontend/lexer/tokenization.h"

int main(int argc, char **argv)
{
    puts("Korva version 0.0.1");
    puts("Press Ctr+C to exit.");

    while (1) {
        char buffer[256];
        printf(">>> ");
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            Token *tokens = NULL;
            Result result = tokenize(&tokens, buffer);
            print_result(result);
            if (result.code != 0) {
                continue;
            }
            size_t token_len = (size_t)(uintptr_t) result.value;
            if (token_len == 0) {
                continue;
            }

            printf("\n");

            if (parse(tokens, token_len).code != 0) {
                continue;
            }
            printf("Parsed %ld items:\n", token_len);
            for (int i = 0; i < token_len; i++) {
                printf("[%d] '%s' in %d:%d\n", tokens[i].token, tokens[i].lexeme, tokens[i].lineno, tokens[i].colno);
            }
        }
    }
    return 0;
}
