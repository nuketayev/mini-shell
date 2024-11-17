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
	int		*status;
	char	*line;
	int		i;
	int		id;
	t_list	*tokens;

	signal(SIGINT, &handler);
	while (1)
	{
		i = 0;
		line = readline("$ ");
		if (line == NULL)
			break ;
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			tokens = tokenize_input(line);
			id = fork();
			if (id == 0)
			{
				process_tokens(tokens, envp);
				exit(0);
			}
			waitpid(id, NULL, 0);
			ft_lstclear(&tokens, free_token);
		}
		free(line);
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