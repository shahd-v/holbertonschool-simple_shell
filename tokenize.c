#include "shell.h"

/**
 * read_line - gets a full line of input from stdin
 *
 * Description: Allocates and returns a buffer containing the l>
 * read from standard input. Handles dynamic resizing as needed.
 *
 * Return: pointer to the allocated buffer, or NULL on failure >
 */
char *read_line(void)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;

	nread = getline(&line, &len, stdin);
	if (nread == -1)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * trim_newline - removes the trailing newline from a string
 * @line: pointer to the string to modify
 *
 * Description: Checks if the last character in the string is a>
 * ('\n') and replaces it with a null terminator. Does nothing >
 * string is NULL or does not contain a newline at the end.
 */
void trim_newline(char *line)
{
	int len = strlen(line);

	while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'
		|| line[len - 1] == ' ' || line[len - 1] == '\t'))
			line[--len] = '\0';

}

/**
 * is_empty - checks if a string contains only whitespace or is>
 * @line: pointer to the string to check
 *
 * Description: Determines whether the given string is NULL, an>
 * string, or consists only of whitespace characters (spaces, t>
 *
 * Return: 1 if the string is empty or whitespace-only, 0 other>
 */
int is_empty(const char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '\t' && *line != '\n')
		return (0);
		line++;
	}

	return (1);
}
/**
 * split_line - splits a line into an array of tokens
 * @line: pointer to the input string to tokenize
 *
 * Description: Breaks the given line into separate tokens using
 * whitespace as delimiters. Allocates memory for the array of
 * token strings. The last element of the returned array is NUL>
 *
 * Return: pointer to an array of token strings, or NULL on fai>
 */
char **split_line(char *line)
{
	char **tokens;
	char *token;
	int i = 0;

	tokens = malloc(64 * sizeof(char *));
	if (!tokens)
		return (NULL);

	token = strtok(line, " \t");

	while (token != NULL)
	{
		tokens[i++] = token;
		token = strtok(NULL, " \t");
	}

	tokens[i] = NULL;

	return (tokens);
}

