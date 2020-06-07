/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/17 14:49:01 by eduwer            #+#    #+#             */
/*   Updated: 2020/02/28 22:30:29 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <unistd.h>
#include <stdlib.h>
#include "libft.h"

int				remalloc_the_line(char **line, int size)
{
	char *buff;

	size = ft_strlen(*line) + size + 1;
	if ((buff = (char *)malloc(size)) == NULL)
		return (-1);
	ft_memset(buff, 0, size);
	ft_strcpy(buff, *line);
	free(*line);
	*line = buff;
	return (0);
}

int				size_to_add(char *line)
{
	int i;

	i = 0;
	while (line[i] != '\n' && line[i] != '\0')
		i++;
	return (i);
}

int				ft_empty_the_buffer(char **line, char **cursor, int i)
{
	int j;

	if (remalloc_the_line(line, size_to_add(*cursor)) == -1)
		return (-1);
	j = 0;
	while ((*cursor)[j] != '\n' && (*cursor)[j] != '\0')
	{
		(*line)[i] = (*cursor)[j];
		i++;
		j++;
	}
	if ((*cursor)[j] == '\n')
	{
		if (i > 0 && (*line)[i - 1] == '\r')
			(*line)[i - 1] = '\0';
		j++;
		*cursor = &((*cursor)[j]);
		return (1);
	}
	**cursor = '\0';
	return (0);
}

int				ft_check(int ret, char **line)
{
	if (ret == -1)
		return (-1);
	if (ret == 0 && (*line)[0] == '\0')
		return (0);
	if (ret == 0)
		return (1);
	return (2);
}

int				get_next_line(const int fd, char **line)
{
	static char stat_buff[BUFF_SIZE] = {'\0'};
	static char *cursor = NULL;
	int			ret;

	if (cursor == NULL)
		cursor = stat_buff;
	if (line == NULL || fd < 0)
		return (-1);
	if ((*line = (char *)malloc(1)) == NULL)
		return (-1);
	(*line)[0] = '\0';
	if (*cursor != '\0' && \
		(ret = ft_empty_the_buffer(line, &cursor, 0)) != 0)
		return (ret);
	while (1)
	{
		ft_memset(stat_buff, 0, BUFF_SIZE);
		cursor = stat_buff;
		ret = read(fd, stat_buff, BUFF_SIZE);
		if (ft_check(ret, line) != 2)
			return (ft_check(ret, line));
		if ((ret = ft_empty_the_buffer(line, &cursor, ft_strlen(*line))) != 0)
			return (ret);
	}
}
