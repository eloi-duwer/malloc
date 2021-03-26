/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 18:45:25 by marvin            #+#    #+#             */
/*   Updated: 2021/03/26 13:00:30 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

void	show_alloc_mem(void)
{
	t_zone	*zone;
	size_t	total;

	pthread_mutex_lock(&g_mutex);
	total = 0;
	zone = g_zones;
	while (zone != NULL)
	{
		total += print_zone(zone, false);
		if (zone->next == zone)
			return ;
		zone = zone->next;
	}
	pthread_mutex_unlock(&g_mutex);
	ft_putstr("Total : ");
	put_size_t_nbr(total, 10);
	ft_putstr(" bytes\n");
}

void	show_alloc_mem_ex(void)
{
	t_zone	*zone;
	size_t	total;

	pthread_mutex_lock(&g_mutex);
	total = 0;
	zone = g_zones;
	while (zone != NULL)
	{
		total += print_zone(zone, true);
		if (zone->next == zone)
			return ;
		zone = zone->next;
	}
	pthread_mutex_unlock(&g_mutex);
	ft_putstr("Total : ");
	put_size_t_nbr(total, 10);
	ft_putstr(" bytes\n");
}

size_t	get_align(void)
{
	return (2 * sizeof(size_t) - 1);
}
