#include "shell.h"

/**
 * count_tokens - counts whitespace-separated tokens
 * @s: input string (will be duplicated internally)
 *
 * Return: number of tokens
 */
static int count_tokens(char *s)
{
	int count = 0;
	char *dup, *tok;

	dup = strdup(s);
	if (dup == NULL)
		return (0);
	tok = strtok(dup, " \t\n");
	while (tok)
	{
		count++;
		tok = strtok(NULL, " \t\n");
	}
	free(dup);
	return (count);
}

/**
 * tokenize - splits a line into tokens
 * @line: input line from getline
 *
 * Return: NULL-terminated array of malloc'd strings, or NULL on failure
 */
char **tokenize(char *line)
{
	int n, i = 0;
	char **tokens, *tok;

	if (line == NULL)
		return (NULL);
	n = count_tokens(line);
	tokens = malloc(sizeof(char *) * (n + 1));
	if (tokens == NULL)
		return (NULL);
	tok = strtok(line, " \t\n");
	while (tok)
	{
		tokens[i] = strdup(tok);
		if (tokens[i] == NULL)
		{
			free_tokens(tokens);
			return (NULL);
		}
		i++;
		tok = strtok(NULL, " \t\n");
	}
	tokens[i] = NULL;
	return (tokens);
}

/**
 * free_tokens - frees a NULL-terminated array of strings
 * @tokens: array to free
 */
void free_tokens(char **tokens)
{
	int i = 0;

	if (tokens == NULL)
		return;
	while (tokens[i])
	{
		free(tokens[i]);
		i++;
	}
	free(tokens);
}
