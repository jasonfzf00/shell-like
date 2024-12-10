#include "main.h"
#include <termios.h>
#include <ctype.h>

#define HISTORY_MAX 1000
#define ARROW_UP "\x1b[A"
#define ARROW_DOWN "\x1b[B"

static char **history = NULL;
static int history_count = 0;
static int current_position = 0;

void init_history(void)
{
    history = malloc(sizeof(char *) * HISTORY_MAX);
    if (!history)
    {
        perror("Failed to allocate history");
        exit(1);
    }
}

void add_to_history(const char *line)
{
    if (!line || line[0] == '\0' || line[0] == '\n')
        return;

    // Remove newline character if present
    char *clean_line = strdup(line);
    char *newline = strchr(clean_line, '\n');
    if (newline) *newline = '\0';

    // Avoid adding duplicate commands
    if (history_count > 0 && strcmp(history[history_count - 1], clean_line) == 0)
    {
        free(clean_line);
        return;
    }
    
    if (history_count >= HISTORY_MAX)
    {
        free(history[0]);
        memmove(history, history + 1, (HISTORY_MAX - 1) * sizeof(char *));
        history_count--;
    }

    history[history_count++] = clean_line;
    current_position = history_count;
}

static struct termios orig_termios;

void disable_raw_mode(void)
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode(void)
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

char *get_input(void)
{
    char *buffer = malloc(1024);
    int pos = 0;
    int len = 1024;
    buffer[0] = '\0';

    while (1)
    {
        char c;
        ssize_t nread = read(STDIN_FILENO, &c, 1);

        if (nread <= 0) continue;

        if (c == '\x1b')
        {
            char seq[3];
            
            if (read(STDIN_FILENO, &seq[0], 1) != 1) continue;
            if (read(STDIN_FILENO, &seq[1], 1) != 1) continue;

            if (seq[0] == '[')
            {
                switch (seq[1])
                {
                    case 'A': // Up arrow
                        if (current_position > 0)
                        {
                            current_position--;
                            // Clear current line
                            while (pos > 0)
                            {
                                write(STDOUT_FILENO, "\b \b", 3);
                                pos--;
                            }
                            strcpy(buffer, history[current_position]);
                            pos = strlen(buffer);
                            write(STDOUT_FILENO, buffer, pos);
                        }
                        break;

                    case 'B': // Down arrow
                        if (current_position < history_count)
                        {
                            current_position++;
                            // Clear current line
                            while (pos > 0)
                            {
                                write(STDOUT_FILENO, "\b \b", 3);
                                pos--;
                            }
                            if (current_position == history_count)
                                buffer[0] = '\0';
                            else
                                strcpy(buffer, history[current_position]);
                            pos = strlen(buffer);
                            write(STDOUT_FILENO, buffer, pos);
                        }
                        break;
                }
            }
            continue;
        }

        if (c == '\n')
        {
            buffer[pos] = '\n';
            buffer[pos + 1] = '\0';
            write(STDOUT_FILENO, "\n", 1);
            return buffer;
        }

        if (c == 127) // Backspace
        {
            if (pos > 0)
            {
                pos--;
                buffer[pos] = '\0';
                write(STDOUT_FILENO, "\b \b", 3);
            }
            continue;
        }

        if (!iscntrl(c))
        {
            if (pos >= len - 1)
            {
                len *= 2;
                buffer = realloc(buffer, len);
            }
            buffer[pos++] = c;
            buffer[pos] = '\0';
            write(STDOUT_FILENO, &c, 1);
        }
    }
}

void cleanup_history(void)
{
    if (history)
    {
        for (int i = 0; i < history_count; i++)
        {
            free(history[i]);
        }
        free(history);
    }
} 
