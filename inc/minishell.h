/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:21:43 by anuketay          #+#    #+#             */
/*   Updated: 2024/11/16 16:21:45 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/inc/libft.h"
# include "sys/wait.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <linux/limits.h>

typedef enum e_token_type
{
	TOKEN_END,
	TOKEN_R_INPUT,
	TOKEN_HERE_DOC,
	TOKEN_R_OUTPUT,
	TOKEN_A_OUTPUT,
	TOKEN_TEXT,
	TOKEN_LAST,
	TOKEN_PIPE
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}					t_token;

typedef struct s_data
{
	int		exit_flag;
	char	**envp;
	int		fd;
	t_list	*ids;
	int		last_exit_status;
}			t_data;

extern volatile sig_atomic_t g_sigint_received;

//signal_handler.c
void	set_handler_one(struct sigaction *sa);
void	set_handler_two(struct sigaction *sa);
void	set_handler_three(struct sigaction *sa);

//parse_input.c
t_list	*tokenize_input(char *line, int index, t_list *new, t_list *root);
char **expand_args(char **args, char **envp);

//pipex.c
void	process_tokens(t_list *tokens, char *envp[], t_data *data);

//pipex_utils.c
char	*get_command_path(char **envp, char *argv);
void	free_split(char **args);
int		is_command(char *cmd);
int		is_env_command(char *cmd);

//get_last_token.c
t_list	*finish_tokenizing(t_list *first);

//handle_quotes.c
char	*handle_quotes(char *line);

//execute.c
t_data	*process_exec(t_list **command, t_token_type *first, t_data *data);

//redirections.c
int		redirect_input(char *filename);
int		redirect_output(char *filename, t_token_type type, int isdup);
void	here_doc(char *limiter);

void	unset(char **args, char *envp[]);
void	pwd(char **args, char *envp[]);
void	cd(char **args, t_data *data);
void	export(char **args, char ***envp);
void	ft_exit(void);
void	env(char *envp[]);
void	echo(char **args, char *envp[]);
void	print_exit_int();

void	print_lst(t_list *lst);
void	print_array(char **arr);
#endif
