/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 01:40:02 by marvin            #+#    #+#             */
/*   Updated: 2020/06/27 02:14:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

t_bool	find_block(void *ptr, t_zone **ret_zone, t_block **ret_block)
{
	if (g_zones == NULL)
		return (false);
	ret_zone[0] = g_zones;
	ret_zone[1] = NULL;
	while (ret_zone[0]->next != NULL && (void *)ret_zone[0]->next < ptr)
	{
		ret_zone[1] = ret_zone[0];
		ret_zone[0] = ret_zone[0]->next;
	}
	if (ret_zone[0] == NULL || (void *)ret_zone[0] > ptr)
	{
		show_alloc_mem();
		return (false);
	}
	ret_block[0] = (t_block *)shift_zone(ret_zone[0]);
	ret_block[1] = NULL;
	while (ret_block[0] != NULL)
	{
		if (shift_block(ret_block[0]) == ptr)
			return (true);
		ret_block[1] = ret_block[0];
		ret_block[0] = ret_block[0]->next;
	}
	return (false);
}

static void	free_zone(t_zone **zones)
{
	if (zones[1] != NULL)
		zones[1]->next = zones[0]->next;
	else
		g_zones = zones[0]->next;
	munmap(zones[0], zones[0]->size);
}

static void	check_zone_freeable(t_zone **zones)
{
	t_block	*block;

	block = (t_block *)shift_zone(zones[0]);
	while (block != NULL)
	{
		if (block->free == false)
			return;
		block = block->next;
	}
	free_zone(zones);
}

void	mutexed_free(void *ptr)
{
	t_zone	*zones[2];
	t_block	*blocks[2];

	if (find_block(ptr, zones, blocks) == false)
		return;
	blocks[0]->free = true;
	if (blocks[0]->next != NULL && blocks[0]->next->free == true)
	{
		blocks[0]->size += blocks[0]->next->size + sizeof(t_block);
		blocks[0]->next = blocks[0]->next->next;
	}
	if (blocks[1] != NULL && blocks[1]->free == true)
	{
		blocks[1]->size += blocks[0]->size + sizeof(t_block);
		blocks[1]->next = blocks[0]->next;
	}
	check_zone_freeable(zones);
}

void	free(void *ptr)
{
	if (ptr == NULL)
		return;
	pthread_mutex_lock(&g_mutex);
	mutexed_free(ptr);
	pthread_mutex_unlock(&g_mutex);
}
