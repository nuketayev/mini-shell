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

#include "../../inc/minishell.h"

void	free_split(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
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
int	is_command(char *cmd)
{
	if (ft_strncmp(cmd, "echo", 4) == 0
		|| ft_strncmp(cmd, "pwd", 3) == 0
		|| ft_strncmp(cmd, "cd", 2) == 0
		|| ft_strncmp(cmd, "export", 6) == 0
		|| ft_strncmp(cmd, "unset", 5) == 0
		|| ft_strncmp(cmd, "exit", 4) == 0
		|| ft_strncmp(cmd, "env", 3) == 0)
		return (1);
	return (0);
}