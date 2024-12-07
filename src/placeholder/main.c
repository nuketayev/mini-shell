/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:26:00 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 13:25:51 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t g_sigint_received = 0;

static void	free_token(void *lst)
{
	t_token	*token;

	token = (t_token *)lst;
	free(token->value);
	free(token);
}

static void	process_input(char *line, struct sigaction sa, char *envp[],
		t_data *data)
{
	int		id;
	t_list	*tokens;

	tokens = tokenize_input(line, 0, NULL, NULL);
    if (is_command(((t_token *)tokens->content)->value))
    {
        signal(SIGINT, SIG_DFL);
        process_tokens(tokens, envp, data);
    }
    else
    {
        id = fork();
        if (id == 0)
        {
            set_handler_three(&sa);
            process_tokens(tokens, envp, data);
            exit(0);
        }
    }
    set_handler_two(&sa);
    waitpid(id, NULL, 0);
    ft_lstclear(&tokens, free_token);
}

static char **copy_envp(char **envp)
{
    int     i;
    char    **new_envp;

	i = 0;
	while (envp[i])
		i++;
    new_envp = malloc((i + 1) * sizeof(char *));
    if (!new_envp)
        return (NULL);
    for (i = 0; envp[i]; i++)
        new_envp[i] = ft_strdup(envp[i]);
    new_envp[i] = NULL;
    return (new_envp);
}

int main(int _argc, char *_argv[], char *envp[])
{
    struct sigaction    sa;
    t_data              data;
    char                *line;

    data.envp = copy_envp(envp);
    data.exit_flag = 0;
	data.fd = -1;
	data.ids = NULL;
    while (1)
    {
        set_handler_one(&sa);
        line = readline("$ ");
        if (line == NULL)
            break;
        if (ft_strlen(line) > 0)
        {
            add_history(line);
            process_input(line, sa, data.envp, &data);
        }
        free(line);
    }
    free_split(data.envp);
    return (0);
}
