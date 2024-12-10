#include "main.h"

char **tokenize(char *line) {
    // Allocate memory for tokens
    char **tokens = malloc(sizeof(char *) * 1024); 
    if (!tokens) {
        perror("Memory allocation failed");
        exit(1);
    }

    char *token = strtok(line, " \n");
    int i = 0;
    while (token != NULL) {
        tokens[i] = token; // Add token to array
        token = strtok(NULL, " \n");
        i++;
    }

    tokens[i] = NULL; // Null-terminate the array
    return tokens;
}

void handle_cd(char **args)
{
    char *home_dir;

    // If no argument is provided or "~" is provided, change to HOME directory
    if (args[1] == NULL || strcmp(args[1], "~") == 0)
    {
        home_dir = getenv("HOME");
        if (home_dir == NULL)
        {
            perror("cd: HOME not set");
            return;
        }
        if (chdir(home_dir) != 0)
        {
            perror("cd failed");
        }
        return;
    }

    // Change to the specified directory
    if (chdir(args[1]) != 0)
    {
        perror("cd failed");
    }
}

