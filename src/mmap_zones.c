/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap_zones.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 22:03:35 by marvin            #+#    #+#             */
/*   Updated: 2021/01/21 20:36:24 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

size_t		get_nb_pages(t_type type, size_t size)
{
	size_t	bytes_bloc;
	size_t	ret;

	if (type == LARGE)
		return (((size + sizeof(t_zone) + sizeof(t_block)) \
			/ getpagesize() + 1));
	else if (type == TINY)
		bytes_bloc = TINY_BLOC_BYTES;
	else
		bytes_bloc = SMALL_BLOC_BYTES;
	ret = (((bytes_bloc + sizeof(t_block)) * 128 + sizeof(t_zone)) \
		/ getpagesize() + 1);
	return (ret);
}

static void	append_zone(t_zone *zone)
{
	t_zone	*ptr;

	if (g_zones == NULL)
		g_zones = zone;
	else if (zone < g_zones)
	{
		zone->next = g_zones;
		g_zones = zone;
	}
	else
	{
		ptr = g_zones;
		while (ptr->next != NULL && zone > ptr->next)
			ptr = ptr->next;
		zone->next = ptr->next;
		ptr->next = zone;
	}
}

t_zone		*mmap_zone(t_type type, size_t nb_pages, t_block **block_ptr)
{
	t_zone	*ret;
	t_block	*block;
	size_t	size;

	size = nb_pages * getpagesize();
	ret = (t_zone *)mmap(NULL, size, \
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ret == MAP_FAILED)
		return (NULL);
	ret->next = NULL;
	ret->type = type;
	ret->size = size;
	block = (t_block *)shift_zone(ret);
	block->size = size - sizeof(t_zone) - sizeof(t_block);
	block->free = true;
	block->next = NULL;
	append_zone(ret);
	*block_ptr = block;
	return (ret);
}
