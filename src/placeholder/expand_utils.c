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
	int		i;
	int		offset;

	new_str = malloc(ft_strlen(var_name) * sizeof(char));
	if (!new_str)
		return (NULL);
	if (var_name[0] == '\"')
		i = 1;
	else
		i = 0;
	offset = i;
	while (var_name[i] && var_name[i] != '\"')
	{
		new_str[i - offset] = var_name[i];
		i++;
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
