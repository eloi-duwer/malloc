/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 21:44:13 by marvin            #+#    #+#             */
/*   Updated: 2020/06/16 01:21:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

t_zone			*g_zones = NULL;
pthread_mutex_t	g_mutex = PTHREAD_MUTEX_INITIALIZER;

t_bool	claim_block(size_t size, t_zone *zone, t_block **ret_block)
{
	t_block *ptr;
	t_block	*next;

	ptr = (t_block *)shift_zone(zone);
	while (ptr != NULL)
	{
		if (ptr->free == true && ptr->size >= size)
		{
			if (ptr->size > size + sizeof(t_block))
			{
				next = (t_block *)((void *)ptr + sizeof(t_block) + size);
				next->free = true;
				next->size = ptr->size - sizeof(t_block) - size;
				next->next = NULL;
				ptr->next = next;
				ptr->size = size;
			}
			else
				ptr->next = NULL;
			ptr->free = false;
			*ret_block = ptr;
			return (true);
		}
		ptr = ptr->next;
	}
	return (false);
}

void	find_or_create_tiny_small_block(t_type type, size_t size, \
			t_zone **zone, t_block **block) 
{
	*zone = g_zones;
	while (*zone != NULL)
	{
		if ((*zone)->type == type && claim_block(size, *zone, block) == true)
			return;
		*zone = (*zone)->next;
	}
	if ((*zone = mmap_tiny_small_zone(type, block)) == NULL)
		return;
	claim_block(size, *zone, block);
}

void	get_zone_and_block(size_t size, t_zone **zone, t_block **block)
{
	if (size > SMALL_BLOC_BYTES)
		*zone = mmap_large_zone(size);
	else if (size <= TINY_BLOC_BYTES)
		find_or_create_tiny_small_block(TINY, size, zone, block);
	else
		find_or_create_tiny_small_block(SMALL, size, zone, block);
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
	if (zone->type == LARGE)
		return (shift_zone(zone));
	else if (block != NULL)
		return (shift_block(block));
	return (NULL);
}

void	*malloc(size_t size)
{
	void	*ret;

	pthread_mutex_lock(&g_mutex);
	ret = mutexed_malloc(size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}

void	*calloc(size_t n, size_t size)
{
	void	*ret;

	pthread_mutex_lock(&g_mutex);
	ret = mutexed_malloc(n * size);
	pthread_mutex_unlock(&g_mutex);
	ft_bzero(ret, n * size);
	return (ret);
}
