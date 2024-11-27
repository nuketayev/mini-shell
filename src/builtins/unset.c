#include "../../inc/minishell.h"

//challenges:
//when it's piped to another function it does not do anything anything
//has to be set in parent, I thought adding the pointer to data might work,
//but it only chnges in child
//workaround is to create a hidden file and putting the string
//of the env variables there and later parent reads from that file
//but I am not sure if it's correct approach
//also you can look into the allowed functions cause maybe
//they have the answer to the issue
static void	remove_env_var(char *var, char **envp)
{
    int		i;
    int		j;
    size_t	var_len;

    var_len = ft_strlen(var);
    i = 0;
    while (envp[i])
    {
        if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
        {
            free(envp[i]);
            j = i;
            while (envp[j])
            {
                envp[j] = envp[j + 1];
                j++;
            }
            return;
        }
        i++;
    }
}

void	unset(char **args, char *envp[])
{
    int	i;

    if (!args[1])
    {
        ft_printf("unset: no argument\n"); // in normal bash its not showing anything. So delete?
        return;
    }
    i = 1;
    while (args[i])
    {
        remove_env_var(args[i], envp);
        i++;
    }
}
