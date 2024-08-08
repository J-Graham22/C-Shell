#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <Windows.h>

#include "shell.h"

#define SHELL_LH_BUFFER_SIZE 1024

#define SHELL_TOK_BUFFER_SIZE 64
#define SHELL_TOK_DELIMITER " \t\r\n\a"

void shell_loop(void) {
    char *line;
    char **args;
    int status;

    do {
        printf("> ");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_execute(args);

        free(line);
        free(args);
    } while (status);
}

char* shell_read_line(void) {
    int buffer_size = SHELL_LH_BUFFER_SIZE;
    int pos = 0;
    char* buffer = malloc(buffer_size * sizeof(char));
    int character;

    if (buffer == NULL) {
        fprintf(stderr, "shell: error allocating memory to buffer\n");
        exit(EXIT_FAILURE);
    }

    while(1) {
        character = getchar();

        if(character == EOF || character == '\n') {
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos] = character;
        }
        pos++;

        if(pos >= buffer_size) {
            buffer_size += SHELL_LH_BUFFER_SIZE;
            buffer = realloc(buffer, buffer_size);

            if(buffer == NULL) {
                fprintf(stderr, "shell: error reallocating memory to buffer\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}

char **shell_split_line(char *line) {
    int buffer_size = SHELL_TOK_BUFFER_SIZE, pos = 0;
    char **tokens = malloc(buffer_size * sizeof(char*));
    char *token;

    if(tokens == NULL) {
        fprintf(stderr, "shell: error allocating memory for tokens\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SHELL_TOK_DELIMITER);
    while(token != NULL) {
        tokens[pos] = token;
        pos++;

        if(pos >= buffer_size) {
            buffer_size += SHELL_TOK_BUFFER_SIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char*));

            if(tokens == NULL) {
                fprintf(stderr, "shell: error reallocating memory for tokens\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, SHELL_TOK_DELIMITER);
    }
    tokens[pos] = NULL;
    return tokens;
}

int shell_execute(char **args) {
    int status;

    if(CreateProcess(args[0], args) == -1) {
        perror("shell");
    }
    exit(EXIT_FAILURE);
}