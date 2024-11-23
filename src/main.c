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

#include "../inc/minishell.h"

static void	free_token(void *lst)
{
	t_token	*token;

	token = (t_token *)lst;
	free(token->value);
	free(token);
}

static void	process_input(char *line, struct sigaction sa, char *envp[])
{
	int		id;
	t_list	*tokens;

	tokens = tokenize_input(line, 0, NULL, NULL);
	id = fork();
	if (id == 0)
	{
		signal(SIGINT, SIG_DFL);
		process_tokens(tokens, envp);
		exit(0);
	}
	set_handler_two(&sa);
	waitpid(id, NULL, 0);
	ft_lstclear(&tokens, free_token);
}

int	main(int _argc, char *_argv[], char *envp[])
{
	struct sigaction	sa;
	char				*line;

	while (1)
	{
		set_handler_one(&sa);
		line = readline("$ ");
		if (line == NULL)
			break ;
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			process_input(line, sa, envp);
		}
		free(line);
	}
	return (0);
}
