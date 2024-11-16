#include "../inc/minishell.h"

static int	count_slash_semicolon(char *line)
{
	int	i;
	int	count;

	if (!line)
		return (0);
	i = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == '\\' || line[i] == ';')
			count++;
		i++;
	}
	return (count);
}

static char	*remove_slash_semicolon(char *line)
{
	char	*f_line;
	int		count;
	int		i;
	int		j;

	if (!line)
		return (NULL);
	count = count_slash_semicolon(line);
	f_line = (char *)malloc(sizeof(char) * (ft_strlen(line) - count + 1));
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] != '\\' && line[i] != ';')
		{
			f_line[j] = line[i];
			j++;
		}
		i++;
	}
	f_line[j] = 0;
	free(line);
	return (f_line);
}

char	**ft_split_str(char *line)
{
	char	**split_line;
	int	i;

	i = 0;
	line = remove_slash_semicolon(line);
	split_line = ft_split(line, ' ');
	while (split_line[i])
	{
		if (split_line[i][0] == '\'')
			split_line[i] = ft_strtrim(split_line[i], "\'");
		else
			split_line[i] = ft_strtrim(split_line[i], "\"");
		i++;
	}
	return (split_line);
}

///

t_token *create_token(t_token_type type, char *value)
{
	t_token *token = malloc(sizeof(t_token));
	if (!token)
		return NULL;
	token->type = type;
	token->value = value;
	return (token);
}

int	add_token_to_list(t_list **node, int index, int type, char *value)
{
	*node = ft_lstnew(create_token(type, value));
	return index;
}

t_list *finish_tokenizing(t_list *first)
{
	t_list *current;
	t_list	*last_cmd;
	current = first;
	last_cmd = NULL;
	if (((t_token *)current->content)->type == TOKEN_R_INPUT ||
		((t_token *)current->content)->type == TOKEN_HERE_DOC)
	{
		current = current->next;
		if (current)
			current = current->next;
	}
	while (current)
	{
		if (((t_token *)current->content)->type == TOKEN_TEXT)
			last_cmd = current;
		while (((t_token *)current->content)->type == TOKEN_TEXT)
			current = current->next;
		if (last_cmd &&
			(((t_token *)current->content)->type == TOKEN_END ||
			((t_token *)current->content)->type == TOKEN_R_OUTPUT ||
			((t_token *)current->content)->type == TOKEN_A_OUTPUT))
		{
			((t_token *)last_cmd->content)->type = TOKEN_LAST;
			return (first);
		}
		current = current->next;
	}
	return (first);
}

t_list	*tokenize_input(char *line)
{
	t_list	*root;
	t_list	*new;
	int index;

	index = 0;
	root = NULL;
	new = NULL;
	while (line[index])
	{
		if (line[index] == '<' && line[index + 1] == '<')
			index = add_token_to_list(&new, index + 2, TOKEN_HERE_DOC, ft_strdup("<<"));
		else if (line[index] == '>' && line[index + 1] == '>')
			index = add_token_to_list(&new, index + 2, TOKEN_A_OUTPUT, ft_strdup(">>"));
		else if (line[index] == '<')
			index = add_token_to_list(&new, index + 1, TOKEN_R_INPUT, ft_strdup("<"));
		else if (line[index] == '>')
			index = add_token_to_list(&new, index + 1, TOKEN_R_OUTPUT, ft_strdup(">"));
		else if (line[index] == '|')
			index = add_token_to_list(&new, index + 1, TOKEN_PIPE, ft_strdup("|"));
		else if (!isspace(line[index]))
		{
			int start;
			char *token_value;

			start = index;
			while (line[index] && !isspace(line[index]) && line[index] != '<' && line[index] != '>')
				index++;
			token_value = ft_substr(line, start, index - start);
			if (strcmp(token_value, "echoa") == 0 || strcmp(token_value, "cd") == 0 ||
				strcmp(token_value, "pwd") == 0 || strcmp(token_value, "export") == 0 ||
				strcmp(token_value, "unset") == 0 || strcmp(token_value, "env") == 0 ||
				strcmp(token_value, "exit") == 0)
				index = add_token_to_list(&new, index, TOKEN_COMMAND, token_value);
			else
				index = add_token_to_list(&new, index, TOKEN_TEXT, token_value);
		}
		else if (line[index])
			index++;
		if (!root)
			root = new;
		else
			ft_lstadd_back(&root, new);
	}
	add_token_to_list(&new, 0, TOKEN_END, "");
	ft_lstadd_back(&root, new);
	root = finish_tokenizing(root);
	return (root);
}