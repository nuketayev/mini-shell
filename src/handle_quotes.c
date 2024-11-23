#include "../inc/minishell.h"

static char	*remove_char_until(char *line, char c) //handle_quotes.c
{
	char	*f_line;
	int		count;
	int		i;

	if (!line)
		return (NULL);
	count = 2;
	i = 0;
	while (line[i] && count > 0)
	{
		if (line[i] != c)
			i++;
		else
		{
			i++;
			count--;
		}
	}
	f_line = ft_substr(line, 1, i - 2);
	return (f_line);
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
