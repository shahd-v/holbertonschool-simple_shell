#include "shell.h"

/**
 * main - entry point of the shell
 * @argc: argument count (unused)
 * @argv: argument vector; argv[0] used for error messages
 *
 * Return: 0 on success
 */
int main(int argc, char **argv)
{
	int stat;
	(void)argc;
	stat = shell_loop(argv[0]);
	return (stat);
}

/**
 * shell_loop - main read-eval-print loop
 * @prog_name: name of the program (argv[0])
 *
 * Return: the status.
 */
int shell_loop(char *prog_name)
{
	char *line = NULL;
	size_t len = 0;
	ssize_t nread;
	char **args;
	int line_num = 0, interactive, stat = 0;

	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "WriteSomething;> ", 17);
		nread = getline(&line, &len, stdin);
		line_num++;
		if (nread == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			return (stat);
		}
		args = tokenize(line);
		if (args == NULL || args[0] == NULL)
		{
			free_tokens(args);
			continue;
		}
		if (handle_builtin(args, line))
		{
			free_tokens(args);
			continue;
		}
		stat = execute_command(args, prog_name, line_num);
		free_tokens(args);
	}
	free(line);
	return (stat);
}
