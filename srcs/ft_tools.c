/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 12:36:09 by ldedier          ###   ########.fr       */
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
	if (lflags->long_format)
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

void	ft_affich_stat(struct stat sb)
{
	printf("\tinode: %llu\n", sb.st_ino);
	printf("\towner: %u \n", sb.st_uid);
	printf("\tgroup: %u \n", sb.st_gid);
	printf("\tperms: %o\n", sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
	printf("\tlinks: %d\n", sb.st_nlink);
	printf("\tsize: %lld\n", sb.st_size); /* you may use %lld */
	printf("\tatime: %s", ctime(&sb.st_atimespec.tv_sec));
	printf("\tmtime: %s", ctime(&sb.st_mtimespec.tv_sec));
	printf("\tctime: %s", ctime(&sb.st_ctimespec.tv_sec));
	printf("\n");
}

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

int		ft_print_directories(t_tree *tree, t_lflags *lflags)
{
	int			ret;
	int			save;
	t_file		*directory;

	ret = 0;
	if (tree != NULL)
	{
		if ((save = ft_print_directories(tree->left, lflags)) == 2)
			return (2);
		ret |= save;
		directory = (t_file *)(tree->content);
		if ((save = ft_process_ls_directory(lflags, directory->name,
				directory->name)) == 2)
			return (2);
		ret |= save;
		if ((save = ft_print_directories(tree->right, lflags)) == 2)
			return (2);
		ret |= save;
	}
	return (ret);
}
