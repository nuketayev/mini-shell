/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:24:22 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 13:24:23 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	handler_one(int signum)
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

static void	handler_two(int signum)
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

static void	handler_three(int signum)
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

void	set_handler_one(struct sigaction *sa)
{
	sa->sa_handler = handler_one;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

void	set_handler_two(struct sigaction *sa)
{
	sa->sa_handler = handler_two;
	sigemptyset(&sa->sa_mask);
	sigaction(SIGINT, sa, NULL);
	sigaction(SIGQUIT, sa, NULL);
}

void	set_handler_three(struct sigaction *sa)
{
	sa->sa_handler = handler_three;
	sigemptyset(&sa->sa_mask);
	sigaction(SIGINT, sa, NULL);
	sigaction(SIGQUIT, sa, NULL);
}
