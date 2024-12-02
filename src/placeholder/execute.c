/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:26:00 by anuketay          #+#    #+#             */
/*   Updated: 2024/11/16 16:42:03 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void process_builtins(char **args, t_data *data, int is_pipe)
{
    if (ft_strncmp(args[0], "pwd", 4) == 0)
        pwd(args, data->envp);
    else if (ft_strncmp(args[0], "echo", 5) == 0)
        echo(args, data->envp);
    else if (ft_strncmp(args[0], "export", 7) == 0)
        export(args, &(data->envp));
    else if (ft_strncmp(args[0], "unset", 6) == 0)
        unset(args, data->envp);
    else if (ft_strncmp(args[0], "exit", 5) == 0)
        ft_exit();
    else if (ft_strncmp(args[0], "env", 4) == 0)
        env(data->envp);
    else if (ft_strncmp(args[0], "cd", 3) == 0 && !is_pipe)
        cd(args, data);
}

static void execute_last(char **envp, char **args, t_data *data, int is_pipe)
{
    int id;
    char *cmd_path;
    char **expanded_args;

    expanded_args = expand_args(args, envp);

    if (is_command(expanded_args[0]))
    {
        process_builtins(expanded_args, data, is_pipe);
        free_split(expanded_args);
        return;
    }

    if (expanded_args[0][0] == '/')
    {
        cmd_path = ft_strdup(expanded_args[0]);
    }
    else
    {
        cmd_path = get_command_path(envp, expanded_args[0]);
    }

    id = fork();
    if (id == 0)
    {
        if (execve(cmd_path, expanded_args, envp) == -1)
            ft_printf("%s: command not found\n", expanded_args[0]);
    }
    else
    {
        waitpid(id, NULL, 0);
        free(cmd_path);
    }

    free_split(expanded_args);
}

static t_data	*execute(char **envp, char **args, t_data *data, int is_pipe)
{
    int		id;
    int		fd[2];
    char	*cmd_path;

    if (is_command(args[0]))
    {
        process_builtins(args, data, is_pipe);
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
        if (execve(cmd_path, args, envp) == -1)
            ft_printf("%s: command not found\n", args[0]);
    }
    else
    {
    	if (data->ids == NULL)
            data->ids = ft_lstnew((void *)(intptr_t)id);
        else
            ft_lstadd_back(&data->ids, ft_lstnew((void *)(intptr_t)id));
    	dup2(fd[0], STDIN_FILENO);
    	close(fd[0]);
    	close(fd[1]);
        free(cmd_path);
    }
	return (data);
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

t_data	*process_exec(t_list **command, t_token_type *first, t_data *data)
{
	char	**args;
	int		fd;
    int     is_pipe;
    t_list  *current;


	args = ft_combine(command);
	fd = -1;
    is_pipe = 0;

    current = *command;
    while (current)
    {
        if (((t_token *)current->content)->type == TOKEN_PIPE)
        {
            is_pipe = 1;
            break;
        }
        current = current->next;
    }
	if (*first == TOKEN_LAST)
	{
		if (((t_token *)(*command)->content)->type == TOKEN_R_OUTPUT
			|| ((t_token *)(*command)->content)->type == TOKEN_A_OUTPUT)
		{
			fd = redirect_output(((t_token *)(*command)->next->content)->value,
					((t_token *)(*command)->content)->type);
			*command = (*command)->next->next;
		}
		execute_last(data->envp, args, data, is_pipe);
	}
	else
		execute(data->envp, args, data, is_pipe);
	if (fd != -1)
		close(fd);
	free_split(args);
	return (data);
}
