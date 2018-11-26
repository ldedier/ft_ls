/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_ls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:02:07 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/23 15:02:07 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_process_ls(t_lflags *lflags, int i, int argc, char **argv)
{
	t_env		e;
	struct stat	st;
	int 		ret;
	int			(*stat_func)(const char *restrict , struct stat *restrict);

	ft_init_env(&e);
	ret = 0;
	if (lflags->long_format)
		stat_func = lstat;
	else
		stat_func = stat;
	if (i < argc - 1)
		lflags->verbose = 1;
	while (i < argc)
	{
		if (stat_func(argv[i], &st) == -1)
		{
			ret = 1;
			if (ft_fill_path_error_list(argv[i], &(e.errors)))
				return (1);
		}
		else
		{
			if (!lflags->dir_as_file && (S_ISDIR(st.st_mode)))
			{
				if (!ft_process_fill_files_list(st, argv[i], &(e.directories)))
					return (1);
			}
			else
			{
				if (ft_process_fill_dir_files_list(st, argv[i],
							&(e.regular_file_group), lflags))
					return (1);
			}
		}
		i++;
	}
	ft_sort_errors(&(e.errors));
	ft_sort_files_list(&(e.regular_file_group.files), lflags);
	ft_sort_files_list(&(e.directories), lflags);
	ft_print_errors(e.errors);
	if (e.regular_file_group.files)
		ret  = ret | ft_print_dir(&(e.regular_file_group), lflags);
	ret = ret | ft_print_directories(e.directories, lflags);
	return (ret);
}

int		ft_process_ls_directory(t_lflags *lflags, char *path)
{
	t_directory		directory;
	DIR				*current_dir;

	if (ft_init_directory(&directory, path))
		return (1);
	if (!(current_dir = opendir(path)))
	{
		ft_process_error_dir(path, lflags);
		free(directory.path);
		return (1);
	}
	if (ft_fill_dir_files_list(&directory, current_dir, path, lflags))
	{
		closedir(current_dir);
		ft_lstdel_value(&(directory.files));
		free(directory.path);
		return (1);
	}
	ft_sort_files_list(&(directory.files), lflags);
	ft_print_dir(&directory, lflags);
	ft_lstdel(&(directory.files), &ft_free_file);
	free(directory.path);
	closedir(current_dir);
	return (0);
}
