/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 00:42:35 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_free_file_long_format(void *f)
{
	t_file *file;

	file = (t_file *)f;
	free(file->name);
	if (file->user != NULL)
		free(file->user);
	if (file->group != NULL)
		free(file->group);
	if (file->destination != NULL)
		free(file->destination);
	free(file);
}

void	ft_free_file(void *f)
{
	t_file *file;

	file = (t_file *)f;
	free(file->name);
	if (file->destination != NULL)
		free(file->destination);
	free(file);
}

void	ft_free_files_tree(t_tree **files, t_lflags *lflags)
{
	if (lflags->display_format == LONG)
		ft_tree_del(files, &ft_free_file_long_format);
	else
		ft_tree_del(files, &ft_free_file);
}

void	ft_free_directory(t_directory *directory, t_lflags *lflags)
{
	if (directory->path)
		free(directory->path);
	ft_free_files_tree(&(directory->files), lflags);
}

void	ft_free_env(t_env *e, t_lflags *lflags)
{
	ft_tree_del_value(&(e->errors));
	ft_free_files_tree(&(e->directories), lflags);
	ft_free_files_tree(&(e->regular_file_group.files), lflags);
}
