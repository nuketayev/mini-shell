
#include "../../inc/minishell.h"

void	execve_fail(char *cmd_path, char **args, t_data *data)
{
    ft_printf("%s: command not found\n", args[0]);
    free_split(data->envp);
    ft_lstclear(&data->root_token, free_token);
    if (cmd_path)
        free(cmd_path);
    free_split(args);
    exit(127);
}

char	**ft_combine(t_list **command)
{
    char	**args;
    int		i;

    args = (char **)malloc(80);
    i = 0;
    while (((t_token *)(*command)->content)->type == TOKEN_TEXT
        || ((t_token *)(*command)->content)->type == TOKEN_LAST)
    {
        args[i] = ft_strdup(((t_token *)(*command)->content)->value);
        i++;
        *command = (*command)->next;
    }
    args[i] = NULL;
    return (args);
}

void	find_last_redirection(t_list **command)
{
    while ((((t_token *)(*command)->content)->type == TOKEN_R_OUTPUT
        || ((t_token *)(*command)->content)->type == TOKEN_A_OUTPUT)
        && (((t_token *)(*command)->next->next->content)->type == TOKEN_R_OUTPUT
        || ((t_token *)(*command)->next->next->content)->type
        == TOKEN_A_OUTPUT))
    {
        redirect_output(((t_token *)(*command)->next->content)->value,
            ((t_token *)(*command)->content)->type, 0);
        *command = (*command)->next->next;
    }

}

void    free_all(char *cmd_path, t_data *data, char **args)
{
    free_split(data->envp);
    ft_lstclear(&data->root_token, free_token);
    if (cmd_path)
        free(cmd_path);
    free_split(args);
    exit(0);
}

t_data    *prepare_exec_last(t_list **command, t_data *data, char **args)
{
    int fd;

    fd = -1;
    find_last_redirection(command);
    if (((t_token *)(*command)->content)->type == TOKEN_R_OUTPUT
        || ((t_token *)(*command)->content)->type == TOKEN_A_OUTPUT)
    {
        fd = redirect_output(((t_token *)(*command)->next->content)->value,
                ((t_token *)(*command)->content)->type, 1);
        *command = (*command)->next->next;
    }
    if (fd != -1)
        close(fd);
    data = execute_last(data->envp, args, data);
    return (data);
}

t_data    *save_id(t_data *data, int id, int *fd, char *cmd_path)
{
    if (data->ids == NULL)
        data->ids = ft_lstnew((void *)(intptr_t)id);
    else
        ft_lstadd_back(&data->ids, ft_lstnew((void *)(intptr_t)id));
    dup2(fd[0], STDIN_FILENO);
    close(fd[0]);
    close(fd[1]);
    free(cmd_path);
    return (data);
}