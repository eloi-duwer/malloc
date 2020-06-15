/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 22:48:48 by marvin            #+#    #+#             */
/*   Updated: 2020/06/16 01:32:36 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	void	*ptr;
	int i;

	i = 1;
	while (i < 100)
	{
		ptr = malloc(10 * i);
		if (i % 10 == 0)
			free(ptr);
		++i;
	}
	show_alloc_mem();
	return (0);
}
