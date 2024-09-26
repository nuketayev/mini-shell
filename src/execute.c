#include "../inc/minishell.h"

void	execute()
{
	int	id;

	id = fork();
	if (id == 0)
	{
		execve();
	}
}