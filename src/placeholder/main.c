/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:26:00 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/16 19:39:30 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

volatile sig_atomic_t	g_sigint_received = 0;

static char	*join_args(char **args)
{
	char	*joined_args;
	char	*temp;
	int		i;

	joined_args = ft_strdup("");
	i = 0;
	while (args[i])
	{
		temp = ft_strjoin(joined_args, args[i], 1);
		joined_args = ft_strjoin(temp, " ", 1);
		i++;
	}
	return (joined_args);
}

static void	process_line(t_list *tokens, char *envp[], t_data *data,
		struct sigaction sa)
{
	int	id;

	id = 0;
	if (is_env_command(((t_token *)tokens->content)->value))
	{
		signal(SIGINT, SIG_DFL);
		process_tokens(tokens, envp, data);
	}
	else
	{
		id = fork();
		if (id == 0)
		{
			set_handler_three(&sa);
			process_tokens(tokens, envp, data);
			ft_lstclear(&tokens, free_token);
			free_split(data->envp);
			exit(0);
		}
	}
	set_handler_two(&sa);
	if (id)
		waitpid(id, NULL, 0);
	ft_lstclear(&tokens, free_token);
}

static void	process_input(char *line, struct sigaction sa, char *envp[],
		t_data *data)
{
	t_list	*tokens;
	char	**expanded_args;
	char	*joined_args;

	expanded_args = expand_args(ft_split(line, ' '), envp);
	joined_args = join_args(expanded_args);
	free_split(expanded_args);
	tokens = tokenize_input(joined_args, 0, NULL, NULL);
	data->root_token = tokens;
	free(joined_args);
	if (tokens && validate_tokens(tokens))
		process_line(tokens, envp, data, sa);
	else if (tokens)
		ft_lstclear(&tokens, free_token);
}

static char	**copy_envp(char **envp)
{
	int		i;
	char	**new_envp;

	i = 0;
	while (envp[i])
		i++;
	new_envp = malloc((i + 1) * sizeof(char *));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

int	main(int _argc, char *_argv[], char *envp[])
{
	struct sigaction	sa;
	t_data				data;
	char				*line;

	data.envp = copy_envp(envp);
	data.exit_flag = 0;
	data.fd = -1;
	data.ids = NULL;
	while (1)
	{
		set_handler_one(&sa);
		line = readline("$ ");
		if (line == NULL)
			break ;
		if (ft_strlen(line) > 0)
		{
			add_history(line);
			process_input(line, sa, data.envp, &data);
		}
		free(line);
	}
	free_split(data.envp);
	return (0);
}
