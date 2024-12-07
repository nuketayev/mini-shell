/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:25:11 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 17:27:20 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_env_var(char *var, char **envp)
{
	int		i;
	int		j;
	size_t	var_len;

	var_len = ft_strlen(var);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
		{
			free(envp[i]);
			j = i;
			while (envp[j])
			{
				envp[j] = envp[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

void	unset(char **args, char *envp[])
{
	int	i;

	if (!args[1])
	{
		return ;
	}
	i = 1;
	while (args[i])
	{
		remove_env_var(args[i], envp);
		i++;
	}
}
