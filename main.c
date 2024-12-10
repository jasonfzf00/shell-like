#include "main.h"

int main(int argc, char **argv)
{
    (void)argc, (void)argv;
    char *line = NULL, *path;
    char **array;
    pid_t child_pid;
    int status;

    init_history();
    enable_raw_mode();

    while (1)
    {
        write(STDOUT_FILENO, "JShell$ ", 9);
        
        line = get_input();
        if (line == NULL)
        {
            perror("Failed to read line");
            break;
        }

        line[strcspn(line, "\n")] = '\0';
        
        add_to_history(line);

        if (strcmp(line, "exit") == 0)
        {
            write(STDOUT_FILENO, "Exiting shell\n", 15);
            break;
        }

        array = tokenize(line);

        // Check if command is cd
        if (strcmp(array[0], "cd") == 0)
        {
            handle_cd(array);
            free(array);
            continue;
        }

        path = get_file_path(array[0]);

        // Create a child process to execute the command
        child_pid = fork();

        if (child_pid == -1)
        {
            perror("Failed to create.");
            free(array);
            free(line);
            exit(1);
        }

        if (child_pid == 0)
        {
            if (execve(path, array, NULL) == -1)
            {
                perror("Failed to execute");
                free(array);
                free(line);
                exit(1);
            }
        }
        else
        {
            wait(&status);
        }

        free(array);
    }

    cleanup_history();
    return 0;
}
