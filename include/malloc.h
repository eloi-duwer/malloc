/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 21:44:05 by marvin            #+#    #+#             */
/*   Updated: 2020/06/14 17:33:05 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MALLOC_H__
# define __MALLOC_H__
# include <stdlib.h>
# include <sys/mman.h>
# include <libft.h>
# include <pthread.h>
# include <unistd.h>
# define false 0
# define true 1
# define TINY_BLOC_BYTES 128
# define SMALL_BLOC_BYTES 4096 

typedef char	t_bool;

typedef enum	e_type {
	TINY,
	SMALL,
	LARGE
}				t_type;

typedef struct	s_block {
	struct s_block	*next;
	size_t			size;
	t_bool			free;
}				t_block;

typedef struct	s_zone {
	struct s_zone	*next;
	size_t			size;
	t_type			type;
}				t_zone;

extern t_zone			*g_zones;
extern pthread_mutex_t	g_mutex;

void	*malloc(size_t size);
void	free(void *ptr);
void	*realloc(void *ptr, size_t size);
void	*shift_block(t_block *block);
void	*shift_zone(t_zone *zone);
t_zone	*mmap_large_zone(size_t size);
t_zone	*mmap_tiny_small_zone(t_type type, t_block **block);
t_bool	find_block(void *ptr, t_zone **ret_zone, t_block **ret_block);

#endif
