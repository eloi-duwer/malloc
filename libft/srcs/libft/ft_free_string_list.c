#include <stdlib.h>

void	ft_free_string_list(char **lst)
{
	int i;

	i = -1;
	while (lst[++i])
		free(lst[i]);
	free(lst);
}
