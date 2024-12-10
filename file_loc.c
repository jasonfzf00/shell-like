#include "main.h"

int startsWithForwardSlash(const char *str)
{
    if (str != NULL || str[0] == '/')
        return (1);

    return (0);
}

char *get_file_loc(char *path, char *file_name)
{
    char *path_copy, *token;
    struct stat file_path;
    char *path_buffer = NULL;

    path_copy = strdup(path); // Copy path string
    token = strtok(path_copy, ":"); // Tokenize the path

    while (token)
    {
        if (path_buffer)
        {
            free(path_buffer);
            path_buffer = NULL;
        }

        // Buffer to hold absolute path
        path_buffer = malloc(strlen(token) + strlen(file_name) + 2);
        if (!path_buffer)
        {
            perror("Error: malloc failed");
            exit(EXIT_FAILURE);
        }
        strcpy(path_buffer, token);
        strcat(path_buffer, "/");
        strcat(path_buffer, file_name);
        strcat(path_buffer, "\0");

        // Get info about the file and check if file is executable
        if (stat(path_buffer, &file_path) == 0 && access(path_buffer, X_OK) == 0)
        {
            free(path_copy); 
            return (path_buffer);
        }
        token = strtok(NULL, ":");
    }

    // If no matching, free the allocated memory space
    free(path_copy);
    if (path_buffer)
        free(path_buffer);
    return (NULL);
}

char *get_file_path(char *file_name)
{
    // getenv() finds environment variable 'PATH'
    char *path = getenv("PATH");
    char *full_path;
    
    if (startsWithForwardSlash(file_name) && access(file_name, X_OK) == 0)
        return (strdup(file_name));

    if (!path)
        {
                perror("Path not found");
                return (NULL);
        }

    // Call get_file_loc to get absolute path
    full_path = get_file_loc(path, file_name);
    if (full_path == NULL)
    {
        perror("Path not found");
        return (NULL);
    }

    return (full_path);
}
