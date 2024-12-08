/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:24:55 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 13:20:34 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_existing_env(char *var, char ***envp, char *name,
		size_t name_len)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], name, name_len) == 0
			&& (*envp)[i][name_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(var);
			free(name);
			return ;
		}
		i++;
	}
}

static void	add_new_env(char *var, char ***envp, char *name)
{
	int		i;
	char	**new_envp;

	i = 0;
	while ((*envp)[i])
		i++;
	new_envp = malloc((i + 2) * sizeof(char *));
	i = 0;
	while ((*envp)[i])
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_envp[i] = ft_strdup(var);
	new_envp[i + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	free(name);
}

static void	add_or_update_env(char *var, char ***envp)
{
	char	*name;
	size_t	name_len;

	name_len = 0;
	while (var[name_len] && var[name_len] != '=')
		name_len++;
	name = ft_substr(var, 0, name_len);
	update_existing_env(var, envp, name, name_len);
	add_new_env(var, envp, name);
}

void	export(char **args, char ***envp)
{
	int	i;

	if (!args[1])
	{
		sort_envp(envp);
		print_sorted_envp(envp);
	}
	else
	{
		i = 1;
		while (args[i])
		{
			add_or_update_env(args[i], envp);
			i++;
		}
	}
}
