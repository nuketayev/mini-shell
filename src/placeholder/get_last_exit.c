/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_last_exit.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 18:49:37 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/20 18:54:32 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// if it finds $? replaces it with the last exit code
t_list	*get_last_exit(t_list *tokens, t_list *root, t_data *data)
{
	t_token	*token;

	while (tokens)
	{
		token = (t_token *)tokens->content;
		if (ft_strncmp(token->value, "$?", 2) == 0)
		{
			free(token->value);
			if (g_sigint_received)
				token->value = ft_itoa(g_sigint_received);
			else if (data->exit_flag < 3)
				token->value = ft_itoa(data->exit_flag);
			else
				token->value = ft_itoa(WEXITSTATUS(data->exit_flag));
		}
		tokens = tokens->next;
	}
	data->exit_flag = 0;
	return (root);
}
