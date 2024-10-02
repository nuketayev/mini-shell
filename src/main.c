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

int	main(int _argc, char *_argv[], char *envp[])
{
	int    *status;
	char	*line;
	char	**split_line;
	int		i;

	status = malloc(sizeof(int) * 3);
	status[0] = COMMAND;
	status[1] = R_INPUT;
	status[2] = END;
	i = 0;
	while (status[i] != END)
	{
        printf("%i\n", status[i]);
        i++;
    }
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
		pipex(line, envp);
		wait(0);
	}
	return (0);
}
