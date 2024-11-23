#include "../../inc/minishell.h"

//this does not work, exit_flag is still 0 in parent
void	new_exit(char **args, char *envp[], t_data *data)
{
	ft_printf("I am exit function\n");
	data->exit_flag = 1;
	return ;
}