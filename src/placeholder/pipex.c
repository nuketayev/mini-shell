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
	while (((t_token *)tokens->content)->type != TOKEN_END)
	{
		if (((t_token *)tokens->content)->type == TOKEN_TEXT
			|| ((t_token *)tokens->content)->type == TOKEN_LAST)
		{
			data = process_exec(&tokens, &((t_token *)tokens->content)->type, data);
		}
		else if (((t_token *)tokens->content)->type == TOKEN_PIPE)
		{
			tokens = tokens->next;
			data = process_exec(&tokens, &((t_token *)tokens->content)->type, data);
		}
		else if (((t_token *)tokens->content)->type == TOKEN_R_INPUT)
		{
			if (redirect_input(((t_token *)tokens->next->content)->value) == -1)
				break ;
			tokens = tokens->next->next;
		}
		else if (((t_token *)tokens->content)->type == TOKEN_HERE_DOC)
		{
			here_doc(((t_token *)tokens->next->content)->value);
			tokens = tokens->next->next;
		}
	}
	while (data->ids)
	{
		ft_printf("waiting for %i\n", data->ids->content);
		waitpid(data->ids->content, NULL, 0);
		data->ids = data->ids->next;
	}
	ft_printf("done waiting\n");
}
