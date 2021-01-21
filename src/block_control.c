/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   block_control.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 20:41:16 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/21 20:41:35 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

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
			return ;
	}
	else
	{
		*zone = g_zones;
		while (*zone != NULL)
		{
			if ((*zone)->type == type \
				&& claim_block(size, *zone, block) == true)
				return ;
			*zone = (*zone)->next;
		}
		if ((*zone = mmap_zone(type, get_nb_pages(type, size), block)) == NULL)
			return ;
	}
	claim_block(size, *zone, block);
}
