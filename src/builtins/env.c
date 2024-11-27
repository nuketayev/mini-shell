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