#include "../inc/minishell.h"

static void	handler_one(int signum)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	handler_two(int signum)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	set_handler_one(struct sigaction *sa)
{
	sa->sa_handler = handler_one;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sigaction(SIGINT, sa, NULL);
}

void	set_handler_two(struct sigaction *sa)
{
	sa->sa_handler = handler_two;
	sigemptyset(&sa->sa_mask);
	sigaction(SIGINT, sa, NULL);
}
