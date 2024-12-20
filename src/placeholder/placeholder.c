#include "../../inc/minishell.h"

static int	check_last_token(t_token *last_token)
{
    if (last_token && (last_token->type == TOKEN_PIPE
            || last_token->type == TOKEN_R_OUTPUT
            || last_token->type == TOKEN_A_OUTPUT
            || last_token->type == TOKEN_R_INPUT))
    {
        ft_errprintf("minishell: syntax error near unexpected token `%s`\n",
            last_token->value);
        return (0);
    }
    return (1);
}


int	check_last(t_list *current, t_token *last_token)
{
    t_token	*token;

    while (current)
    {
        token = (t_token *)current->content;
        if (token->type == TOKEN_END)
        {
            if (!check_last_token(last_token))
                return (0);
            break ;
        }
        last_token = token;
        current = current->next;
    }
    return (1);
}

void	free_all(char *cmd_path, t_data *data, char **args)
{
    free_split(data->envp);
    ft_lstclear(&data->root_token, free_token);
    if (cmd_path)
        free(cmd_path);
    free_split(args);
    exit(0);
}