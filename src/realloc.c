/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 17:25:29 by marvin            #+#    #+#             */
/*   Updated: 2020/06/21 02:50:03 by marvin           ###   ########.fr       */
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
		ptr->free = true;
		ptr->size = block->size - (sizeof(t_block) + size);
		ptr->next = block->next;
		block->size = size;
		block->next = ptr;
	}
}

static void	reduce_next_block(t_block *block, size_t size)
{
	t_block *ptr;
	size_t	next_size;

	if (block->size + block->next->size > size + sizeof(t_block))
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
		ft_bzero(ret, size);
		ft_memmove(ret, ptr, block->size);
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
		ft_memcpy(ret, ptr, zones[0]->size - sizeof(t_zone) - sizeof(t_block));
		mutexed_free(ptr);
		return (ret);
	}
	else if (blocks[0]->size == size)
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

	//size = (size + 15) & ~15;
	/*ft_putstr("REALLOC ");
	put_size_t_nbr((size_t)ptr, 16);
	ft_putstr(": ");
	put_size_t_nbr(size, 10);
	ft_putstr(" bytes\n");*/
	/*if (ptr != NULL)
		show_alloc_mem();*/
	check_incoherence("deb realloc");
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
	/*if (ptr != NULL)
	{
		ft_putstr("END REALLOC (");
		put_size_t_nbr((size_t)ptr, 16);
		ft_putstr(" ");
		ft_putnbr(size);
		ft_putstr(") ret = ");
		put_size_t_nbr((size_t)ret, 16);
		ft_putstr("\n");
	}
	if (ret == NULL)
		show_alloc_mem();*/
	check_incoherence("fin realloc");
	return (ret);
}
