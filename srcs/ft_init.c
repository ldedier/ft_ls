/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/22 19:41:05 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_init_env(t_env *e)
{
	e->directories = NULL;
	e->errors = NULL;
	ft_init_directory_no_path(&(e->regular_file_group));
}

void	ft_init_directory_no_path(t_directory *directory)
{
	directory->files = NULL;
	directory->path = NULL;
	directory->total_blocks = 0;
	directory->max_length_uid = 0;
	directory->max_length_gid = 0;
	directory->max_links = 0;
	directory->max_size = 0;
	directory->has_devices = 0;
}

int		ft_init_directory(t_directory *directory, char *path)
{
	ft_init_directory_no_path(directory);
	if (!(directory->path = ft_strdup(path)))
		return (1);
	return (0);
}

void	ft_init_lflags(t_lflags *lflags)
{
	lflags->sort_format = LEXICOGRAPHIC;
	lflags->reverse_sort = 0;
	lflags->long_format = 0;
	lflags->recursive = 0;
	lflags->all = 0;
	lflags->dir_as_file = 0;
	lflags->colored = 0;
	lflags->show_owner = 1;
	lflags->last_access_flag = 0;
	lflags->verbose = 0;
	lflags->first_entry = 1;
}
