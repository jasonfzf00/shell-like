#include "main.h"

char **get_matching_files(const char *partial_name, int *count) {
    DIR *dir;
    struct dirent *entry;
    char **matches = malloc(sizeof(char *) * 100);
    *count = 0;

    // Open current directory
    dir = opendir(".");
    if (!dir) {
        return NULL;
    }

    // Read directory entries and add matches to the array
    while ((entry = readdir(dir)) != NULL) {
        if (strncmp(entry->d_name, partial_name, strlen(partial_name)) == 0) {
            matches[*count] = strdup(entry->d_name);
            (*count)++;
        }
    }

    closedir(dir);
    return matches;
}

void handle_tab_completion(char *buffer, int *pos) {
    char partial_name[256] = {0};
    int last_space = -1;
    int count = 0;

    // Find the last space in the buffer
    for (int i = 0; i < *pos; i++) {
        if (buffer[i] == ' ') {
            last_space = i;
        }
    }

    // Extract partial name after the last space
    strncpy(partial_name, buffer + last_space + 1, *pos - last_space - 1);

    // Get matching files
    char **matches = get_matching_files(partial_name, &count);

    if (count == 0) {
        // No matches found
        return;
    } else if (count == 1) {
        // Single match - complete the name
        int match_len = strlen(matches[0]);
        while (*pos > last_space + 1) {
            write(STDOUT_FILENO, "\b \b", 3);
            (*pos)--;
        }
        strcpy(buffer + last_space + 1, matches[0]);
        write(STDOUT_FILENO, matches[0], match_len);
        *pos += match_len;
    } else {
        // Multiple matches - display all possible completions
        write(STDOUT_FILENO, "\n", 1);
        for (int i = 0; i < count; i++) {
            write(STDOUT_FILENO, matches[i], strlen(matches[i]));
            write(STDOUT_FILENO, "  ", 2);
        }
        write(STDOUT_FILENO, "\nJShell$ ", 9);
        write(STDOUT_FILENO, buffer, *pos);
    }
    
    for (int i = 0; i < count; i++) {
        free(matches[i]);
    }
    free(matches);
} 
