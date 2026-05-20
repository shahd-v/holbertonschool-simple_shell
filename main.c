#include "shell.h"
static void handle_child(char **args, char *line);
static int handle_fork(char **args, char *line);
static int process_line(char *line);
int last_status = 0;

/**
 * main - entry point for simple shell
 * Return: 0 on success
 */
int main(void)
{
	char *line;
	while (1)
	{

		if (isatty(STDIN_FILENO))
			write(STDOUT_FILENO, "WriteSomeThing;>$ ", 18);

		line = read_line();

		if (line == NULL)
		{
			if (isatty(STDIN_FILENO))
				write(STDOUT_FILENO, "\n", 1);

			break;
		}

		process_line(line);
		free(line);
	}
	return (last_status);
}

/**
 * process_line - processes an input line and executes its command
 * @line: pointer to the input line buffer
 *
 * Description: Trims the newline, checks if the line is empty, splits
 * it into tokens, and executes the resulting command. Handles memory
 * cleanup for the token array and manages error cases.
 *
 * Return: exit status of the executed command, or -1 on failure
 */
static int process_line(char *line)
{
	char **args;
	char *cmd;
	int i = 0;
	trim_newline(line);

	if (is_empty(line))
		return (0);

	args = split_line(line);

	if (args == NULL)
		return (0);

	if (strcmp(args[0], "exit") == 0)
	{
		if (args[1] == NULL)
		{
			free(args);
			free(line);
			exit(last_status);
		}

		if (!is_number(args[1]))
		{
			fprintf(stderr, "./hsh: 1: exit: Illegal number: %s\n", args[1]);
			free(args);
			free(line);
			exit(2);
		}
		i = atoi(args[1]);
		free(args);
		free(line);
		exit(i);
	}

	if (strcmp(args[0], "env") == 0)
	{
		print_env();
		free(args);
		return (0);
	}

	cmd = find_path(args[0]);

	if (!cmd)
	{
		fprintf(stderr, "./hsh: 1: %s: not found\n", args[0]);
		last_status = 127;
		free(args);
		return (0);
	}

	handle_fork(args, cmd);
	free(cmd);
	free(args);

	return (0);
}

/**
 * handle_fork - creates a child process to execute a command
 * @args: array of argument strings for the command
 * @cmd: pointer to the input line buffer
 *
 * Description: Forks the current process. In the child process>
 * command is executed. In the parent process, waits for the ch>
 * finish and returns its exit status. Frees memory and handles>
 * errors appropriately.
 *
 * Return: exit status of the child process, or -1 on fork fail>
 */
static int handle_fork(char **args, char *cmd)
{
	pid_t pid;
	int status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (-1);
	}

	if (pid == 0)
		handle_child(args, cmd);

	waitpid(pid, &status, 0);
	last_status = status >> 8;

	return (0);
}

/**
 * handle_child - executes a command in the child process
 * @args: array of argument strings for the command
 * @cmd: pointer to the input line buffer
 *
 * Description: Forks and executes the given command using exec>
 * Frees the input line and argument array in the child before
 * terminating. Handles execution errors and exits with status 1
 * if the command cannot be executed.
 */
static void handle_child(char **args, char *cmd)
{

	if (execve(cmd, args, environ) == -1)
	{
		fprintf(stderr, "./hsh: %s: No such file or directory\n", args[>
		free(args);
		free(cmd);
		exit(127);
	}

}
