#include "../inc/minishell.h"

static int	count_char_until(char *line, char c, char until)
{
	int	i;
	int	count;

	if (!line)
		return (0);
	i = 0;
	count = 0;
	while (line[i] && line[i] != until)
	{
		if (line[i] == c)
			count++;
		i++;
	}
	return (count);
}

static char	*remove_char_until(char *line, char c)
{
	char	*f_line;
	int		count;
	int		i;
	int		j;

	if (!line)
		return (NULL);
	count = 2;
	f_line = (char *)malloc(sizeof(char) * (ft_strlen(line) - 1));
	i = 0;
	j = 0;
	while (line[i] && count > 0)
	{
		if (line[i] != c)
		{
			f_line[j] = line[i];
			j++;
			i++;
		}
		else
		{
			i++;
			count--;
		}
	}
	f_line[j] = 0;
	return (f_line);
}

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

t_list	*finish_tokenizing(t_list *first)
{
	t_list	*current;
	t_list	*last_cmd;

	current = first;
	last_cmd = NULL;
	while (((t_token *)current->content)->type == TOKEN_R_INPUT
		|| ((t_token *)current->content)->type == TOKEN_HERE_DOC)
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
		if (last_cmd && (((t_token *)current->content)->type == TOKEN_END
				|| ((t_token *)current->content)->type == TOKEN_R_OUTPUT
				|| ((t_token *)current->content)->type == TOKEN_A_OUTPUT))
		{
			((t_token *)last_cmd->content)->type = TOKEN_LAST;
			return (first);
		}
		current = current->next;
	}
	return (first);
}

char	*handle_quotes(char *line)
{
	size_t	i;
	char	*new_line;
	char	*temp;

	new_line = NULL;
	i = 0;
	while (line[i])
	{
		temp = NULL;
		if (line[i] == '\"' || line[i] == '\'')
		{
			temp = remove_char_until(&line[i], line[i]);
			i += 2;
		}
		else
			temp = ft_substr(line, i, 1);
		new_line = ft_strjoin(new_line, temp, 1);
		i += ft_strlen(temp);
		free(temp);
	}
	free(line);
	return (new_line);
}

int	find_end(char *line, int i, char c)
{
	while (line[i] && line[i] != c)
		i++;
	return (i);
}

t_list	*tokenize_input(char *line)
{
	t_list *root;
	t_list *new;
	int index;

	index = 0;
	root = NULL;
	new = NULL;
	while (line[index])
	{
		if (line[index] == '<' && line[index + 1] == '<')
			index = add_token_to_list(&new, index + 2, TOKEN_HERE_DOC,
					ft_strdup("<<"));
		else if (line[index] == '>' && line[index + 1] == '>')
			index = add_token_to_list(&new, index + 2, TOKEN_A_OUTPUT,
					ft_strdup(">>"));
		else if (line[index] == '<')
			index = add_token_to_list(&new, index + 1, TOKEN_R_INPUT,
					ft_strdup("<"));
		else if (line[index] == '>')
			index = add_token_to_list(&new, index + 1, TOKEN_R_OUTPUT,
					ft_strdup(">"));
		else if (line[index] == '|')
			index = add_token_to_list(&new, index + 1, TOKEN_PIPE,
					ft_strdup("|"));
		else if (!isspace(line[index]))
		{
			int start;
			char *token_value;

			start = index;
			while (line[index] && !isspace(line[index]) && line[index] != '<'
				&& line[index] != '>')
			{
				if (line[index] == '\'')
					index = find_end(line, index + 1, '\'');
				if (line[index] == '\"')
					index = find_end(line, index + 1, '\"');
				index++;
			}
			token_value = handle_quotes(ft_substr(line, start, index - start));
			if (ft_strncmp(token_value, "echoa", 5) == 0 || // echoa?
				ft_strncmp(token_value, "cd", 2) == 0 || ft_strncmp(token_value,
						"pwd", 3) == 0 || ft_strncmp(token_value, "export",
						6) == 0 || ft_strncmp(token_value, "unset", 5) == 0
					|| ft_strncmp(token_value, "env", 3) == 0
					|| ft_strncmp(token_value, "exit", 4) == 0)
				index = add_token_to_list(&new, index, TOKEN_COMMAND,
						token_value);
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
	add_token_to_list(&new, 0, TOKEN_END, ft_strdup(" "));
	ft_lstadd_back(&root, new);
	root = finish_tokenizing(root);
	return (root);
}