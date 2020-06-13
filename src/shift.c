/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shift.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 21:43:35 by marvin            #+#    #+#             */
/*   Updated: 2020/06/12 21:43:42 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>

void	*shift_block(t_block *block)
{
	return ((void *)block + sizeof(t_block));
}

void	*shift_zone(t_zone *zone)
{
	return ((void *)zone + sizeof(t_zone));
}
