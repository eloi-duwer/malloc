/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 17:25:29 by marvin            #+#    #+#             */
/*   Updated: 2020/06/27 02:14:51 by marvin           ###   ########.fr       */
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
		ptr->size += block->size - size;
		block->next = ptr;
		block->size = size;
	}
	else if (block->size > size + sizeof(t_block))
	{
		ptr = (t_block *)(shift_block(block) + size);
		ptr->next = block->next;
		ptr->free = true;
		ptr->size = block->size - (sizeof(t_block) + size);
		block->size = size;
		block->next = ptr;
	}
}

static t_bool	try_reduce_next_block(t_block *block, size_t size)
{
	t_block *ptr;
	size_t	next_size;

	if (block->next != NULL && block->next->free == true \
		&& block->size + block->next->size > size + sizeof(t_block))
	{
		next_size = block->size + block->next->size - size;
		ptr = (t_block *)(shift_block(block) + size);
		ft_memmove(ptr, block->next, sizeof(t_block));
		ptr->size = next_size;
		block->next = ptr;
		block->size = size;
		return (true);
	}
	else
		return (false);
}


static void	*expand_block_or_malloc(void *ptr, t_block *block, size_t size)
{
	void	*ret;

	if (try_reduce_next_block(block, size) == true)
		return (ptr);
	else
	{
		if ((ret = mutexed_malloc(size)) == NULL)
			return (NULL);
		ft_bzero(ret, size);
		ft_memmove(ret, ptr, block->size > size ? size : block->size);
		mutexed_free(ptr);
		return (ret);
	}
}

void		*mutexed_realloc(void *ptr, size_t size)
{
	t_zone	*zones[2];
	t_block	*blocks[2];

	if (find_block(ptr, zones, blocks) == false)
		return (NULL);
	if (blocks[0]->size == size) {
		return (ptr);
	}
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
	size_t	align;

	align = get_align();
	size = (size + align) & ~align;
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

void		*reallocf(void *ptr, size_t size)
{
	void	*ret;

	ret = realloc(ptr, size);
	if (ret == NULL && size != 0)
		free(ptr);
	return (ret);
}
