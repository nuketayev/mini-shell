/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:26:41 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 13:29:38 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handler_one(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_sigint_received = 130;
	}
	else if (signum == SIGQUIT)
	{
		g_sigint_received = 131;
	}
}

void	handler_two(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		g_sigint_received = 130;
	}
	else if (signum == SIGQUIT)
	{
		g_sigint_received = 131;
		return ;
	}
}

void	handler_three(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_received = 130;
		exit(SIGINT);
	}
	if (signum == SIGQUIT)
	{
		g_sigint_received = 131;
		ft_putstr_fd("Quit (Core dumped)\n", STDOUT_FILENO);
		exit(SIGQUIT);
	}
}
