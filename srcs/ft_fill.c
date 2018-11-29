/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:22:21 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 00:42:02 by ldedier          ###   ########.fr       */
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

int			ft_should_add_entry(struct dirent entry, t_lflags *lflags)
{
	if (!lflags->all && entry.d_name[0] == '.' && !(lflags->dir_as_file &&
		!ft_strcmp((char *)entry.d_name, ".")))
		return (0);
	else if (lflags->dir_as_file &&
		ft_strcmp((char *)entry.d_name, "."))
		return (0);
	return (1);
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
	struct stat		stat;
	struct dirent	*entry;
	char			*full_path;

	while ((entry = readdir(current_dir)) != NULL)
	{
		if (ft_should_add_entry(*entry, lflags))
		{
			if (!(full_path = ft_strjoin_3(path, "/", entry->d_name)))
				return (1);
			if (lstat(full_path, &stat) == -1)
			{
				free(full_path);
				return (0);
			}
			free(full_path);
			if (ft_process_fill_dir_files_tree(stat, entry->d_name,
						directory, lflags))
				return (1);
		}
	}
	return (0);
}
