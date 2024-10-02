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

typedef enum e_status
{
    END,
    COMMAND,
    R_INPUT,
	HERE_DOC,
    R_OUTPUT,
	A_OUTPUT
}	t_status;

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
void	pipex(char *arg, char *envp[]);


#endif
