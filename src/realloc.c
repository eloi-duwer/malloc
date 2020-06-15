/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 17:25:29 by marvin            #+#    #+#             */
/*   Updated: 2020/06/16 01:21:19 by marvin           ###   ########.fr       */
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

static void	reduce_next_block(t_block *block, size_t size)
{
	t_block *ptr;
	size_t	next_size;

	if (block->size + block->next->size > size)
	{
		next_size = block->size + block->next->size - size;
		ptr = (t_block *)(shift_block(block) + size);
		ft_memmove(ptr, block->next, sizeof(t_block));
		ptr->size = next_size;
		block->next = ptr;
		block->size = size;
	}
}


static void	*expand_block_or_malloc(void *ptr, t_block *block, size_t size)
{
	size_t	expandable_size;
	void	*ret;

	expandable_size = block->size;
	if (block->next != NULL && block->next->free == true)
		expandable_size += block->next->size + sizeof(t_block);
	if (expandable_size >= size)
	{
		reduce_next_block(block, size);
		return (ptr);
	}
	else
	{
		if ((ret = mutexed_malloc(size)) == NULL)
			return (NULL);
		ft_memcpy(ret, ptr, block->size);
		mutexed_free(ptr);
		return (ret);
	}
}

void		*mutexed_realloc(void *ptr, size_t size)
{
	t_zone	*zones[2];
	t_block	*blocks[2];
	void	*ret;

	if (find_block(ptr, zones, blocks) == false)
		return (NULL);
	else if (zones[0]->type == LARGE)
	{
		if (zones[0]->size >= size)
			return (ptr);
		if ((ret = mutexed_malloc(size)) == NULL)
			return (NULL);
		ft_memcpy(ret, ptr, zones[0]->size - sizeof(t_zone));
		mutexed_free(ptr);
		return (ret);
	}
	else if (size == blocks[0]->size)
		return (ptr);
	else if (blocks[0]->size > size)
	{
		shrink_block(blocks[0], size);
		return (ptr);
	}
	else
		return (expand_block_or_malloc(ptr, blocks[0], size));
}

void		*realloc(void *ptr, size_t size)
{
	void	*ret;

	if (ptr == NULL)
		return (malloc(size));
	if (size == 0)
	{
		free(ptr);
		return (NULL);
	}
	pthread_mutex_lock(&g_mutex);
	ret = mutexed_realloc(ptr, size);
	pthread_mutex_unlock(&g_mutex);
	return (ret);
}
