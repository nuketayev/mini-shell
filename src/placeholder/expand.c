/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 18:24:55 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 13:24:48 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*get_env_value(char *var_name, char **envp)
{
	char	*var_value;
	int		i;

	var_value = NULL;
	i = 0;
	if (ft_findchar(var_name, '\"') == -1)
		var_name = remove_quotes(var_name);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name + 1, ft_strlen(var_name + 1)) == 0)
		{
			var_value = ft_strdup(envp[i] + ft_strlen(var_name));
			break ;
		}
		i++;
	}
	return (var_value);
}

static int	is_expansion(char *arg)
{
	int	i;

	i = find_dollar(arg);
	if (i == -1)
		return (1);
	if (ft_strncmp(arg, "\"$\"", 3) == 0 || ft_strncmp(&arg[i], "$?", 3) == 0
		|| ft_strncmp(&arg[i], "$", 2) == 0)
	{
		return (1);
	}
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return (1);
	return (0);
}

static char	*expand_env_var(char *arg, char **envp)
{
	char	*expanded_arg;
	char	*var_name;
	char	*var_value;

	if (is_expansion(arg))
		return (ft_strdup(arg));
	var_name = ft_strdup(arg);
	var_value = get_env_value(var_name, envp);
	if (var_value)
		expanded_arg = ft_strdup(var_value);
	else
		expanded_arg = ft_strdup("");
	free(var_name);
	free(var_value);
	return (expanded_arg);
}

char	**expand_args(char **args, char **envp)
{
	char	**expanded_args;
	int		i;
	int		count;

	count = 0;
	while (args[count])
		count++;
	expanded_args = malloc((count + 1) * sizeof(char *));
	if (!expanded_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		expanded_args[i] = expand_env_var(args[i], envp);
		i++;
	}
	expanded_args[i] = NULL;
	return (expanded_args);
}
