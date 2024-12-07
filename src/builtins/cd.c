/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:24:37 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 17:37:12 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	update_envp_pwd(char *new_pwd, char **envp)
{
	int		index;
	char	*new_pwd_entry;

	index = 0;
	while (envp[index])
	{
		if (ft_strncmp(envp[index], "PWD=", 4) == 0)
		{
			new_pwd_entry = malloc(4 + ft_strlen(new_pwd) + 1);
			if (!new_pwd_entry)
				return ;
			ft_strlcpy(new_pwd_entry, "PWD=", ft_strlen("PWD=") + 1);
			ft_strlcat(new_pwd_entry, new_pwd, ft_strlen("PWD=")
				+ ft_strlen(new_pwd) + 1);
			free(envp[index]);
			envp[index] = new_pwd_entry;
			break ;
		}
		index++;
	}
}

void	cd(char **args, t_data *data)
{
	char	*path;
	char	cwd[PATH_MAX];

	if (!args[1] || ft_strncmp(args[1], "~", 1) == 0)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_printf("cd: HOME not set\n");
			return ;
		}
	}
	else
	{
		path = args[1];
	}
	if (chdir(path) != 0)
	{
		perror("cd");
		return ;
	}
	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		update_envp_pwd(cwd, data->envp);
	}
	else
	{
		perror("getcwd");
	}
}
