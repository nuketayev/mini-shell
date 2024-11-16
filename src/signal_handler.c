#include "../inc/minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;


void	handler(int signum)
{
	printf("this runs in handler_one\n");
	if (signum == SIGINT)
	{
		g_sigint_received = 1;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handler_two(int signum)
{
	printf("this runs in handler_two\n");
	exit(0);
}

void	set_handler_one()
{
	return;
}

void	set_handler_two()
{
	return;
}