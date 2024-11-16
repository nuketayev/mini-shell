/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:42:24 by anuketay          #+#    #+#             */
/*   Updated: 2024/11/16 16:46:02 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	skip_spaces(char *line, int i)
{
	while (line[i])
	{
		if (line[i] != ' ')
			return (i);
	}
}

int	followed_by_command(char *line, int i)
{
	while (line[i] != ' ')
		i++;
	if (line[i] != '>' && line[i] != '<')
		return (1);
	return (0);
}

int	get_process_count(char *line)
{
	int	i;
	int	count;

	if (!line)
		return (0);
	i = 0;
	if (line[0] == '>' || line[0] == '<')
		count = 0;
	else
		count = 1;
	while (line[i])
	{
		if (line[i] == '|' || (line[i] == '>' && line[i + 1] != '>')
			|| (line[i] == '<' && line[i + 1] != '<'))
		{
			count++;
			if (line[i] != '|' && followed_by_command(line, i))
				count++;
		}
		i++;
	}
	return (count);
}

int	main(void)
{
	char	*line;
	char	**split_line;
	int		*process_list;
	int		process_count;

	while (1)
	{
		line = readline("$ ");
		process_count = get_process_count(line);
		printf("%i\n", process_count);
	}
}
