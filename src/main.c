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
void	handler(int signum)
{
	if (signum == SIGINT)
	{
		g_sigint_received = 1;
		ft_putstr_fd("\n", STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handler_two(int signum)
{
	exit(0);
}

void	handler_three(int signum)
{
	ft_putstr_fd("\n", STDOUT_FILENO);
}

void	free_split(char **split_line)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		free(split_line[i]);
		i++;
	}
	free(split_line);
}

int	ft_splitlen(char *arr[])
{
	int	len;

	if (!arr)
		return (0);
	len = 0;
	while (arr[len])
		len++;
	return (len);
}

void	free_token(void *lst)
{
	t_token	*token;

	token = (t_token *)lst;
	free(token->value);
	free(token);
}

int	main(int _argc, char *_argv[], char *envp[])
{
	struct	sigaction sa;
	char	*line;
	int		id;
	t_list	*tokens;


	while (1)
	{
		sa.sa_handler = handler;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		if (g_sigint_received == 0)
			line = readline("$ ");
		else
			g_sigint_received = 0;
		if (line == NULL)
			break ;
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			tokens = tokenize_input(line);
			id = fork();
			if (id == 0)
			{
				sa.sa_handler = handler_two;
				sigemptyset(&sa.sa_mask);
				sigaction(SIGINT, &sa, NULL);
				process_tokens(tokens, envp);
				exit(0);
			}
			sa.sa_handler = handler_three;
			sigemptyset(&sa.sa_mask);
			sigaction(SIGINT, &sa, NULL);
			waitpid(id, NULL, 0);
			ft_lstclear(&tokens, free_token);
		}
		free(line);
		if (g_sigint_received == 1)
			g_sigint_received = 0;
	}
	return (0);
}

/*				for (t_list *temp = tokens; temp != NULL; temp = temp->next)
				{
					ft_errprintf("token: %s has type: %i\n",
						((t_token *)temp->content)->value,
						((t_token *)temp->content)->type);
				}
				
				*/