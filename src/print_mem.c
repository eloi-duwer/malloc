/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_mem.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/26 12:59:39 by eduwer            #+#    #+#             */
/*   Updated: 2021/03/26 13:58:12 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

static void		print_chars(unsigned char *ptr, size_t size)
{
	size_t i;

	ft_putstr("  |");
	i = 0;
	while (i != size)
	{
		if (ft_isprint(ptr[i]))
			ft_putchar(ptr[i]);
		else
			ft_putchar('.');
		i++;
	}
	ft_putstr("|\n");
}

static void		print_mem_content(t_block *block)
{
	unsigned char	*ptr;
	size_t			size;
	size_t			line_size;

	ptr = shift_block(block);
	size = 0;
	line_size = get_align() + 1;
	while (size != block->size)
	{
		if (size % line_size == 0)
			put_size_t_nbr((size_t)ptr, 16);
		ft_putchar(' ');
		if (size % 8 == 0)
			ft_putchar(' ');
		put_hex_char((size_t)ptr[0]);
		if ((size + 1) % line_size == 0)
			print_chars(ptr - (line_size - 1), line_size);
		ptr++;
		size++;
	}
}

static size_t	print_block(t_block *ptr, t_bool print_mem)
{
	ft_putstr("Block ");
	put_size_t_nbr((size_t)ptr, 16);
	ft_putstr(": ");
	put_size_t_nbr((size_t)shift_block(ptr), 16);
	ft_putstr(" - ");
	put_size_t_nbr((size_t)(shift_block(ptr) + ptr->size), 16);
	ft_putstr(" : ");
	put_size_t_nbr(ptr->size, 10);
	ft_putstr(" bytes");
	if (ptr->free)
		ft_putstr(" (free)");
	if (ptr->next && ptr->size != \
		(size_t)((void *)ptr->next - shift_block(ptr)))
	{
		ft_putstr(" INCOHERENCE diff = ");
		put_size_t_nbr((void *)ptr->next - shift_block(ptr), 10);
		ft_putstr(")");
		return (ptr->size);
	}
	ft_putstr("\n");
	if (print_mem == true)
		print_mem_content(ptr);
	return (ptr->size);
}

size_t			print_zone(t_zone *zone, t_bool print_mem)
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
	ft_putstr(" bytes\n");
	ret = 0;
	ptr = shift_zone(zone);
	while (ptr != NULL)
	{
		if (ptr->free == false)
			ret += print_block(ptr, print_mem);
		if (ptr == ptr->next)
			break ;
		ptr = ptr->next;
	}
	return (ret);
}
