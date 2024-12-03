#include "../../inc/minishell.h"

void	print_exit_int()
{
	ft_printf("%i\n", g_sigint_received);
}