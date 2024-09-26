#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/inc/libft.h"

typedef enum e_status
{
    END,
    COMMAND,
    R_INPUT,
    R_OUTPUT
}	t_status;

char	**ft_split_str(char *line);

#endif
