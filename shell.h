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
extern int last_status;

/* main.c  static methods */

/* tokenize.c */
char *read_line(void);
void trim_newline(char *line);
int is_empty(const char *line);
char **split_line(char *line);

/* path.c */
char *find_path(char *cmd);

/* builtins.c */
int print_env(void);
int is_number(char *s);

#endif /* SHELL_H */
