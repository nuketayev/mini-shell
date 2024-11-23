/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:26:00 by anuketay          #+#    #+#             */
/*   Updated: 2024/11/16 16:42:03 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	print_lst(tokens);
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		process_tokens(tokens, envp, data);
		ft_printf("exit? %i\n", data->exit_flag);
		exit(0);
	}
	set_handler_two(&sa);
	waitpid(id, NULL, 0);
	ft_lstclear(&tokens, free_token);
}

int	main(int _argc, char *_argv[], char *envp[])
{
	struct sigaction	sa;
	t_data				data;
	char				*line;

	data.envp = envp;
	data.exit_flag = 0;
	while (1)
	{
		set_handler_one(&sa);
		line = readline("$ ");
		if (line == NULL)
			break ;
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			process_input(line, sa, envp, &data);
		}
		free(line);
	}
	return (0);
}
