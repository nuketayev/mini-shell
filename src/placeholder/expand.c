/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 18:24:55 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/20 18:44:32 by anuketay         ###   ########.fr       */
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
		if (ft_strncmp(envp[i], var_name + 1, ft_strlen_until(envp[i],
					'=')) == 0)
		{
			var_value = ft_strjoin(ft_strdup(envp[i] + ft_strlen_until(envp[i],
							'=') + 1), var_name + ft_strlen_until(envp[i], '=')
					+ 1, 1);
			break ;
		}
		i++;
	}
	return (var_value);
}

static int	is_expansion(char *arg, int *single_quote)
{
	int	i;

	i = find_dollar(arg);
	if (arg[0] == '\'' || arg[ft_strlen(arg) - 1] == '\'')
	{
		*single_quote += count_chars(arg, '\'');
		return (1);
	}
	if (i == -1)
		return (1);
	if (ft_strncmp(arg, "\"$\"", 3) == 0 || ft_strncmp(&arg[i], "$", 2) == 0
		|| ft_strncmp(&arg[i], "$?", 3) == 0)
		return (1);
	return (0);
}

static char	*expand_env_var(char *arg, char **envp, int *single_quote)
{
	char	*expanded_arg;
	char	*var_name;
	char	*var_value;

	if (is_expansion(arg, single_quote) == 1)
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

char	**check_for_quotes(char **args, char **expanded_args, int single_quote,
		char **envp)
{
	int	i;

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

char	**expand_args(char **args, char **envp)
{
	char	**expanded_args;
	int		count;
	int		single_quote;

	count = 0;
	single_quote = 0;
	while (args[count])
		count++;
	expanded_args = malloc((count + 1) * sizeof(char *));
	if (!expanded_args)
		return (NULL);
	check_for_quotes(args, expanded_args, single_quote, envp);
	free_split(args);
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