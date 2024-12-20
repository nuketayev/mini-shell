/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anuketay <anuketay@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 16:21:43 by anuketay          #+#    #+#             */
/*   Updated: 2024/12/08 16:17:32 by anuketay         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/inc/libft.h"
# include "sys/wait.h"
# include <linux/limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

typedef enum e_token_type
{
	TOKEN_END, // 0
	TOKEN_R_INPUT, // 1
	TOKEN_HERE_DOC, // 2
	TOKEN_R_OUTPUT, // 3
	TOKEN_A_OUTPUT, // 4
	TOKEN_TEXT, // 5
	TOKEN_LAST, // 6
	TOKEN_PIPE 	// 7
}								t_token_type;

typedef struct s_token
{
	t_token_type				type;
	char						*value;
}								t_token;

typedef struct s_data
{
	int							exit_flag;
	char						**envp;
	int							fd;
	t_list						*ids;
	int							last_exit_status;
	int							is_pipe;
	t_list						*root_token;
}								t_data;

extern volatile sig_atomic_t	g_sigint_received;

// signal_handler.c
void							set_handler_one(struct sigaction *sa);
void							set_handler_two(struct sigaction *sa);
void							set_handler_three(struct sigaction *sa);
void							handler_one(int signum);
void							handler_two(int signum);
void							handler_three(int signum);

// parse_input.c
t_list							*tokenize_input(char *line, int index,
									t_list *new, t_list *root);
char							**expand_args(char **args, char **envp);
int								find_dollar(char *arg);
char							*remove_quotes(char *var_name);

// process_tokens.c
void							process_tokens(t_list *tokens, t_data *data);
int								validate_tokens(t_list *tokens);
void							handle_token_exec(t_list **tokens,
									t_data **data);
void							handle_token_pipe(t_list **tokens,
									t_data **data);
int								handle_token_input(t_list **tokens);
void							handle_token_here_doc(t_list **tokens);
void							handle_token_output(t_list **tokens);

// pipex_utils.c
char							*get_command_path(char **envp, char *argv);
void							free_split(char **args);
int								is_command(char *cmd);
int								is_env_command(char *cmd);

// get_last_token.c
t_list							*finish_tokenizing(t_list *first);
void							free_token(void *lst);

// handle_quotes.c
char							*handle_quotes(char *line);

// execute.c
t_data							*process_exec(t_list **command,
									t_token_type *first, t_data *data);

// redirections.c
int								redirect_input(char *filename);
int								redirect_output(char *filename,
									t_token_type type, int isdup);
void							here_doc(char *limiter);

void							unset(char **args, char *envp[]);
void							pwd(void);
void							cd(char **args, t_data *data);
void							export(char **args, char ***envp);
void							sort_envp(char ***envp);
void							print_sorted_envp(char ***envp);
void							ft_exit(char **args);
void							env(char *envp[]);
void							echo(char **args);
void							print_exit_int(void);
int								ft_strlen_until(char *str, char c);
int								count_chars(char *arg, char c);
void							find_last_redirection(t_list **command);
char							**ft_combine(t_list **command);
void							execve_fail(char *cmd_path, char **args, t_data *data);
void							execute_last(char **envp, char **args, t_data *data);
void							prepare_exec_last(t_list **command, t_data *data, char **args);
t_data							*save_id(t_data *data, int id, int *fd, char *cmd_path);

void							print_lst(t_list *lst);
void							print_array(char **arr);
#endif
