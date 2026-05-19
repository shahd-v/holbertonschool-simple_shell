#include "shell.h"

/**
 * get_env_value - finds a variable in environ by name
 * @name: variable name (e.g. "PATH")
 *
 * Return: pointer to value (after '='), or NULL if not found
 */
char *get_env_value(const char *name)
{
	int i = 0;
	size_t nlen = strlen(name);

	while (environ[i])
	{
		if (strncmp(environ[i], name, nlen) == 0 && environ[i][nlen] == '=')
			return (environ[i] + nlen + 1);
		i++;
	}
	return (NULL);
}

/**
 * build_path - joins a directory and command with '/'
 * @dir: directory part (may be empty, meaning ".")
 * @cmd: command name
 *
 * Return: malloc'd full path, or NULL on failure
 */
static char *build_path(const char *dir, const char *cmd)
{
	size_t dlen, clen;
	char *full;

	if (dir == NULL || *dir == '\0')
		dir = ".";
	dlen = strlen(dir);
	clen = strlen(cmd);
	full = malloc(dlen + clen + 2);
	if (full == NULL)
		return (NULL);
	strcpy(full, dir);
	full[dlen] = '/';
	strcpy(full + dlen + 1, cmd);
	return (full);
}

/**
 * find_in_path - searches PATH directories for an executable command
 * @command: command name (e.g. "ls")
 *
 * Return: malloc'd full path if found and executable, NULL otherwise.
 *         If command contains '/', returns a duplicate of command if it
 *         is accessible.
 */
char *find_in_path(char *command)
{
	char *path, *path_dup, *dir, *full;

	if (command == NULL || *command == '\0')
		return (NULL);
	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) == 0)
			return (strdup(command));
		return (NULL);
	}
	path = get_env_value("PATH");
	if (path == NULL || *path == '\0')
		return (NULL);
	path_dup = strdup(path);
	if (path_dup == NULL)
		return (NULL);
	dir = strtok(path_dup, ":");
	while (dir)
	{
		full = build_path(dir, command);
		if (full && access(full, X_OK) == 0)
		{
			free(path_dup);
			return (full);
		}
		free(full);
		dir = strtok(NULL, ":");
	}
	free(path_dup);
	return (NULL);
}
