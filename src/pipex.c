/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gharazka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 21:23:45 by gharazka          #+#    #+#             */
/*   Updated: 2024/08/21 21:23:46 by gharazka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	open_files(t_program *program)
{
	program->infile_fd = open(program->infile_name, O_RDONLY, 0444);
	program->outfile_fd = open(program->outfile_name,
			O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (program->infile_fd == -1 || program->outfile_fd == -1)
	{
		ft_errprintf("/bin/sh: 1: cannot open %s: No such file\n",
			program->infile_name);
		exit(1);
	}
}

t_program	init_struct(int argc, char *argv)
{
	t_program	program;

	program.command = NULL;
	program.command_path = NULL;
	return (program);
}

void	execute_last(t_program *program, char **envp, char *argv)
{
	int	id;

	signal(SIGQUIT, &handler);
	if (get_command_path(envp, program, argv) == -1)
	{
		ft_errprintf("/bin/sh: 1: %s: not found\n", argv);
	}
	id = fork();
	if (id == -1)
		free_and_exit(*program, "fork failed");
	signal(SIGINT, &handler);
	if (id == 0)
	{
		if (execve(program->command_path, program->command, envp) == -1)
			free_and_exit(*program, "");
	}
	else
		waitpid(id, NULL, 0);
}

void	execute(t_program *program, char **envp, char *argv)
{
	int	id;
	int	fd[2];

	if (get_command_path(envp, program, argv) == -1)
	{
		ft_errprintf("/bin/sh: 1: %s: not found\n", argv);
	}
	pipe(fd);
	id = fork();
	if (id == -1)
		free_and_exit(*program, "fork failed");
	if (id == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (execve(program->command_path, program->command, envp) == -1)
			free_and_exit(*program, "");
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
	}
}

void	pipex(int split_len, char *split_line[], char *envp[])
{
	t_program	program;
	int			i;
	t_token **tokens;

	i = 0;
	program = init_struct(1, split_line[0]);
	while (i < split_len - 1)
	{
		execute(&program, envp, split_line[i]);
		i++;
	}
	execute_last(&program, envp, split_line[split_len - 1]);
	exit(0);
}
/// Here below is new code

void execute_command(char *envp[], char *command)
{
    char *argv[] = {command, NULL};
    char *path = getenv("PATH");
    char *full_path = NULL;
    char *token = strtok(path, ":");

    while (token != NULL)
    {
        full_path = malloc(strlen(token) + strlen(command) + 2);
        if (!full_path)
        {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        sprintf(full_path, "%s/%s", token, command);
        execve(full_path, argv, envp);
        free(full_path);
        token = strtok(NULL, ":");
    }
    perror("execve");
    exit(EXIT_FAILURE);
}

void process_tokens(t_list *tokens, char *envp[])
{
    while (tokens)
    {
        if (((t_token *)tokens->content)->type == TOKEN_COMMAND)
        {
            if (strcmp(((t_token *)tokens->content)->value, "echo") == 0)
            {
                printf("$ Its echo command sukkaa why not showing\n");
                // Handle echo command
                tokens = tokens->next;
                while (tokens && ((t_token *)tokens->content)->type == TOKEN_TEXT)
                {
                    printf("$ %s", ((t_token *)tokens->content)->value);
                	tokens = tokens->next;
                }
                printf("\n");
            }
            else if (strcmp(((t_token *)tokens->content)->value, "cd") == 0)
            {
                // Handle cd command
            	tokens = tokens->next;
                if (tokens && ((t_token *)tokens->content)->type == TOKEN_TEXT)
                {
                    // Process cd argument
                	tokens = tokens->next;
                }
            }
            else if (strcmp(((t_token *)tokens->content)->value, "pwd") == 0)
            {
                // Handle pwd command
				execute_command(envp, ((t_token *)tokens->content)->value);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "export") == 0)
            {
                // Handle export command
				execute_command(envp, ((t_token *)tokens->content)->value);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "unset") == 0)
            {
                // Handle unset command
				execute_command(envp, ((t_token *)tokens->content)->value);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "env") == 0)
            {
                // Handle env command
				execute_command(envp, ((t_token *)tokens->content)->value);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "exit") == 0)
            {
                // Handle exit command
				execute_command(envp, ((t_token *)tokens->content)->value);
            	tokens = tokens->next;
            }
        }
        else if (((t_token *)tokens->content)->type == TOKEN_TEXT)
        {
			printf("$ executing..\n");
            execute_command(envp, ((t_token *)tokens->content)->value);
        }
        else if (((t_token *)tokens->content)->type == TOKEN_R_INPUT)
        {
            // Handle input redirection
        }
        else if (((t_token *)tokens->content)->type == TOKEN_R_OUTPUT)
        {
            // Handle output redirection
        }
        else if (((t_token *)tokens->content)->type == TOKEN_A_OUTPUT)
        {
            // Handle append output redirection
        }
        else if (((t_token *)tokens->content)->type == TOKEN_HERE_DOC)
        {
            // Handle here document
        }
    	tokens = tokens->next;
    }
    // execute_last(&program, envp, tokens[i - 1]->value);
}