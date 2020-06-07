/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_whole_file.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/28 20:03:01 by eduwer            #+#    #+#             */
/*   Updated: 2020/02/29 16:07:54 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include <unistd.h>

static int	remalloc_the_line(void **ptr, size_t *current_length, \
				size_t size_to_add)
{
	void	*newbuf;

	if ((newbuf = (void *)malloc(*current_length + size_to_add)) == NULL)
		return (1);
	if (*current_length > 0 && *ptr)
	{
		ft_memcpy(newbuf, *ptr, *current_length);
		free(*ptr);
	}
	*ptr = newbuf;
	*current_length += size_to_add;
	return (0);
}

int			read_whole_file(int fd, void **ptr, size_t *size)
{
	int		readsize;
	void	*ret;
	char	buff[READ_BUFF_SIZE];
	size_t	total_size;

	total_size = 0;
	while (((readsize = read(fd, buff, READ_BUFF_SIZE)) == READ_BUFF_SIZE) \
		|| (fd == 0 && readsize > 0))
	{
		if (remalloc_the_line(&ret, &total_size, readsize) != 0)
			return (1);
		ft_memcpy(ret + total_size - readsize, buff, readsize);
	}
	if (readsize == -1)
		return (1);
	if (remalloc_the_line(&ret, &total_size, readsize) != 0)
		return (1);
	ft_memcpy(ret + total_size - readsize, buff, readsize);
	*ptr = ret;
	*size = total_size;
	return (0);
}
