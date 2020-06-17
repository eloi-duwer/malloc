/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/07 22:48:48 by marvin            #+#    #+#             */
/*   Updated: 2020/06/16 20:53:34 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <malloc.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	void *ptrs[10];

	ptrs[0] = malloc(10);
	ptrs[1] = malloc(20);
	ptrs[2] = malloc(100);
	ptrs[3] = malloc(10);

	free(ptrs[2]);
	show_alloc_mem();
	ft_memcpy(ptrs[0], "123456789\0", 10);
	ft_printf("TRUC %s\n", ptrs[0]);
	ptrs[4] = realloc(ptrs[0], 15);
	ft_printf("MACHIN %s\n", ptrs[4]);
	return (0);
}
