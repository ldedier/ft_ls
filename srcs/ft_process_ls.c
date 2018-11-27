/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_ls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:02:07 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 00:21:03 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_fill_arg_file(char *arg, struct stat *st, t_env *e, t_lflags *lflags)
{
	int ret;
	
	ret = 0;
	if (e->stat_func(arg, st) == -1)
	{
		ret = 1;
		if (ft_fill_path_error_tree(arg, &(e->errors)))
			return (2);
	}
	else
	{
		if (!lflags->dir_as_file && (S_ISDIR(st->st_mode)))
		{
			if (!ft_process_fill_files_tree(*st, arg,
						&(e->directories), lflags))
				return (2);
		}
		else
		{
			if (ft_process_fill_dir_files_tree(*st, arg,
						&(e->regular_file_group), lflags))
				return (2);
		}
	}
	return (ret);
}

void	ft_free_env(t_env *e, t_lflags *lflags)
{
	ft_tree_del_value(&(e->errors));
	ft_free_files_tree(&(e->directories), lflags);
	ft_free_files_tree(&(e->regular_file_group.files), lflags);
}

int		ft_process_ls(t_lflags *lflags, int i, int argc, char **argv)
{
	t_env		e;
	struct stat	st;
	int 		ret;

	ft_init_env(&e, lflags);
	ret = 0;
	if (i < argc - 1)
		lflags->verbose = 1;
	while (i < argc)
	{
		if ((ret = ft_fill_arg_file(argv[i], &st, &e, lflags)) == 2)
		{
			ft_free_env(&e, lflags);
			return (2);
		}
		i++;
	}
	ft_print_errors(e.errors);
	if (e.regular_file_group.files)
		ret  = ret | ft_print_dir(&(e.regular_file_group), lflags);
	ret = ret | ft_print_directories(e.directories, lflags);
	ft_free_env(&e, lflags);
	return (ret);
}

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

int		ft_process_ls_directory(t_lflags *lflags, char *full_path, char *path)
{
	t_directory		directory;
	DIR				*cd;
	int				ret;
	
	ret = 0;
	if (ft_init_directory(&directory, full_path))
		return (2);
	if (!(cd = opendir(full_path)))
	{
		ft_process_error_dir(full_path, path, lflags);
		free(directory.path);
		return (1);
	}
	if ((ret = ft_fill_dir_files_tree(&directory, cd, full_path, lflags)) == 2)
		return (2);
	ft_print_dir(&directory, lflags);
	if (lflags->recursive)
	{
		lflags->verbose = 1;
		if (ft_process_ls_directories(directory.files, &directory, lflags))
			ret = 1;
	}
	ft_free_directory(&directory, lflags);
	closedir(cd);
	return (ret);
}
