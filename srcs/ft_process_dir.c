/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_ls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:02:07 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 14:40:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_process_process_ls_directories(t_file *file, t_directory *dir,
			t_lflags *lflags)
{
	char	*full_path;
	int		ret;

	ret = 0;
	if (S_ISDIR(file->stat.st_mode) &&
			strcmp(file->name, ".") && strcmp(file->name, ".."))
	{
		if (!(full_path = ft_strjoin_3(dir->path, "/", file->name)))
			return (2);
		if (ft_process_ls_directory(lflags, full_path, file->name))
			ret = 1;
		free(full_path);
	}
	return (ret);
}

int		ft_process_ls_directories(t_tree *tree, t_directory *dir,
			t_lflags *lflags)
{
	int		ret;
	int		save;
	t_file	*file;

	ret = 0;
	if (tree != NULL)
	{
		if ((save = ft_process_ls_directories(tree->left, dir, lflags)) == 2)
			return (save);
		ret |= save;
		file = tree->content;
		if ((save = ft_process_process_ls_directories(file, dir, lflags)) == 2)
			return (2);
		ret |= save;
		if ((save = ft_process_ls_directories(tree->right, dir, lflags)) == 2)
			return (2);
		ret |= save;
	}
	return (ret);
}

int		ft_process_process_ls_directory(t_directory *directory, DIR *cd,
			char *full_path, t_lflags *lflags)
{
	int ret;

	ret = 0;
	if (ft_fill_dir_files_tree(directory, cd, full_path, lflags))
		return (1);
	if (ft_print_dir(directory, lflags))
		return (1);
	if (lflags->recursive)
	{
		lflags->verbose = 1;
		if (ft_process_ls_directories(directory->files, directory, lflags))
			ret = 1;
	}
	return (ret);
}

int		ft_process_ls_directory(t_lflags *lflags, char *full_path, char *path)
{
	t_directory		directory;
	DIR				*cd;
	int				ret;

	if (ft_init_directory(&directory, full_path))
		return (2);
	if (!(cd = opendir(full_path)))
	{
		if (ft_process_error_dir(full_path, path, lflags))
		{
			free(directory.path);
			return (2);
		}
		free(directory.path);
		return (1);
	}
	if ((ret = ft_process_process_ls_directory(&directory, cd,
				full_path, lflags)) == 2)
	{
		ft_free_directory(&directory, lflags);
		return (2);
	}
	ft_free_directory(&directory, lflags);
	closedir(cd);
	return (ret);
}
