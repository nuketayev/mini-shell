/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:26:00 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 16:31:30 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	process_builtins(char **args, t_data *data)
{
	if (ft_strncmp(args[0], "pwd", 4) == 0)
		pwd();
	else if (ft_strncmp(args[0], "echo", 5) == 0)
		echo(args);
	else if (ft_strncmp(args[0], "export", 7) == 0)
		export(args, &(data->envp));
	else if (ft_strncmp(args[0], "unset", 6) == 0)
		unset(args, data->envp);
	else if (ft_strncmp(args[0], "exit", 5) == 0)
		ft_exit(args);
	else if (ft_strncmp(args[0], "env", 4) == 0)
		env(data->envp);
	else if (ft_strncmp(args[0], "cd", 3) == 0 && !data->is_pipe)
		cd(args, data);
	else if (ft_strncmp(args[0], "$?", 3) == 0)
		print_exit_int();
}

void	execute_last(char **envp, char **args, t_data *data)
{
	int		id;
	char	*cmd_path;

	if (is_command(args[0]))
	{
		process_builtins(args, data);
		return ;
	}
	if (args[0][0] == '/' || ft_strncmp(args[0], "./", 2) == 0)
		cmd_path = ft_strdup(args[0]);
	else
		cmd_path = get_command_path(envp, args[0]);
	id = fork();
	if (id == 0)
	{
		if (!cmd_path || execve(cmd_path, args, envp) == -1)
			execve_fail(cmd_path, args, data);
	}
	else
	{
		waitpid(id, NULL, 0);
		close(STDIN_FILENO);
		free(cmd_path);
	}
}

static t_data	*execute(char **envp, char **args, t_data *data)
{
	int		id;
	int		fd[2];
	char	*cmd_path;

	if (is_env_command(args[0]))
	{
		process_builtins(args, data);
		return (data);
	}
	cmd_path = get_command_path(envp, args[0]);
	pipe(fd);
	id = fork();
	if (id == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (is_command(args[0]))
			process_builtins(args, data);
		if (execve(cmd_path, args, envp) == -1)
			execve_fail(cmd_path, args, data);
	}
	else
		data = save_id(data, id, fd, cmd_path);
	return (data);
}

t_data	*process_exec(t_list **command, t_token_type *first, t_data *data)
{
	char	**args;
	t_list	*current;

	args = ft_combine(command);
	current = *command;
	while (current)
	{
		if (((t_token *)current->content)->type == TOKEN_PIPE)
		{
			data->is_pipe = 1;
			break ;
		}
		current = current->next;
	}
	if (*first == TOKEN_LAST)
		prepare_exec_last(command, data, args);
	else
		execute(data->envp, args, data);
	free_split(args);
	return (data);
}
