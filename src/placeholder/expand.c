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

int ft_strlen_until(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

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
		if (ft_strncmp(envp[i], var_name + 1, ft_strlen_until(envp[i], '=')) == 0)
		{
			var_value = ft_strdup(envp[i] + ft_strlen(var_name));
			break ;
		}
		i++;
	}
	return (var_value);
}

int	count_chars(char *arg, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (arg[i])
	{
		if (arg[i] == c)
			count++;
		i++;
	}
	return (count);
}

static int	is_expansion(char *arg, int *single_quote)
{
	int	i;

	i = find_dollar(arg);
	if (ft_strncmp(arg, "\"$\"", 3) == 0 || ft_strncmp(&arg[i], "$?", 3) == 0
		|| ft_strncmp(&arg[i], "$", 2) == 0)
	{
		return (1);
	}
	if (arg[0] == '\'' || arg[ft_strlen(arg) - 1] == '\'')
	{
		*single_quote += count_chars(arg, '\'');
		return (1);
	}
	if (i == -1)
		return (1);
	return (0);
}

static char	*expand_env_var(char *arg, char **envp, int *single_quote)
{
	char	*expanded_arg;
	char	*var_name;
	char	*var_value;

	if (is_expansion(arg, single_quote))
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
	int		single_quote;

	count = 0;
	single_quote = 0;
	while (args[count])
		count++;
	expanded_args = malloc((count + 1) * sizeof(char *));
	if (!expanded_args)
		return (NULL);
	i = 0;
	while (args[i])
	{
		if (args[i] && single_quote % 2 == 1 && single_quote)
		{
			single_quote += count_chars(args[i], '\'');
			expanded_args[i] = ft_strdup(args[i]);
			i++;
		}
		else
		{
			expanded_args[i] = expand_env_var(args[i], envp, &single_quote);
			i++;
		}
	}
	expanded_args[i] = NULL;
	return (expanded_args);
}

// input: $USER
// output: gharazka
// input: '$USER'
// output: $USER
// input: ' $USER '
// output: $USER
// input: ' $USER'
// output: $USER
// input: '$USER '
// output: $USER
// input "$USER"
// output: gharazka
// input "$USER "
// output: gharazka
// input " $USER"
// output: gharazka
// input " $USER "
// output: gharazka
// input $USER '$USER' "$USER"
// output gharazka $USER gharazka
// input '$USER' ' $USER' ' $USER ' '$USER '
// output $USER $USER $USER  $USER


// bug fixed:
// finds value without full path -> if variable = PATH, finds it with $PAT