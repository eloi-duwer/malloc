/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 18:45:25 by marvin            #+#    #+#             */
/*   Updated: 2021/01/21 20:54:10 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

size_t	print_block(t_block *ptr)
{
	ft_putstr("Block ");
	put_size_t_nbr((size_t)ptr, 16);
	ft_putstr(": ");
	put_size_t_nbr((size_t)shift_block(ptr), 16);
	ft_putstr(" - ");
	put_size_t_nbr((size_t)(shift_block(ptr) + ptr->size), 16);
	ft_putstr(" : ");
	put_size_t_nbr(ptr->size, 10);
	ft_putstr(" octets");
	if (ptr->next && ptr->size != \
		(size_t)((void *)ptr->next - shift_block(ptr)))
	{
		ft_putstr(" INCOHERENCE diff = ");
		put_size_t_nbr((void *)ptr->next - shift_block(ptr), 10);
		ft_putstr(")");
	}
	if (ptr->free)
		ft_putstr(" (free)");
	ft_putstr("\n");
	return (ptr->size);
}

size_t	print_zone(t_zone *zone)
{
	size_t	ret;
	t_block	*ptr;

	if (zone->type == TINY)
		ft_putstr("TINY  : ");
	else if (zone->type == SMALL)
		ft_putstr("SMALL : ");
	else
		ft_putstr("LARGE : ");
	put_size_t_nbr((size_t)zone, 16);
	ft_putstr(" size: ");
	put_size_t_nbr(zone->size, 10);
	ft_putchar('\n');
	ret = 0;
	ptr = shift_zone(zone);
	while (ptr != NULL)
	{
		if (ptr->free == false)
			ret += print_block(ptr);
		if (ptr == ptr->next)
			break ;
		ptr = ptr->next;
	}
	return (ret);
}

void	show_alloc_mem(void)
{
	t_zone	*zone;
	size_t	total;

	pthread_mutex_lock(&g_mutex);
	total = 0;
	zone = g_zones;
	while (zone != NULL)
	{
		total += print_zone(zone);
		if (zone->next == zone)
			return ;
		zone = zone->next;
	}
	pthread_mutex_unlock(&g_mutex);
	ft_putstr("Total : ");
	put_size_t_nbr(total, 10);
	ft_putstr(" octets\n");
}

size_t	get_align(void)
{
	return (2 * sizeof(size_t) - 1);
}
