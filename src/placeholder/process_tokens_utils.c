/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_tokens_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/08 13:37:23 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 13:38:01 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_token_exec(t_list **tokens, t_data **data)
{
	*data = process_exec(tokens, &((t_token *)(*tokens)->content)->type, *data);
}

void	handle_token_pipe(t_list **tokens, t_data **data)
{
	*tokens = (*tokens)->next;
	*data = process_exec(tokens, &((t_token *)(*tokens)->content)->type, *data);
}

void	handle_token_input(t_list **tokens)
{
	if (redirect_input(((t_token *)(*tokens)->next->content)->value) == -1)
		return ;
	*tokens = (*tokens)->next->next;
}

void	handle_token_here_doc(t_list **tokens)
{
	here_doc(((t_token *)(*tokens)->next->content)->value);
	*tokens = (*tokens)->next->next;
}

void	handle_token_output(t_list **tokens)
{
	redirect_output(((t_token *)(*tokens)->next->content)->value,
		((t_token *)(*tokens)->content)->type, 0);
	*tokens = (*tokens)->next->next;
}
