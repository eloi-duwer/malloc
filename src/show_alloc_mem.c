/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show_alloc_mem.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/15 18:45:25 by marvin            #+#    #+#             */
/*   Updated: 2020/06/27 02:09:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <signal.h>
#include <stdarg.h>

static char		conv_to_char(size_t n)
{
	if (n <= 9)
		return ((char)(n + 48));
	else if (n <= 15)
		return ((char)(n + 87));
	return ('?');
}

static void		put_size_t_nbr_recursive(size_t nbr, size_t base)
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
	ft_putstr("Block ");
	put_size_t_nbr((size_t)ptr, 16);
	ft_putstr(": ");
	put_size_t_nbr((size_t)shift_block(ptr), 16);
	ft_putstr(" - ");
	put_size_t_nbr((size_t)(shift_block(ptr) + ptr->size), 16);
	ft_putstr(" : ");
	put_size_t_nbr(ptr->size, 10);
	ft_putstr(" octets");
	if (ptr->next && ptr->size != (size_t)((void *)ptr->next - shift_block(ptr)))
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

size_t			print_zone(t_zone *zone)
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
			break;
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

void			check_incoherence(char *provenance)
{
	t_zone	*zone;
	t_block	*block;
	size_t	size;

	zone = g_zones;
	while (zone != NULL)
	{
		size = 0;
		block = shift_zone(zone);
		while (block != NULL)
		{
			size += block->size + sizeof(t_block);
			if (size > zone->size)
			{
				ft_putstr("OVERFLOW IN ZONE SIZE FROM OPERATION ");
				ft_putstr(provenance);
				ft_putstr("\nStack:\n");
				show_alloc_mem();
				exit(1);
			}
			if (block->next != NULL && block->size != (size_t)((void *)block->next - shift_block(block)))
			{
				ft_putstr("INCOHERENCE FROM OPERATION ");
				ft_putstr(provenance);
				ft_putstr("\nblock in cause: ");
				put_size_t_nbr((size_t)block, 16);
				ft_putstr("\npid: ");
				put_size_t_nbr(getpid(), 10);
				ft_putstr("\nStack:\n");
				show_alloc_mem();
				exit(1);
			}
			block = block->next;
		}
		if (size + sizeof(t_zone) != zone->size)
		{
			log_print("from operation %s\nTotal Block size in zone %p \
do not match:\nexpected size is %u, block sum is %u\n", \
				provenance, zone, zone->size, size + sizeof(t_zone));
			print_zone(zone);
			exit(1);
		}
		zone = zone->next;
	}
}

void	log_print(char *format, ...)
{
	va_list	ap;
	char	buff[11];
	char	*ret;
	size_t	len;
	size_t	i;

	va_start(ap, format);
	len = ft_strlen(format);
	i = 0;
	while (i < len)
	{
		ft_bzero(buff, 11);
		if ((ret = ft_memccpy(buff, &format[i], '%', len - i > 10 ? 10 : len - i)) == NULL)
		{
			ft_putstr(buff);
			i += 10;
		}
		else
		{
			*(ret - 1) = '\0';
			ft_putstr(buff);
			if (*(&format[i] + (ret - buff)) == '%')
				ft_putchar('%');
			else if (*(&format[i] + (ret - buff)) == 'u')
				put_size_t_nbr(va_arg(ap, size_t), 10);
			else if (*(&format[i] + (ret - buff)) == 'p')
				put_size_t_nbr(va_arg(ap, size_t), 16);
			else if (*(&format[i] + (ret - buff)) == 's')
				ft_putstr(va_arg(ap, char *));
			else
			{
				ft_putchar('%');
				ft_putchar(*(&format[i] + (ret - buff)));
			}
			i += ret - buff + 1;
		}
	}

}

size_t	get_align(void)
{
	return (2 * sizeof(size_t) - 1);
}
