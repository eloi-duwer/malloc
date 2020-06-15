/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 17:25:29 by marvin            #+#    #+#             */
/*   Updated: 2020/06/15 04:10:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

static void	shrink_block(t_block *block, size_t size)
{
	t_block *ptr;

	if (block->next != NULL && block->next->free == true)
	{
		ptr = (t_block *)(shift_block(block) + size);
		ft_memmove(ptr, block->next, sizeof(t_block));
		block->next = ptr;
		block->size = size;
	}
	else if (block->size - size - sizeof(t_block) > 0)
	{
		ptr = (t_block *)(shift_block(block) + size);
		ptr->free = true;
		ptr->size = block->size - sizeof(t_block) - size;
		ptr->next = block->next;
		block->size = size;
		block->next = ptr;
	}
}

void		*mutexed_realloc(void *ptr, size_t size)
{
	t_zone	*zones[2];
	t_block	*blocks[2];

	if (find_block(ptr, zones, blocks) == false)
		return (NULL);
	else if (size == blocks[0]->size)
		return (ptr);
	else if (blocks[0]->size > size)
	{
		shrink_block(blocks[0], size);
		return (ptr);
	}
	else
	{
		
	}
}

void		*realloc(void *ptr, size_t size)
{
	void	*ret;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0 && ptr != NULL)
	{
		free(ptr);
		return (NULL);
	}
	pthread_mutex_lock(&g_mutex);
	ret = mutexed_realloc(ptr, size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
