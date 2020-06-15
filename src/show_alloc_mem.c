/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 18:45:25 by marvin            #+#    #+#             */
/*   Updated: 2020/06/16 01:31:53 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

static	t_zone	*get_next_zone(t_zone *zone)
{
	t_zone	*ret;
	t_zone	*ptr;

	ret = NULL;
	ptr = g_zones;
	while (ptr != NULL)
	{
		if (ptr > zone)
		{
			if (ret == NULL || ptr < ret)
				ret = ptr;
		}
		ptr = ptr->next;
	}
	return (ret);
}

static char		conv_to_char(size_t n)
{
	if (n <= 9)
		return ((char)(n + 48));
	else if (n <= 15)
		return ((char)(n + 87));
	return ('?');
}

void		put_size_t_nbr_recursive(size_t nbr, size_t base)
{
	if (nbr > base - 1)
		put_size_t_nbr_recursive(nbr / base, base);
	nbr %= base;
	ft_putchar(conv_to_char(nbr));
}

void		put_size_t_nbr(size_t nbr, size_t base)
{
	if (base == 16)
		ft_putstr("0x");
	put_size_t_nbr_recursive(nbr, base);
}

size_t			print_block(t_block *ptr)
{
	put_size_t_nbr((size_t)shift_block(ptr), 16);
	ft_putstr(" - ");
	put_size_t_nbr((size_t)(shift_block(ptr) + ptr->size), 16);
	ft_putstr(" : ");
	put_size_t_nbr(ptr->size, 10);
	ft_putstr(" octets (diff: ");
	put_size_t_nbr((void *)ptr->next - shift_block(ptr), 10);
	ft_putstr(")\n");
	return (ptr->size);
}

size_t			print_large_zone(t_zone *zone)
{
	ft_putstr("LARGE : ");
	put_size_t_nbr((size_t)zone, 16);
	ft_putchar('\n');
	put_size_t_nbr((size_t)shift_zone(zone), 16);
	ft_putstr(" - ");
	put_size_t_nbr((size_t)((void *)zone + zone->size), 16);
	ft_putstr(" : ");
	put_size_t_nbr(zone->size - sizeof(t_zone), 10);
	ft_putstr(" octets\n");
	return (zone->size - sizeof(t_zone));
}

size_t			print_zone(t_zone *zone)
{
	size_t	ret;
	t_block	*ptr;

	if (zone->type == TINY)
		ft_putstr("TINY  : ");
	else if (zone->type == SMALL)
		ft_putstr("SMALL : ");
	else
		return (print_large_zone(zone));
	put_size_t_nbr((size_t)zone, 16);
	ft_putchar('\n');
	ret = 0;
	ptr = shift_zone(zone);
	while (ptr != NULL)
	{
		if (ptr->free == false)
			ret += print_block(ptr);
		ptr = ptr->next;
	}
	return (ret);
}

void			show_alloc_mem(void)
{
	t_zone	*zone;
	size_t	total;

	pthread_mutex_lock(&g_mutex);
	total = 0;
	zone = (t_zone *)0;
	while ((zone = get_next_zone(zone)) != NULL)
		total += print_zone(zone);
	pthread_mutex_unlock(&g_mutex);
	ft_putstr("Total : ");
	put_size_t_nbr(total, 10);
	ft_putstr(" octets\n");
}
