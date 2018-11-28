/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 14:44:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_off_tlen(off_t val)
{
	int res;

	res = 1;
	while (val / 10 != 0)
	{
		val /= 10;
		res++;
	}
	return (res);
}

int		ft_nlink_tlen(nlink_t val)
{
	int res;

	res = 1;
	while (val / 10 != 0)
	{
		val /= 10;
		res++;
	}
	return (res);
}

void	ft_update_directory_data(t_directory *dir)
{
	dir->max_links_length = ft_nlink_tlen(dir->max_links);
	dir->max_size_length = ft_off_tlen(dir->max_size);
}

int		ft_populate_usr_gr(t_file *file)
{
	struct passwd	*pwd;
	struct group	*group;

	if (!(pwd = getpwuid(file->stat.st_uid)))
	{
		if (!(file->user = ft_itoa(file->stat.st_uid)))
			return (1);
	}
	else if (!(file->user = ft_strdup(pwd->pw_name)))
		return (1);
	if (!(group = getgrgid(file->stat.st_gid)))
	{
		if (!(file->group = ft_itoa(file->stat.st_gid)))
			return (1);
	}
	else if (!(file->group = ft_strdup(group->gr_name)))
		return (1);
	return (0);
}

int		ft_update_directory_stats(t_file *file, t_directory *directory)
{
	off_t			ret;

	if (ft_populate_usr_gr(file))
		return (1);
	directory->total_blocks += file->stat.st_blocks;
	if ((ret = ft_strlen(file->user)) > directory->max_length_uid)
		directory->max_length_uid = ret;
	if ((ret = ft_strlen(file->group)) > directory->max_length_gid)
		directory->max_length_gid = ret;
	if (directory->max_links < file->stat.st_nlink)
		directory->max_links = file->stat.st_nlink;
	if ((ret = file->stat.st_size) >
			directory->max_size)
		directory->max_size = ret;
	if (S_ISBLK(file->stat.st_mode) || S_ISCHR(file->stat.st_mode))
		directory->has_devices = 1;
	return (0);
}
