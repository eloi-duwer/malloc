#include <malloc.h>

void	write_str(char	*str)
{
	size_t len;

	len = 0;
	while (str[len] != '\0')
		++len;
	
	write(1, str, len);
}

void	*malloc(size_t size)
{
	write_str("no malloc nor ft_printf for you...\n");
	//ft_printf("lol ca crash %p\n", NULL);
	return (NULL);	
}

void	free(void *ptr)
{
	ft_printf("No free 4 u\n");
}

void	*realloc(void *ptr, size_t size)
{
	ft_printf("No realloc 4 u neither\n");
	return (ptr);
}
