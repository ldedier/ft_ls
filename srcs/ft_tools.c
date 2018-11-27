/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/22 19:41:05 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_free_file(void *f, size_t size)
{
	t_file *file;

	(void)size;
	file = f;
	free(file->name);
	free(file->user);
	free(file->group);
	if (file->destination)
		free(file->destination);
	free(file);
}

void	ft_free_directory(t_directory *directory, size_t size)
{
	(void)size;
	free(directory->path);
	free(directory);
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

void	ft_sort_errors(t_list **errors)
{
	ft_lst_mergesort(errors, &ft_sort_lexicographic_err, 0);
}

void	ft_sort_files_list(t_list **files, t_lflags *lflags)
{
	if (lflags->sort_format == LEXICOGRAPHIC)
		ft_lst_mergesort(files, &ft_sort_lexicographic, lflags->reverse_sort);
	else if (lflags->sort_format == TIME_MODIFIED)
		ft_lst_mergesort(files, &ft_sort_modification_time, lflags->reverse_sort);
	else if (lflags->sort_format == LAST_ACCESS)
		ft_lst_mergesort(files, &ft_sort_last_access, lflags->reverse_sort);
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

int		ft_update_directory_stats(t_file *file, t_directory *directory)
{
	struct passwd	*pwd;
	struct group	*group;
	off_t			ret;

	if (!(pwd = getpwuid(file->stat.st_uid)))
		return (1);
	if (!(group = getgrgid(file->stat.st_gid)))
		return (1);
	if (!(file->user = ft_strdup(pwd->pw_name)))
		return (1);
	if (!(file->group = ft_strdup(group->gr_name)))
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

int		ft_print_directories(t_list *directories, t_lflags *lflags)
{
	t_list *ptr;
	t_file *directory;
	int		ret;

	ret = 0;
	ptr = directories;
	while (ptr != NULL)
	{
		directory = (t_file *)(ptr->content);
		if (ft_process_ls_directory(lflags, directory->name))
			ret = 1;
		ptr = ptr->next;
	}
	return (ret);
}
