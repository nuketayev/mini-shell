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
