#include "shell.h"

/**
 * find_path - search for a command in the PATH directories
 * @cmd: the command name (e.g. "ls")
 *
 * Description: This function checks whether a command contains>
 * If it does, the function verifies that the path is executabl>
 * returns a malloc'd copy. If the command has no slash, the fu>
 * searches through the PATH environment variable, building ful>
 * for each directory until an executable match is found.
 *
 * Return: A malloc'd string containing the full path to the co>
 * or NULL if the command cannot be found or is not executable.
 */
char *find_path(char *cmd)
{
char *copy, *dir, *full;
int i;
if (strchr(cmd, '/'))
{
if (access(cmd, X_OK) == 0)
return (strdup(cmd));
return (NULL);
}
copy = NULL;
for (i = 0; environ[i]; i++)
{
if (strncmp(environ[i], "PATH=", 5) == 0)
{
copy = strdup(environ[i] + 5);
break;
}
}
if (!copy)
return (NULL);
dir = strtok(copy, ":");
while (dir)
{
full = malloc(strlen(dir) + strlen(cmd) + 2);
if (!full)
{
free(copy);
return (NULL);
}
sprintf(full, "%s/%s", dir, cmd);
if (access(full, X_OK) == 0)
{
free(copy);
return (full);
}
free(full);
dir = strtok(NULL, ":");
}
free(copy);
return (NULL);
}
