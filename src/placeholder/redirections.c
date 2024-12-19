/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 13:24:17 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 13:45:53 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_errprintf("minishell: %s: No such file or directory\n", filename);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redirect_output(char *filename, t_token_type type, int isdup)
{
	int	fd;

	if (type == TOKEN_R_OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		ft_errprintf("minishell: %s: Failed to open file\n", filename);
		return (-1);
	}
	if (isdup)
		dup2(fd, STDOUT_FILENO);
	return (fd);
}

static int	get_another_line(char **line)
{
	int		i;
	int		result;
	char	c;
	char	*buffer;

	i = 0;
	result = 0;
	buffer = (char *)malloc(1024);
	if (!buffer)
		return (-1);
	write(STDOUT_FILENO, "> ", 2);
	while ((result = read(STDIN_FILENO, &c, 1)) > 0 && c != '\n' && c != '\0')
	{
		if (i >= 1023)
		{
			free(buffer);
			return (-1);
		}
		buffer[i++] = c;
	}
	if (result == -1)
	{
		free(buffer);
		return (-1);
	}
	buffer[i] = '\n';
	buffer[++i] = 0;
	*line = buffer;
	return (result);
}

static void	read_lines(int (*fd)[2], char *limiter)
{
	char	*line;

	close((*fd)[0]);
	while (get_another_line(&line))
	{
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
		{
			free(line);
			exit(EXIT_SUCCESS);
		}
		write((*fd)[1], line, ft_strlen(line));
		free(line);
	}
}

void	here_doc(char *limiter)
{
	pid_t	reader;
	int		fd[2];

	if (pipe(fd) == -1)
		ft_errprintf("minishell: pipe error\n");
	reader = fork();
	if (reader == -1)
		ft_errprintf("minishell: fork error\n");
	else if (reader == 0)
		read_lines(&fd, limiter);
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}
