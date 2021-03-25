/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 21:44:13 by marvin            #+#    #+#             */
/*   Updated: 2021/03/26 00:01:18 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

t_zone			*g_zones = NULL;
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

void	get_zone_and_block(size_t size, t_zone **zone, t_block **block)
{
	if (size > SMALL_BLOC_BYTES)
		find_or_create_block(LARGE, size, zone, block);
	else if (size <= TINY_BLOC_BYTES)
		find_or_create_block(TINY, size, zone, block);
	else
		find_or_create_block(SMALL, size, zone, block);
}

void	*mutexed_malloc(size_t size)
{
	t_zone	*zone;
	t_block	*block;

	if (size == 0)
		return (NULL);
	get_zone_and_block(size, &zone, &block);
	if (zone == NULL)
		return (NULL);
	else if (block != NULL)
		return (shift_block(block));
	return (NULL);
}

void	*malloc(size_t size)
{
	void	*ret;
	size_t	align;

	align = get_align();
	size = (size + align) & ~align;
	pthread_mutex_lock(&g_mutex);
	ret = mutexed_malloc(size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}

void	*calloc(size_t n, size_t size)
{
	void	*ret;
	size_t	align;

	align = get_align();
	size = (size + align) & ~align;
	pthread_mutex_lock(&g_mutex);
	ret = mutexed_malloc(n * size);
	ft_bzero(ret, n * size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
