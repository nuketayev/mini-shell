/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_last_token.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:23:58 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/07 13:23:59 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int is_delimiter(int type)
{
	if (type == TOKEN_END || type == TOKEN_R_OUTPUT || type == TOKEN_A_OUTPUT
		|| type == TOKEN_HERE_DOC || type == TOKEN_R_INPUT)
		return (1);
	return (0);
}

static t_list	*find_last(t_list **current, t_list *first)
{
	t_list	*last_cmd;

	last_cmd = NULL;
	while (*current)
	{
		if (((t_token *)(*current)->content)->type == TOKEN_TEXT)
			last_cmd = *current;
		while (((t_token *)(*current)->content)->type == TOKEN_TEXT)
			*current = (*current)->next;
		if (last_cmd && is_delimiter(((t_token *)(*current)->content)->type))
		{
			((t_token *)last_cmd->content)->type = TOKEN_LAST;
			return (first);
		}
		*current = (*current)->next;
	}
	return (first);
}

t_list	*finish_tokenizing(t_list *first)
{
	t_list	*current;

	current = first;
	while (current
		&& (((t_token *)current->content)->type == TOKEN_R_INPUT
		|| ((t_token *)current->content)->type == TOKEN_HERE_DOC))
	{
		current = current->next;
		if (current)
			current = current->next;
	}
	while (current)
		find_last(&current, first);
	return (first);
}

void	free_token(void *lst)
{
	t_token	*token;

	token = (t_token *)lst;
	free(token->value);
	free(token);
}
