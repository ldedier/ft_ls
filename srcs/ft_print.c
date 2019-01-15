/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 14:03:09 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/05 20:48:33 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_print_long_format(t_directory *directory, t_file *file,
			t_lflags *lflags)
{
	char	str[12];
	char	*full_path;

	if (!(full_path = ft_get_full_path(directory, file)))
		return (1);
	if (ft_put_file_symbol(file, str, full_path))
		return (ft_free_turn(full_path, 1));
	ft_put_file_permissions(file, str);
	ft_put_file_extended_attribute_or_acls(full_path, str);
	str[11] = '\0';
	if (lflags->show_owner)
		ft_printf("%s %*d %-*s  %-*s ", str, directory->max_links_length,
			file->stat.st_nlink, directory->max_length_uid, file->user,
				directory->max_length_gid, file->group);
	else
		ft_printf("%s %*d %-*s ", str, directory->max_links_length,
			file->stat.st_nlink, directory->max_length_gid, file->group);
	ft_print_size(directory, file);
	if (ft_print_time(file, lflags))
		return (ft_free_turn(full_path, 1));
	ft_print_name(file, lflags, 1);
	return (ft_free_turn(full_path, 0));
}

int		ft_print_dir_file_long(t_directory *directory,
			t_file *file, t_lflags *lflags)
{
	if (ft_print_long_format(directory, file, lflags))
		return (1);
	return (0);
}

int		ft_process_print_long(t_tree *tree, t_directory *dir, t_lflags *lflags)
{
	t_file		*file;

	if (tree != NULL)
	{
		if (ft_process_print_long(tree->left, dir, lflags))
			return (1);
		file = tree->content;
		if (ft_print_dir_file_long(dir, file, lflags))
			return (1);
		if (ft_process_print_long(tree->right, dir, lflags))
			return (1);
	}
	return (0);
}

int		ft_print_dir(t_directory *directory, t_lflags *lflags)
{
	if (lflags->first_entry)
		lflags->first_entry = 0;
	else
		ft_printf("\n");
	if (directory->path && lflags->verbose)
		ft_printf("%s:\n", directory->path);
	ft_update_directory_data(directory);
	if (directory->path && lflags->display_format == LONG && directory->files)
		ft_printf("total %d\n", directory->total_blocks);
	if (lflags->display_format == LONG)
		return (ft_process_print_long(directory->files, directory, lflags));
	else
		return (ft_print_short(directory->files, directory, lflags));
}
