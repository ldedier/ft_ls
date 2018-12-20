/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 14:41:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_init_env(t_env *e, t_lflags *lflags)
{
	e->directories = NULL;
	e->errors = NULL;
	ft_init_directory_no_path(&(e->regular_file_group));
	e->ret = 0;
	if (lflags->display_format == LONG)
		e->stat_func = lstat;
	else
		e->stat_func = stat;
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
	directory->name_max_length = 0;
	directory->has_devices = 0;
	directory->has_stat = 0;
}

int		ft_init_directory(t_directory *directory, char *path)
{
	ft_init_directory_no_path(directory);
	if (!(directory->path = ft_strdup(path)))
		return (1);
	if (stat(path, &(directory->stat)) != -1)
		directory->has_stat = 1;
	return (0);
}

void	ft_init_lflags(t_lflags *lflags)
{
	struct stat	dev_fd_stat;

	lflags->sort_format = LEXICOGRAPHIC;
	lflags->sort_func = &ft_sort_lexicographic;
	lflags->order = 1;
	lflags->display_format = COLUMN;
	lflags->recursive = 0;
	lflags->all = 0;
	lflags->dir_as_file = 0;
	lflags->colored = 0;
	lflags->show_owner = 1;
	lflags->last_access_flag = 0;
	lflags->verbose = 0;
	lflags->first_entry = 1;
	if (stat("/dev/fd", &(dev_fd_stat)) == -1)
		lflags->dev_fd_ino = 0;
	else
		lflags->dev_fd_ino = dev_fd_stat.st_ino;
}
