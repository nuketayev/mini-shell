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

void	process_tokens(t_list *tokens, char *envp[], t_data *data)
{
	t_list	*to_free;

	while (((t_token *)tokens->content)->type != TOKEN_END)
	{
		if (((t_token *)tokens->content)->type == TOKEN_TEXT
			|| ((t_token *)tokens->content)->type == TOKEN_LAST)
			handle_token_exec(&tokens, &data);
		else if (((t_token *)tokens->content)->type == TOKEN_PIPE)
			handle_token_pipe(&tokens, &data);
		else if (((t_token *)tokens->content)->type == TOKEN_R_INPUT)
			handle_token_input(&tokens);
		else if (((t_token *)tokens->content)->type == TOKEN_HERE_DOC)
			handle_token_here_doc(&tokens);
		else if (((t_token *)tokens->content)->type == TOKEN_R_OUTPUT
			|| ((t_token *)tokens->content)->type == TOKEN_A_OUTPUT)
			handle_token_output(&tokens);
	}
	while (data->ids)
	{
		waitpid((__pid_t)(intptr_t)data->ids->content, NULL, 0);
		to_free = data->ids;
		data->ids = data->ids->next;
		free(to_free);
	}
	g_sigint_received = 0;
}

static int	check_first_token(t_token *first_token, t_token *second_token)
{
	if (first_token->type == TOKEN_PIPE)
	{
		ft_errprintf("minishell: syntax error near unexpected token `%s`\n",
			first_token->value);
		return (0);
	}
	else if (first_token->type == TOKEN_R_INPUT)
	{
		ft_errprintf("minishell: %s: No such file or directory\n",
			second_token->value);
		return (0);
	}
	return (1);
}

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

int	validate_tokens(t_list *tokens)
{
	t_list	*current;
	t_token	*last_token;
	t_token	*first_token;
	t_token	*second_token;
	t_token	*token;

	last_token = NULL;
	if (!tokens)
		return (0);
	
	first_token = (t_token *)tokens->content;
	second_token = (t_token *)tokens->next->content;
	if (!check_first_token(first_token, second_token))
		return (0);
	current = tokens;
	while (current)
	{
		//printf("Current token: %s, type: %d\n", ((t_token *)current->content)->value, ((t_token *)current->content)->type);
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
