#include "shell.h"
int last_status = 0;

/**
 * print_not_found - prints "prog: line: cmd: not found" to stderr
 * @prog_name: argv[0] of the shell
 * @line_num: current command line number
 * @cmd: command that wasn't found
 */
static void print_not_found(char *prog_name, int line_num, char *cmd)
{
	fprintf(stderr, "%s: %d: %s: not found\n", prog_name, line_num, cmd);
}

/**
 * execute_command - resolves the command via PATH, forks, and execs
 * @args: NULL-terminated argument vector
 * @prog_name: argv[0] of the shell (for error messages)
 * @line_num: line number for error messages
 *
 * Return: exit status of the child (or 127 if not found)
 */
int execute_command(char **args, char *prog_name, int line_num)
{
	char *full_path;
	pid_t pid;
	int status = 0;

	full_path = find_in_path(args[0]);
	if (full_path == NULL)
	{
		print_not_found(prog_name, line_num, args[0]);
		last_status = 127;
		return (127);
	}
	pid = fork();
	if (pid == -1)
	{
		perror(prog_name);
		free(full_path);
		last_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		if (execve(full_path, args, environ) == -1)
		{
			perror(prog_name);
			free(full_path);
			exit(127);
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		free(full_path);

		if (WIFEXITED(status))
		{
			last_status = WEXITSTATUS(status);
			return (last_status);
		}
	}
	return (last_status);
}
