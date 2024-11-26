#include "../../inc/minishell.h"

//this does not work, exit_flag is still 0 in parent
//test this in bash because it looks like if it's piped it's
//not supposed to exit somehow
void	ft_exit(void)
{
    printf("exit\n");
    exit(0);
}
