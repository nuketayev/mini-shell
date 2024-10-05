#include "../inc/minishell.h"

void	execute_not()
{
	int	id;

	id = fork();
	if (id == 0)
	{
		execve();
	}
}