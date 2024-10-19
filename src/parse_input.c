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

t_token **tokenize_input(char *line)
{
	t_token **tokens = malloc(sizeof(t_token *) * (ft_strlen(line) + 1));
	int index;
	int token_index;

	index = 0;
	token_index = 0;
	while (line[index])
	{
		if (line[index] == '<' && line[index + 1] == '<')
		{
			tokens[token_index] = create_token(TOKEN_HERE_DOC, ft_strdup("<<"));
			index += 2;
		}
		else if (line[index] == '>' && line[index + 1] == '>')
		{
			tokens[token_index] = create_token(TOKEN_A_OUTPUT, ft_strdup(">>"));
			index += 2;
		}
		else if (line[index] == '<')
		{
			tokens[token_index] = create_token(TOKEN_R_INPUT, ft_strdup("<"));
			index++;
		}
		else if (line[index] == '>')
		{
			tokens[token_index] = create_token(TOKEN_R_OUTPUT, ft_strdup(">"));
			index++;
		}
		else if (!isspace(line[index]))
		{
			int start;
			char *token_value;

			start = index;
			while (line[index] && !isspace(line[index]) && line[index] != '<' && line[index] != '>')
				index++;
			token_value = ft_substr(line, start, index - start);
			if (strcmp(token_value, "echo") == 0)
				tokens[token_index++] = create_token(TOKEN_COMMAND, token_value);
			else if (strcmp(token_value, "cd") == 0)
				tokens[token_index++] = create_token(TOKEN_COMMAND, token_value);
			else if (strcmp(token_value, "pwd") == 0)
				tokens[token_index++] = create_token(TOKEN_COMMAND, token_value);
			else if (strcmp(token_value, "export") == 0)
				tokens[token_index++] = create_token(TOKEN_COMMAND, token_value);
			else if (strcmp(token_value, "unset") == 0)
				tokens[token_index++] = create_token(TOKEN_COMMAND, token_value);
			else if (strcmp(token_value, "env") == 0)
				tokens[token_index++] = create_token(TOKEN_COMMAND, token_value);
			else if (strcmp(token_value, "exit") == 0)
				tokens[token_index++] = create_token(TOKEN_COMMAND, token_value);
			else
				tokens[token_index++] = create_token(TOKEN_TEXT, token_value);
		}
		else
			index++;
	}
	tokens[token_index] = NULL;
	return (tokens);
}