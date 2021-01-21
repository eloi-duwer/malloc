/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put_size_t_nbr.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 20:47:45 by eduwer            #+#    #+#             */
/*   Updated: 2021/01/21 20:53:37 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

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

void			put_size_t_nbr(size_t nbr, size_t base)
{
	if (base == 16)
		ft_putstr("0x");
	put_size_t_nbr_recursive(nbr, base);
}
