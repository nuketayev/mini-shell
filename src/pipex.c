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

void	execute_last(char **envp, char **args)
{
	int	id;
	char	*cmd_path;

	cmd_path = get_command_path(envp, args[0]);
	id = fork();
	if (id == 0)
	{
		if (execve(cmd_path, args, envp) == -1)
			printf("nie dziala kurwa\n");
	}
	else
		waitpid(id, NULL, 0);
}

void	execute(char **envp, char **args)
{
	int	id;
	int	fd[2];
	char	*cmd_path;

	cmd_path = get_command_path(envp, args[0]);
	pipe(fd);
	id = fork();
	if (id == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (execve(cmd_path, args, envp) == -1)
			printf("nie dziala kurwa\n");

	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
	}
}

char	**ft_combine(t_list **command)
{
	char	**args;
	int		i;

	args = (char **)malloc(10);
	i = 0;
	while ((*command) && ((t_token *)(*command)->content)->type == TOKEN_TEXT)
	{
		args[i] = ft_strdup(((t_token *)(*command)->content)->value);
		i++;
		*command = (*command)->next;
	}
	args[i] = NULL;
	return args;
}

void	pipex(t_list **command, char *envp[])
{
	int			i;
	char		**args = ft_combine(command);

	i = 0;
	while (args[i])
	{
		execute_last(envp, args);
		i++;
	}
	exit(0);
}
/// Here below is new code



// void execute_command(char *envp[], t_list **command)
// {
//     char *argv[] = {command, NULL};
//     char *path = getenv("PATH");
//     char *full_path = NULL;
//     char *token = strtok(path, ":");
//
// 	while (command && command)
// 	{
// 		printf("%s\n", ((t_token *)(*command)->content)->value);
// 		command = &(*command)->next;
// 	}
//     // while (token != NULL)
//     // {
// 	   //  full_path = malloc(strlen(token) + strlen(command) + 2);
// 	   //  if (!full_path)
// 	   //  {
// 		  //   perror("malloc");
// 		  //   exit(EXIT_FAILURE);
// 	   //  }
// 	   //  sprintf(full_path, "%s/%s", token, command);
// 	   //  execve(full_path, argv, envp);
// 	   //  free(full_path);
// 	   //  token = strtok(NULL, ":");
//     // }
//     exit(EXIT_FAILURE);
// }

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
				pipex(&tokens, envp);
            }
            else if (strcmp(((t_token *)tokens->content)->value, "export") == 0)
            {
                // Handle export command
            	pipex(&tokens, envp);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "unset") == 0)
            {
                // Handle unset command
            	pipex(&tokens, envp);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "env") == 0)
            {
                // Handle env command
            	pipex(&tokens, envp);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "exit") == 0)
            {
                // Handle exit command
            	pipex(&tokens, envp);
            	tokens = tokens->next;
            }
        }
        else if (((t_token *)tokens->content)->type == TOKEN_TEXT)
        {
			printf("$ executing..\n");
        	pipex(&tokens, envp);
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