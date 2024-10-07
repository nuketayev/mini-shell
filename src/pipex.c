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

	if (get_command_path(envp, program, argv) == -1)
	{
		ft_errprintf("/bin/sh: 1: %s: not found\n", argv);
	}
	id = fork();
	if (id == -1)
		free_and_exit(*program, "fork failed");
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

void	pipex(int argc, char *argv[], char *envp[])
{
	t_program	program;
	int			i;

	i = 0;
	program = init_struct(1, argv[0]);
	while (i < argc - 1)
	{
		execute(&program, envp, argv[i]);
		i++;
	}
	execute_last(&program, envp, argv[argc - 1]);
	exit(0);
}
