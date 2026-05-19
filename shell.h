#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

extern char **environ;

/* main.c */
void shell_loop(char *prog_name);

/* tokenize.c */
char **tokenize(char *line);
void free_tokens(char **tokens);

/* path.c */
char *find_in_path(char *command);
char *get_env_value(const char *name);

/* execute.c */
int execute_command(char **args, char *prog_name, int line_num);

/* builtins.c */
int handle_builtin(char **args, char *line);
void print_env(void);

#endif /* SHELL_H */
