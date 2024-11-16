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
	{
		waitpid(id, NULL, 0);
	}
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
		waitpid(id, NULL, 0);
	}
}

char	**ft_combine(t_list **command)
{
	char	**args;
	int		i;

	args = (char **)malloc(10);
	i = 0;
	while (((t_token *)(*command)->content)->type == TOKEN_TEXT || ((t_token *)(*command)->content)->type == TOKEN_LAST)
	{
		args[i] = ft_strdup(((t_token *)(*command)->content)->value);
		i++;
		*command = (*command)->next;
	}
	args[i] = NULL;
	return args;
}

void	pipex(t_list **command, char *envp[], t_token_type *first)
{
	char		**args = ft_combine(command);

	if (*first == TOKEN_LAST)
	{
		execute_last(envp, args);
	}
	else
		execute(envp, args);
	free_split(args);
}

int	handle_file(char *filename, t_token_type type)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_errprintf("wrong file kurwo\n");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	get_another_line(char **line)
{
	int		i;
	int		result;
	char	c;
	char	*buffer;

	i = 0;
	result = 0;
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	result = read(0, &c, 1);
	while (result && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		result = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = buffer;
	free(buffer);
	return (result);
}

void	limiter(char *limiter)
{
	pid_t	reader;
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		ft_errprintf("wrong pipe man\n");
	reader = fork();
	if (reader == 0)
	{
		close(fd[0]);
		while (get_another_line(&line))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
				exit(EXIT_SUCCESS);
			write(fd[1], line, ft_strlen(line));
		}
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}

void process_tokens(t_list *tokens, char *envp[])
{
    while (((t_token *)tokens->content)->type != TOKEN_END)
    {
        if (((t_token *)tokens->content)->type == TOKEN_COMMAND)
        {
            if (strcmp(((t_token *)tokens->content)->value, "echoa") == 0)
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
				pipex(&tokens, envp, 0);
            }
            else if (strcmp(((t_token *)tokens->content)->value, "export") == 0)
            {
                // Handle export command
            	pipex(&tokens, envp, 0);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "unset") == 0)
            {
                // Handle unset command
            	pipex(&tokens, envp, 0);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "env") == 0)
            {
                // Handle env command
            	pipex(&tokens, envp, 0);
            	tokens = tokens->next;
            }
            else if (strcmp(((t_token *)tokens->content)->value, "exit") == 0)
            {
                // Handle exit command
            	pipex(&tokens, envp, 0);
            	tokens = tokens->next;
            }
        }
        else if (((t_token *)tokens->content)->type == TOKEN_TEXT || ((t_token *)tokens->content)->type == TOKEN_LAST)
        {
        	printf("$ executing..\n");
        	pipex(&tokens, envp, &((t_token *)tokens->content)->type);
        }
        else if (((t_token *)tokens->content)->type == TOKEN_PIPE)
        {
        	tokens = tokens->next;
        	printf("$ executing..............\n");
        	pipex(&tokens, envp, &((t_token *)tokens->content)->type);
        }
        else if (((t_token *)tokens->content)->type == TOKEN_R_INPUT)
        {
            if (handle_file(((t_token *)tokens->next->content)->value, TOKEN_R_INPUT) == -1)
            	break;
        	tokens = tokens->next->next;
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
            limiter(((t_token *)tokens->next->content)->value);
        	tokens = tokens->next->next;
        }
    }
    // execute_last(&program, envp, tokens[i - 1]->value);
}