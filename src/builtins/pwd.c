#include "../../inc/minishell.h"
#include <linux/limits.h>

void	pwd(char **args, char *envp[])
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		ft_printf("%s\n", cwd);
	else
		perror("pwd");
}
