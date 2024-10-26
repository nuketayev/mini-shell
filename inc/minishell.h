#ifndef MINISHELL_H
# define MINISHELL_H


# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include "sys/wait.h"
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/inc/libft.h"

typedef enum e_token_type
{
    TOKEN_END,
    TOKEN_COMMAND,
    TOKEN_R_INPUT,
	TOKEN_HERE_DOC,
    TOKEN_R_OUTPUT,
	TOKEN_A_OUTPUT,
    TOKEN_TEXT
}	t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
} t_token;

typedef struct s_program
{
    int		infile_fd;
    int		outfile_fd;
    char	*infile_name;
    char	*outfile_name;
    char	**command;
    char	*command_path;
    int		status;
}	t_program;


char	**ft_split_str(char *line);
void	free_split(char **split_line);
void	handler(int signum);
void	free_and_exit(t_program program, char *message);
void	free_commands(t_program *program);
char	**get_path(char *envp[]);
void	execute_last(t_program *program, char **envp, char *argv);
void	open_files(t_program *program);
int	get_command_path(char **envp, t_program *program, char *argv);
void	pipex(int argc, char *argv[], char *envp[]);
t_list  *tokenize_input(char *line);
void process_tokens(t_list *tokens, char *envp[]);



#endif
