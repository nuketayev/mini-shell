/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gharazka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 21:46:47 by gharazka          #+#    #+#             */
/*   Updated: 2024/07/31 21:46:47 by gharazka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_and_exit(t_program program, char *message)
{
	free_commands(&program);
	exit(EXIT_FAILURE);
}

char	**get_path(char *envp[])
{
	int		i;
	char	*pathline;
	char	**path;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
		{
			pathline = (ft_strnstr(envp[i], "PATH=", 5) + 5);
		}
		i++;
	}
	if (!pathline)
		return (NULL);
	path = ft_split(pathline, ':');
	if (!path)
		perror("malloc failed");
	i = 0;
	while (path[i])
	{
		path[i] = ft_strjoin(path[i], "/", 1);
		i++;
	}
	return (path);
}

void	free_commands(t_program *program)
{
	if (program->command != NULL)
		free_split(program->command);
	if (program->command_path)
		free(program->command_path);
}

int	get_command_path(char **envp, t_program *program, char *argv)
{
	int		j;
	char	**path;

	j = 0;
	free_commands(program);
	path = get_path(envp);
	program->command = ft_split(argv, ' ');
	if (!path || !program->command)
		return (-1);
	while (path[j])
	{
		program->command_path = ft_strjoin(path[j],
				program->command[0], 0);
		if (access(program->command_path, X_OK) == 0)
		{
			free_split(path);
			return (0);
		}
		j++;
		if (path[j])
			free(program->command_path);
	}
	free_split(path);
	return (-1);
}
