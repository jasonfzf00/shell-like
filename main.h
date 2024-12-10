#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <string.h>
#include <termios.h>

char *get_file_path(char *file_name);
char *get_file_loc(char *path, char *file_name);
int startsWithForwardSlash(const char *str);
char **tokenize(char *line);
void execute_pipeline(char *line);
char **split_commands(char *line, const char *delim);
void handle_cd(char **args);
void init_history(void);
void add_to_history(const char *line);
char *get_input(void);
void cleanup_history(void);
void enable_raw_mode(void);
void disable_raw_mode(void);

#endif /* MAIN_H */
