/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gharazka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 21:23:45 by gharazka          #+#    #+#             */
/*   Updated: 2024/08/21 21:23:46 by gharazka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	wait_all(t_data *data)
{
	t_list	*to_free;

	while (data->ids)
	{
		waitpid((__pid_t)(intptr_t)data->ids->content, &data->exit_flag, 0);
		to_free = data->ids;
		data->ids = data->ids->next;
		free(to_free);
	}
}

t_data	*process_tokens(t_list *tokens, t_data *data)
{
	while (((t_token *)tokens->content)->type != TOKEN_END)
	{
		if (((t_token *)tokens->content)->type == TOKEN_TEXT
			|| ((t_token *)tokens->content)->type == TOKEN_LAST)
			handle_token_exec(&tokens, &data);
		else if (((t_token *)tokens->content)->type == TOKEN_PIPE)
			handle_token_pipe(&tokens, &data);
		else if (((t_token *)tokens->content)->type == TOKEN_R_INPUT
			&& handle_token_input(&tokens, &data) == -1)
			return (data);
		else if (((t_token *)tokens->content)->type == TOKEN_HERE_DOC)
			handle_token_here_doc(&tokens);
		else if (((t_token *)tokens->content)->type == TOKEN_R_OUTPUT
			|| ((t_token *)tokens->content)->type == TOKEN_A_OUTPUT)
			handle_token_output(&tokens);
	}
	wait_all(data);
	return (data);
}

static int	check_first_token(t_token *first_token, t_token *second_token)
{
	if (first_token->type == TOKEN_PIPE)
	{
		ft_errprintf("minishell: syntax error near unexpected token `%s`\n",
			first_token->value);
		return (0);
	}
	if (first_token->type == TOKEN_R_INPUT && second_token->type != TOKEN_TEXT)
	{
		ft_errprintf("minishell: %s: No such file or directory\n",
			second_token->value);
		return (0);
	}
	return (1);
}

int	validate_tokens(t_list *tokens)
{
	t_list	*current;
	t_token	*last_token;
	t_token	*first_token;
	t_token	*second_token;

	if (!tokens)
		return (0);
	last_token = NULL;
	first_token = (t_token *)tokens->content;
	if ((t_token *)tokens->next)
		second_token = (t_token *)tokens->next->content;
	else
		second_token = NULL;
	if (!check_first_token(first_token, second_token))
		return (0);
	current = tokens;
	if (check_last(current, last_token) == 0)
		return (0);
	return (1);
}
