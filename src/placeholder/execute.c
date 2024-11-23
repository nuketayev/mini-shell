#include "../../inc/minishell.h"

static void	process_builtins(char **args, char *envp[], t_data *data)
{
	if (ft_strncmp(args[0], "pwd", 3) == 0)
		pwd(args, envp);
	else if (ft_strncmp(args[0], "echo", 4) == 0)
		echo(args, envp);
	else if (ft_strncmp(args[0], "export", 6) == 0) // same case as in unset
		export(args, envp);
	else if (ft_strncmp(args[0], "unset", 5) == 0) //we might have to put envp in some new struct so the unset works
		unset(args, envp);
	else if (ft_strncmp(args[0], "exit", 4) == 0) //test this in bash because it looks like if it's piped it does not exit
		new_exit(args, envp, data);
	else if (ft_strncmp(args[0], "env", 3) == 0)
		env(args, envp);
	else if (ft_strncmp(args[0], "cd", 3) == 0)
		cd(args, envp);
	exit(0);
}

static void	execute_last(char **envp, char **args, t_data *data)
{
	int		id;
	char	*cmd_path;

	cmd_path = get_command_path(envp, args[0]);
	id = fork();
	if (id == 0)
	{
		if (is_command(args[0]))
			process_builtins(args, envp, data);
		else if (execve(cmd_path, args, envp) == -1)
			ft_errprintf("nie dziala kurwa\n");
	}
	else
	{
		waitpid(id, NULL, 0);
		free(cmd_path);
	}
}

static void	execute(char **envp, char **args, t_data *data)
{
	int		id;
	int		fd[2];
	char	*cmd_path;

	cmd_path = get_command_path(envp, args[0]);
	pipe(fd);
	id = fork();
	if (id == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		if (is_command(args[0]))
			process_builtins(args, envp, data);
		else if (execve(cmd_path, args, envp) == -1)
			ft_errprintf("nie dziala kurwa\n");
	}
	else
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		free(cmd_path);
	}
}

static char	**ft_combine(t_list **command)
{
	char	**args;
	int		i;

	args = (char **)malloc(80);
	i = 0;
	while (((t_token *)(*command)->content)->type == TOKEN_TEXT
		|| ((t_token *)(*command)->content)->type == TOKEN_LAST)
	{
		args[i] = ft_strdup(((t_token *)(*command)->content)->value);
		i++;
		*command = (*command)->next;
	}
	args[i] = NULL;
	return (args);
}

void	process_exec(t_list **command, char *envp[], t_token_type *first, t_data *data)
{
	char	**args;
	int		fd;

	args = ft_combine(command);
	fd = -1;
	if (*first == TOKEN_LAST)
	{
		if (((t_token *)(*command)->content)->type == TOKEN_R_OUTPUT
			|| ((t_token *)(*command)->content)->type == TOKEN_A_OUTPUT)
		{
			fd = redirect_output(((t_token *)(*command)->next->content)->value,
					((t_token *)(*command)->content)->type);
			*command = (*command)->next->next;
		}
		execute_last(envp, args, data);
	}
	else
		execute(envp, args, data);
	if (fd != -1)
		close(fd);
	free_split(args);
}
