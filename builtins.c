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
 * is_number - checks if a string represents a valid integer
 * @s: the string to check
 *
 * Description: This function verifies that the given string contains
 * only digits, allowing an optional leading '+' or '-'. It is used to
 * validate numeric arguments for built-in commands such as "exit".
 *
 * Return: 1 if the string is a valid number, 0 otherwise.
 */
int is_number(char *s)
{
	int i = 0;

	if (s == NULL)
		return (0);
	if (s[0] == '-' || s[0] == '+')
		i++;
	for (; s[i] != '\0'; i++)
	{
		if (s[i] < '0' || s[i] > '9')
		return (0);
	}
	return (1);
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
	int code;
	if (strcmp(args[0], "exit") == 0)
	{
		if (args[1] == NULL)
		{
			free_tokens(args);
			free(line);
			exit(last_status);
		}
		if (!is_number(args[1]))
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", args[1]);
			free_tokens(args);
			free(line);
			exit(2);
		}
		code = atoi(args[1]);
		free_tokens(args);
		free(line);
		exit(code);
	}
	if (strcmp(args[0], "env") == 0)
	{
		print_env();
		return (1);
	}
	return (0);
}
