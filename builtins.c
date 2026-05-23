#include "shell.h"

/**
 * print_env - Prints the current environment variables
 *
 * Description: This function iterates through the global environment
 * array and prints each variable in the format 'KEY=VALUE'. It is used
 * to implement the 'env' built-in command in the shell.
 *
 * Return: Always 0.
 */
int print_env(void)
{
	int i;

	for (i = 0; environ[i] != NULL; i++)
	{
		write(STDOUT_FILENO, environ[i], strlen(environ[i]));
		write(STDOUT_FILENO, "\n", 1);
	}

	return (0);
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
