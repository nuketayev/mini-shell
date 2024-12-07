/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:24:46 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 13:24:46 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	env(char *envp[])
{
	int	index;

	index = 0;
	if (!envp)
	{
		perror("env");
		return;
	}
	while(envp[index])
	{
		ft_printf("%s\n", envp[index]);
		index++;
	}
	return ;
}