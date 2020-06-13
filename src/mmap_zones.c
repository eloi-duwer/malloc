/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mmap_zones.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 22:03:35 by marvin            #+#    #+#             */
/*   Updated: 2020/06/13 01:59:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

static void		append_zone(t_zone *zone)
{
	t_zone	*ptr;

	if (g_zones == NULL)
		g_zones = zone;
	else
	{
		ptr = g_zones;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = zone;
	}
}

t_zone			*mmap_large_zone(size_t size)
{
	t_zone	*ret;
	size_t	size_map;

	size_map = ((size + sizeof(t_zone)) / getpagesize() + 1) * getpagesize();
	ret = (t_zone *)mmap(NULL, size_map, \
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ret == MAP_FAILED)
		return (NULL);
	ret->next = NULL;
	ret->type = LARGE;
	ret->size = size_map;
	append_zone(ret);
	return (ret);
}

static size_t	get_nb_pages(t_type type)
{
	size_t	pagesize;
	size_t	bytes_bloc;

	pagesize = getpagesize();
	if (type == TINY)
		bytes_bloc = TINY_BLOC_BYTES;
	else
		bytes_bloc = SMALL_BLOC_BYTES;
	return (((bytes_bloc + sizeof(t_block)) * 128 + sizeof(t_zone)) \
		/ pagesize + 1);
}

t_zone			*mmap_tiny_small_zone(t_type type, t_block **block_ptr)
{
	t_zone	*ret;
	t_block	*block;
	size_t	size;

	size = get_nb_pages(type) * getpagesize();
	ret = (t_zone *)mmap(NULL, size, \
		PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ret == MAP_FAILED)
		return (NULL);
	ret->next = NULL;
	ret->type = TINY;
	ret->size = size;
	block = (t_block *)shift_zone(ret);
	block->size = size - sizeof(t_zone) - sizeof(t_block);
	block->free = true;
	block->next = NULL;
	append_zone(ret);
	*block_ptr = block;
	return (ret);
}
