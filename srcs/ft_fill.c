/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_fill.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:22:21 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/23 15:22:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_file	*ft_new_file(struct stat stat, char *name)
{
	t_file	*res;

	if (!(res = (t_file *)(malloc(sizeof(t_file)))))
		return (NULL);
	res->stat = stat;
	res->name = ft_strdup(name);
	return (res);
}

int		ft_should_add_entry(struct dirent entry, struct stat stat,
			t_lflags *lflags)
{
	(void)stat;

	if (!lflags->all && entry.d_name[0] == '.' && !(lflags->dir_as_file &&
		!ft_strcmp((char *)entry.d_name, ".")))
		return (0);
	else if (lflags->dir_as_file &&
		ft_strcmp((char *)entry.d_name, "."))
		return (0);
	return (1);
}

t_file		*ft_process_fill_files_list(struct stat stat, char *name,
			t_list **list)
{
	t_file			*file;

	if (!(file = ft_new_file(stat, name)))
	{
		ft_lstdel_value(list);
		return (NULL);
	}
	if (ft_add_to_list_ptr_back(list, file,
				sizeof(t_file)))
	{
		ft_lstdel_value(list);
		return (NULL);
	}
	return (file);
}

int		ft_process_fill_dir_files_list(struct stat stat, char *name,
			t_directory *directory, t_lflags *lflags)
{
	t_file *file;

	if (!(file = ft_process_fill_files_list(stat, name, &(directory->files))))
		return (1);
	if (lflags->long_format)
	{
		if (ft_update_directory_stats(file, directory))
			return (1);
	}
	return (0);
}

int		ft_fill_dir_files_list(t_directory *directory, DIR *current_dir,
			char *path, t_lflags *lflags)
{
	struct stat		stat;
	struct dirent	*entry;
	char			*full_path;

	while ((entry = readdir(current_dir)) != NULL)
	{
		if (!(full_path = ft_strjoin_3(path, "/", entry->d_name)))
			return (1);
		if (lstat(full_path, &stat) == -1)
		{
			free(full_path);
			perror(full_path);
			return (1);
		}
		free(full_path);
		if (ft_should_add_entry(*entry, stat, lflags))
		{
			if (ft_process_fill_dir_files_list(stat, entry->d_name,
					directory, lflags))
				return (1);
		}
	}
	return (0);
}