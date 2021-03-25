/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eduwer <eduwer@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/12 21:44:05 by marvin            #+#    #+#             */
/*   Updated: 2021/03/25 22:54:06 by eduwer           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MALLOC_H
# define MALLOC_H
# include <stdlib.h>
# include <sys/mman.h>
# include <libft.h>
# include <pthread.h>
# include <unistd.h>
# define TINY_BLOC_BYTES 128
# define SMALL_BLOC_BYTES 2048

typedef enum			e_bool {
	false,
	true
}						t_bool;

typedef enum			e_type {
	TINY,
	SMALL,
	LARGE
}						t_type;

typedef struct			s_block {
	struct s_block	*next;
	size_t			size;
	t_bool			free;
}						t_block;

typedef struct			s_zone {
	struct s_zone	*next;
	size_t			size;
	t_type			type;
}						t_zone;

extern t_zone			*g_zones;
extern pthread_mutex_t	g_mutex;

void					*malloc(size_t size);
void					*calloc(size_t n, size_t size);
void					free(void *ptr);
void					*realloc(void *ptr, size_t size);
void					*reallocf(void *ptr, size_t size);
void					show_alloc_mem(void);
void					*mutexed_malloc(size_t size);
void					mutexed_free(void *ptr);
void					*shift_block(t_block *block);
void					*shift_zone(t_zone *zone);
size_t					get_nb_pages(t_type type, size_t size);
t_zone					*mmap_zone(t_type type, size_t nb_pages, \
							t_block **block);
t_bool					find_block(void *ptr, t_zone **ret_zone, \
							t_block **ret_block);
void					put_size_t_nbr(size_t nbr, size_t base);
void					check_incoherence(char *provenance);
void					log_print(char *format, ...);
size_t					get_align(void);
void					find_or_create_block(t_type type, size_t size, \
							t_zone **zone, t_block **block);
t_bool					claim_block(size_t size, t_zone *zone, \
							t_block **ret_block);
void					try_split_block(t_block *block, size_t size);
t_bool					try_reduce_next_block(t_block *block, size_t size);
void					shrink_block(t_block *block, size_t size);

#endif
