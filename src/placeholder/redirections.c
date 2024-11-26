#include "../../inc/minishell.h"

int	redirect_input(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		ft_errprintf("wrong file kurwo\n");
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redirect_output(char *filename, t_token_type type)
{
	int	fd;

	if (type == TOKEN_R_OUTPUT)
		fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0666);
	else
		fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd == -1)
	{
		ft_errprintf("wrong file kurwo\n");
		return (-1);
	}
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
	buffer = (char *)malloc(10000);
	if (!buffer)
		return (-1);
	write(0, "> ", 2);
	result = read(0, &c, 1);
	while (result && c != '\n' && c != '\0')
	{
		if (c != '\n' && c != '\0')
			buffer[i] = c;
		i++;
		result = read(0, &c, 1);
	}
	buffer[i] = '\n';
	buffer[++i] = '\0';
	*line = buffer;
	free(buffer);
	return (result);
}

void	here_doc(char *limiter)
{
	pid_t	reader;
	int		fd[2];
	char	*line;

	if (pipe(fd) == -1)
		ft_errprintf("wrong pipe man\n");
	reader = fork();
	if (reader == 0)
	{
		close(fd[0]);
		while (get_another_line(&line))
		{
			if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0)
				exit(EXIT_SUCCESS);
			write(fd[1], line, ft_strlen(line));
		}
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		wait(NULL);
	}
}