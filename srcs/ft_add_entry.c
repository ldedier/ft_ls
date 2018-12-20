/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_add_entry.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 14:47:12 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 14:47:14 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int			ft_should_add_entry(char *file_name, t_lflags *lflags)
{
	if (!lflags->all && file_name[0] == '.' && !(lflags->dir_as_file &&
		!ft_strcmp(file_name, ".")))
		return (0);
	else if (lflags->dir_as_file &&
		ft_strcmp(file_name, "."))
		return (0);
	return (1);
}

int			ft_add_entry(t_directory *directory, char *entry, char *path,
				t_lflags *lflags)
{
	char			*full_path;
	struct stat		stat;

	if (ft_should_add_entry(entry, lflags))
	{
		if (!(full_path = ft_strjoin_3(path, "/", entry)))
			return (1);
		if (lstat(full_path, &stat) == -1)
		{
			free(full_path);
			return (0);
		}
		free(full_path);
		if (ft_process_fill_dir_files_tree(stat, entry,
					directory, lflags))
			return (1);
	}
	return (0);
}
