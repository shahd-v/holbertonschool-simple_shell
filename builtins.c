#include "shell.h"

/**
 * print_env - prints each environment variable on its own line (Task 6)
 */
void print_env(void)
{
	int i = 0;

	while (environ[i])
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/**
 * handle_builtin - checks for and executes built-in commands
 * @args: tokenized command
 * @line: the raw line buffer (freed before exit to avoid leaks)
 *
 * Return: 1 if a built-in was handled, 0 otherwise
 */
int handle_builtin(char **args, char *line)
{
	if (strcmp(args[0], "exit") == 0)
	{
		free_tokens(args);
		free(line);
		exit(0);
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_env();
		return (1);
	}
	return (0);
}
