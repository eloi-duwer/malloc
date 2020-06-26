/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 21:44:13 by marvin            #+#    #+#             */
/*   Updated: 2020/06/27 01:16:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

t_zone			*g_zones = NULL;
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

void	try_split_block(t_block *block, size_t size)
{
	t_block	*next;

	if (block->size <= size + sizeof(t_block))
		return ;
	next = (t_block *)(shift_block(block) + size);
	next->next = block->next;
	next->free = true;
	next->size = block->size - size - sizeof(t_block);
	block->size = size;
	block->next = next;
}

t_bool	claim_block(size_t size, t_zone *zone, t_block **ret_block)
{
	t_block	*ptr;

	ptr = (t_block *)shift_zone(zone);
	while (ptr != NULL)
	{
		if (ptr->free == true && ptr->size >= size)
		{
			try_split_block(ptr, size);
			ptr->free = false;
			*ret_block = ptr;
			return (true);
		}
		ptr = ptr->next;
	}
	return (false);
}

void	find_or_create_block(t_type type, size_t size, \
			t_zone **zone, t_block **block) 
{
	if (type == LARGE)
	{
		if ((*zone = \
			mmap_zone(type, get_nb_pages(LARGE, size), block)) == NULL)
			return;
	}
	else
	{
		*zone = g_zones;
		while (*zone != NULL)
		{
			if ((*zone)->type == type \
				&& claim_block(size, *zone, block) == true)
				return;
			*zone = (*zone)->next;
		}
		if ((*zone = mmap_zone(type, get_nb_pages(type, size), block)) == NULL)
			return;
	}
	claim_block(size, *zone, block);
}

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

	//size = (size + 15) & ~15;
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

	size = (size + 1) & ~1;
	pthread_mutex_lock(&g_mutex);
	ret = mutexed_malloc(size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}

void	*calloc(size_t n, size_t size)
{
	void	*ret;

	size = (size + 1) & ~1;
	pthread_mutex_lock(&g_mutex);
	ret = mutexed_malloc(n * size);
	ft_bzero(ret, n * size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
