/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:22:21 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 14:49:28 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_file		*ft_new_file(struct stat stat, char *name)
{
	t_file	*res;

	if (!(res = (t_file *)(malloc(sizeof(t_file)))))
		return (NULL);
	if (!(res->name = ft_strdup(name)))
	{
		free(res);
		return (NULL);
	}
	res->stat = stat;
	res->user = NULL;
	res->group = NULL;
	res->destination = NULL;
	return (res);
}

t_file		*ft_process_fill_files_tree(struct stat stat, char *name,
			t_tree **tree, t_lflags *lflags)
{
	t_file			*file;

	if (!(file = ft_new_file(stat, name)))
		return (NULL);
	if (ft_tree_add_sorted(tree, file, lflags->sort_func))
	{
		free(file->name);
		free(file);
		return (NULL);
	}
	return (file);
}

int			ft_process_fill_dir_files_tree(struct stat stat, char *name,
			t_directory *directory, t_lflags *lflags)
{
	t_file *file;

	if (!(file = ft_process_fill_files_tree(stat, name,
			&(directory->files), lflags)))
		return (1);
	if (lflags->display_format == LONG)
	{
		if (ft_update_directory_stats(file, directory))
			return (1);
	}
	else if (lflags->display_format == COLUMN)
		ft_update_directory_col_stats(file, directory);
	return (0);
}

int			ft_fill_dir_files_tree(t_directory *directory, DIR *current_dir,
			char *path, t_lflags *lflags)
{
	struct dirent	*entry;

	if (directory->has_stat && directory->stat.st_ino == lflags->dev_fd_ino)
	{
		if (ft_add_entry(directory, ".", path, lflags))
			return (1);
		if (ft_add_entry(directory, "..", path, lflags))
			return (1);
	}
	while ((entry = readdir(current_dir)) != NULL)
	{
		if (ft_add_entry(directory, entry->d_name, path, lflags))
			return (1);
	}
	return (0);
}
