#include "../../inc/minishell.h"

//challenges:
//when it's piped to another function it does not do anything anything
//has to be set in parent, I thought adding the pointer to data might work, but it only chnges in child
//workaround is to create a hidden file and putting the string of the env variables there and later parent reads from that file
//but I am not sure if it's correct approach
//also you can look into the allowed functions cause maybe they have the answer to the issue
void	unset(char **args, char *envp[])
{
	ft_printf("I am unset function\n");
	return ;
}