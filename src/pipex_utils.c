/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 21:46:47 by gharazka          #+#    #+#             */
/*   Updated: 2024/11/16 16:10:22 by anuketay         ###   ########.fr       */
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

char	*get_command_path(char **envp, char *argv)
{
	int		i;
	char	**path;
	char	*real_path;

	i = 0;
	path = get_path(envp);
	while (path[i])
	{
		real_path = ft_strjoin(path[i], argv, 0);
		if (access(real_path, X_OK) == 0)
		{
			free_split(path);
			return (real_path);
		}
		free(real_path);
		i++;
	}
	free_split(path);
	return (NULL);
}
