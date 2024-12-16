/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:50:17 by anuketay          #+#    #+#             */
/*   Updated: 2024/11/16 16:58:06 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = value;
	return (token);
}

int	add_token_to_list(t_list **node, int index, int type, char *value)
{
	*node = ft_lstnew(create_token(type, value));
	return (index);
}

int	check_token_symbols(char *line, t_list **new, int index)
{
	if (line[index] == '<' && line[index + 1] == '<')
		index = add_token_to_list(new, index + 2, TOKEN_HERE_DOC,
				ft_strdup("<<"));
	else if (line[index] == '>' && line[index + 1] == '>')
		index = add_token_to_list(new, index + 2, TOKEN_A_OUTPUT,
				ft_strdup(">>"));
	else if (line[index] == '<')
		index = add_token_to_list(new, index + 1, TOKEN_R_INPUT,
				ft_strdup("<"));
	else if (line[index] == '>')
		index = add_token_to_list(new, index + 1, TOKEN_R_OUTPUT,
				ft_strdup(">"));
	else if (line[index] == '|')
		index = add_token_to_list(new, index + 1, TOKEN_PIPE,
				ft_strdup("|"));
	return (index);
}

int	skip_word(char *line, int index)
{
	while (line[index] && !isspace(line[index]) && line[index] != '<'
		&& line[index] != '>')
	{
		if (line[index] == '\'')
		{
			index++;
			while (line[index] && line[index] != '\'')
				index++;
		}
		else if (line[index] == '\"')
		{
			index++;
			while (line[index] && line[index] != '\"')
				index++;
		}
		index++;
	}
	return (index);
}

t_list	*tokenize_input(char *line, int index, t_list *new, t_list *root)
{
	int		start;
	char	*token_value;

	while (line[index])
	{
		if (line[index] == '>' || line[index] == '<' || line[index] == '|')
			index = check_token_symbols(line, &new, index);
		else if (!isspace(line[index]))
		{
			start = index;
			index = skip_word(line, index);
			token_value = handle_quotes(ft_substr(line, start, index - start));
			index = add_token_to_list(&new, index, TOKEN_TEXT, token_value);
		}
		else if (line[index])
			index++;
		if (!root)
			root = new;
		else
			ft_lstadd_back(&root, new);
	}
	add_token_to_list(&new, 0, TOKEN_END, ft_strdup(" "));
	ft_lstadd_back(&root, new);
	root = finish_tokenizing(root);
	return (root);
}
