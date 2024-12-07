#include "../../inc/minishell.h"

int find_dollar(char *arg)
{
    int i;

    i = 0;
    while (arg[i])
    {
        if (arg[i] == '$')
            return (i);
        i++;
    }
    return (-1);
}

char    *remove_quotes(char *var_name)
{
    char    *new_str;
    int     i;

    new_str = malloc(ft_strlen(var_name) * sizeof(char));
    if (!new_str)
        return NULL;
    if (var_name[0] == '\"')
        i = 1;
    else
        i = 0;
    while (var_name[i] && var_name[i] != '\"')
    {
        new_str[i - 1] = var_name[i];
        i++;
    }
    new_str[i - 1] = '\0';
    return new_str;
}

static char *get_env_value(char *var_name, char **envp)
{
    char *var_value = NULL;
    int i = 0;
    if (ft_findchar(var_name, '\"') == -1)
        var_name = remove_quotes(var_name);
    while (envp[i])
    {
        if (ft_strncmp(envp[i], var_name + 1, ft_strlen(var_name + 1)) == 0)
        {
            var_value = ft_strdup(envp[i] + ft_strlen(var_name));
            break;
        }
        i++;
    }

    return var_value;
}



static int is_expansion(char *arg)
{
    int i;

    i = find_dollar(arg);
    if (i == -1)
        return (1);

    if (ft_strncmp(arg, "\"$\"", 3) == 0 || ft_strncmp(&arg[i], "$?", 3) == 0 || ft_strncmp(&arg[i], "$", 2) == 0)
    {
        return 1;
    }
    if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
        return 1;
    return 0;
}

static char *expand_env_var(char *arg, char **envp)
{
    char *expanded_arg;
    char *var_name;
    char *var_value;

    if (is_expansion(arg))
        return ft_strdup(arg);

    var_name = ft_strdup(arg);
    var_value = get_env_value(var_name, envp);

    if (var_value)
        expanded_arg = ft_strdup(var_value);
    else
        expanded_arg = ft_strdup("");

    free(var_name);
    free(var_value);
    return expanded_arg;
}

char **expand_args(char **args, char **envp)
{
    char **expanded_args;
    int i;
    int count;

    count = 0;
    while (args[count])
        count++;

    expanded_args = malloc((count + 1) * sizeof(char *));
    if (!expanded_args)
        return NULL;

    i = 0;
    while (args[i])
    {
        expanded_args[i] = expand_env_var(args[i], envp);
        i++;
    }

    expanded_args[i] = NULL;
    return expanded_args;
}
