/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/06 17:31:54 by eduwer            #+#    #+#             */
/*   Updated: 2016/11/09 17:43:43 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <string.h>
#include <libft.h>

void	ft_putstr_fd(char const *s, int fd)
{
	size_t	len;

	len = ft_strlen(s);
	write(fd, s, len);
}
