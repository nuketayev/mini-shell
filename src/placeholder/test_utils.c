/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:24:29 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 13:24:29 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		ft_printf("%s\n", arr[i]);
		i++;
	}
}

void	print_lst(t_list *lst)
{
	while (lst)
	{
		ft_printf("value: %s, type: %i\n", ((t_token *)lst->content)->value,
			((t_token *)lst->content)->type);
		lst = lst->next;
	}
}
