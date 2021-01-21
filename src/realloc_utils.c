/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 20:42:57 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/21 20:53:51 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

void	shrink_block(t_block *block, size_t size)
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

t_bool	try_reduce_next_block(t_block *block, size_t size)
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
