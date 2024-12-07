/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 17:21:38 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 17:21:39 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	find_dollar(char *arg)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$')
			return (i);
		i++;
	}
	return (-1);
}

static char	*get_env_value(char *var_name, char **envp)
{
	char	*var_value;
	int		i;

	var_value = NULL;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var_name, ft_strlen(var_name)) == 0
			&& envp[i][ft_strlen(var_name)] == '=')
		{
			var_value = ft_strdup(envp[i] + ft_strlen(var_name) + 1);
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
	if (ft_strncmp(&arg[i], "$?", 3) == 0 || ft_strncmp(&arg[i], "$", 2) == 0)
		return (1);
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
	ft_printf("arg: %s\n", arg);
	var_name = ft_strdup(arg + 1);
	ft_printf("var_name: %s\n", var_name);
	var_value = get_env_value(var_name, envp);
	ft_printf("var_value: %s\n", var_value);
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
		ft_printf("args[%d]: %s\n", i, args[i]);
		expanded_args[i] = expand_env_var(args[i], envp);
		i++;
	}
	expanded_args[i] = NULL;
	return (expanded_args);
}
