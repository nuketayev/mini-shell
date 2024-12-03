#include "../../inc/minishell.h"

static void	handler_one(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signum == SIGQUIT)
	{
		return ;
	}

}

static void	handler_two(int signum)
{
	if (signum == SIGINT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
	}
	else if (signum == SIGQUIT)
	{
		ft_putstr_fd("\n", STDOUT_FILENO);
		return ;
	}
}

static void	handler_three(int signum)
{
	if (signum == SIGINT)
	{
		exit(SIGINT);
	}
	else if (signum == SIGQUIT)
	{
		ft_putstr_fd("Core dumped\n", STDOUT_FILENO);
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
