/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:24:55 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 17:34:38 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	add_or_update_env(char *var, char ***envp)
{
	int		i;
	char	*name;
	size_t	name_len;
	char	**new_envp;

	name_len = 0;
	while (var[name_len] && var[name_len] != '=')
		name_len++;
	name = ft_substr(var, 0, name_len);
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

void	export(char **args, char ***envp)
{
	int		i;
	int		j;
	char	*temp;

	if (!args[1])
	{
		i = 0;
		while ((*envp)[i])
		{
			j = i + 1;
			while ((*envp)[j])
			{
				if (ft_strncmp((*envp)[i], (*envp)[j],
						ft_strlen((*envp)[i])) > 0)
				{
					temp = (*envp)[i];
					(*envp)[i] = (*envp)[j];
					(*envp)[j] = temp;
				}
				j++;
			}
			i++;
		}
		i = 0;
		while ((*envp)[i])
		{
			ft_printf("declare -x %s\n", (*envp)[i]);
			i++;
		}
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
