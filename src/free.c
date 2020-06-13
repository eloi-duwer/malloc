/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/13 01:40:02 by marvin            #+#    #+#             */
/*   Updated: 2020/06/13 03:02:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

t_bool	find_block(void *ptr, t_zone **ret_zone, t_block **ret_block)
{
	ret_zone[0] = g_zones;
	ret_zone[1] = NULL;
	while (ret_zone[0] != NULL)
	{
		ret_block[0] = (t_block *)shift_zone(ret_zone[0]);
		ret_block[1] = NULL;
		while (ret_block[0] != NULL)
		{
			if ((void *)(ret_block[0]) == ptr)
				return (true);
			ret_block[1] = ret_block[0];
			ret_block[0] = ret_block[0]->next;
		}
		ret_zone[1] = ret_zone[0];
		ret_zone[0] = ret_zone[0]->next;
	}
	return (false);
}

static void	free_large_zone(t_zone **zones)
{
	
}

static void	check_zone_freeable(t_zone **ptr)
{

}

static void	mutexed_free(void *ptr)
{
	t_zone	*zones[2];
	t_block	*blocks[2];

	if (find_block(ptr, zones, blocks) == false)
		return;
	if (zones[0]->type == LARGE)
	{
		free_large_zone(zones);
		return;
	}
	blocks[0]->free = true;
	if (blocks[0]->next != NULL && blocks[0]->free == true)
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
