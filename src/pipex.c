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
		if (execve(program->command_path, program->command, envp) == -1)
			free_and_exit(*program, "");
}

void	execute(t_program *program, char **envp, char *argv)
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
}

void	pipex(char *arg, char *envp[])
{
	t_program	program;
	int			i;

	i = 2;
	program = init_struct(1, arg);
	execute(&program, envp, arg);
}
