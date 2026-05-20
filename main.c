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
	(void)argc;
	shell_loop(argv[0]);
	exit(last_status);
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
	int line_num = 0, interactive;

	interactive = isatty(STDIN_FILENO);
	while (1)
	{
		if (interactive)
			write(STDOUT_FILENO, "WriteSomeThing;>$ ", 18);
		nread = getline(&line, &len, stdin);
		line_num++;
		if (nread == -1)
		{
			if (interactive)
				write(STDOUT_FILENO, "\n", 1);
			free(line);
			return (last_status);
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
		last_status = execute_command(args, prog_name, line_num);
		free_tokens(args);
	}
	free(line);
	return (last_status);
}
