#include "../../inc/minishell.h"

void	print_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		ft_printf("%s\n", arr);
		i++;
	}
}

void	print_lst(t_list *lst)
{
	while (lst)
	{
		ft_printf("value: %s, type: %i\n", ((t_token *)lst->content)->value,
			((t_token *)lst->content)->type);
		lst = lst->next;
	}
}
