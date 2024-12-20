/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:21:33 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 13:22:30 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	find_dollar(char *arg)
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

char	*remove_quotes(char *var_name)
{
	char	*new_str;
	size_t		i;
	size_t	offset;

	new_str = malloc(ft_strlen(var_name) - count_chars(var_name, '\'') - count_chars(var_name, '\"') + 2 * sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	offset = i;
	while (var_name[i])
	{
		if (var_name[i] != '\'' && var_name[i] != '\"')
		{
			new_str[i - offset] = var_name[i];
			i++;
		}
		else
		{
			offset++;
			i++;
		}
	}
	new_str[i - offset] = '\0';
	return (new_str);
}

int	ft_strlen_until(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
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
