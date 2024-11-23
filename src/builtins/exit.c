#include "../../inc/minishell.h"

//this does not work, exit_flag is still 0 in parent
//test this in bash because it looks like if it's piped it's
//not supposed to exit somehow
void	new_exit(char **args, char *envp[], t_data *data)
{
	ft_printf("I am exit function\n");
	data->exit_flag = 1;
	return ;
}
