/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:17:06 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 13:20:33 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	sort_envp(char ***envp)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while ((*envp)[i])
	{
		j = i + 1;
		while ((*envp)[j])
		{
			if (ft_strncmp((*envp)[i], (*envp)[j], ft_strlen((*envp)[i])) > 0)
			{
				temp = (*envp)[i];
				(*envp)[i] = (*envp)[j];
				(*envp)[j] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_envp(char ***envp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		ft_printf("declare -x %s\n", (*envp)[i]);
		i++;
	}
}
