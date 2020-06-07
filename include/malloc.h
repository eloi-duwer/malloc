#ifndef __MALLOC_H__
# define __MALLOC_H__
# include <stdlib.h>
# include <sys/mman.h>
# include <libft.h>

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);

#endif
