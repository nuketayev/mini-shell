#include "../inc/minishell.h"

sig_atomic_t	g_sigint_received = 0;

void	handler(int signum)
{
	if (signum == SIGINT)
		g_sigint_received = 1;
}

void	free_split(char **split_line)
{
	int	i;

	i = 0;
	while (split_line[i])
	{
		free(split_line[i]);
		i++;
	}
	free(split_line);
}

int	ft_splitlen(char *arr[])
{
	int	len;
	if (!arr)
		return (0);
	len = 0;
	while (arr[len])
		len++;
	return (len);
}

int	main(int _argc, char *_argv[], char *envp[])
{
	int    *status;
	char	*line;
	char	**split_line;
	int		i;
	int		split_len;
	int	id;

	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, SIG_IGN);
	while (1)
	{
		i = 0;
		if (g_sigint_received)
		{
			printf("\n");
			g_sigint_received = 0;
			continue ;
		}
		line = readline("$ ");
		if (line == NULL)
			break ;
		add_history(line);
		id = fork();
		if (id == 0)
		{
			split_line = ft_split(line, '|');
			split_len = ft_splitlen(split_line);
			pipex(split_len, split_line, envp);
		}
		waitpid(id, NULL, 0);
		free(line);
	}
	return (0);
}
