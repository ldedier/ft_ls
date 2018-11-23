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

static t_opt_func g_opt_arr[NB_CHARS] =
{
	['l'] = ft_opt_l,
	['R'] = ft_opt_r_maj,
	['a'] = ft_opt_a,
	['r'] = ft_opt_r,
	['t'] = ft_opt_t,
	['f'] = ft_opt_f,
	['u'] = ft_opt_u,
	['g'] = ft_opt_g,
	['d'] = ft_opt_d,
	['G'] = ft_opt_g_maj
};

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

int		ft_describe_options(char *str)
{
	int		i;

	if (str[0] != '-')
		return (0);
	i = 0;
	while (str[i])
	{
		if (i == 1)
			return (1);
		i++;
	}
	return (0);
}

int		ft_parse_options(char *str, t_lflags *lflags)
{
	int i;

	i = 1;
	while (str[i])
	{
		if (g_opt_arr[(int)str[i]] == NULL)
		{
			ft_dprintf(2, "ls: illegal option -- %c\n", str[i]);
			ft_dprintf(2, "usage: ls [-GRadfglrtu] [file ...]\n"); //to update?
			return (1);
		}
		g_opt_arr[(int)str[i]](lflags);
		i++;
	}
	return (0);
}

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

int		ft_get_size_length(int bytes)
{
	(void)bytes;
	return (0);
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

void	ft_put_file_symbol(t_file *file, char *str)
{
	if (S_ISREG(file->stat.st_mode))
		str[0] = '-';
	else if (S_ISDIR(file->stat.st_mode))
		str[0] = 'd';
	else if (S_ISLNK(file->stat.st_mode))
		str[0] = 'l';
	else if (S_ISSOCK(file->stat.st_mode))
		str[0] = 's';
	else if (S_ISFIFO(file->stat.st_mode))
		str[0] = 'p';
	else if (S_ISBLK(file->stat.st_mode))
		str[0] = 'b';
	else if (S_ISCHR(file->stat.st_mode))
		str[0] = 'c';
}

void	ft_put_file_permissions(t_file *file, char *str)
{
	str[1] = file->stat.st_mode & S_IRUSR ? 'r' : '-';
	str[2] = file->stat.st_mode & S_IWUSR ? 'w' : '-';
	str[3] = file->stat.st_mode & S_IXUSR ? 'x' : '-';
	str[4] = file->stat.st_mode & S_IRGRP ? 'r' : '-';
	str[5] = file->stat.st_mode & S_IWGRP ? 'w' : '-';
	str[6] = file->stat.st_mode & S_IXGRP ? 'x' : '-';
	str[7] = file->stat.st_mode & S_IROTH ? 'r' : '-';
	str[8] = file->stat.st_mode & S_IWOTH ? 'w' : '-';
	str[9] = file->stat.st_mode & S_IXOTH ? 'x' : '-';
}

void	ft_put_file_extended_attribute(t_file *file, char *str)
{
	int		listlen;
	char	list[XATTR_SIZE];

	listlen = listxattr(file->name, list, XATTR_SIZE,
		XATTR_SHOWCOMPRESSION);
	if (listlen)
		str[10] = '@';
	else
		str[10] = ' ';
	//ft_printf("%d\n", listlen);
}

int		ft_print_long_format(t_directory *directory, t_file *file,
			t_lflags *lflags)
{
	char str[12];
	(void)directory;	
	ft_memset(str, 0, 12);
	(void)file;
	(void)lflags;
	ft_put_file_symbol(file, str);
	ft_put_file_permissions(file, str);
	ft_put_file_extended_attribute(file, str);
	ft_printf("%s\n", str);
	return (0);
}

int		ft_print_short_format(t_file *file, t_lflags *lflags)
{
	(void)lflags;
	ft_printf("%s\n", file->name);
	return (0);
}

int		ft_print_file(t_directory *directory, t_file *file, t_lflags *lflags)
{
	if (lflags->long_format)
	{
		if (ft_print_long_format(directory, file, lflags))
			return (1);
	}
	else
		ft_print_short_format(file, lflags);
		//printf("%s\n", ctime(&(file->stat.st_mtimespec.tv_sec)));
	return (0);
}

int		ft_update_directory_stats(t_file *file, t_directory *directory)
{
	int ret;

	if (!(file->user = getpwuid(file->stat.st_uid)))
		return (1);
	if (!(file->group = getgrgid(file->stat.st_gid)))
		return (1);
	directory->total_bytes += file->stat.st_size;
	if ((ret = ft_strlen(file->user->pw_name)) > directory->max_length_uid)
		directory->max_length_uid = ret;
	if ((ret = ft_strlen(file->group->gr_name)) > directory->max_length_gid)
		directory->max_length_gid = ret;
	if (directory->max_links < file->stat.st_nlink)
		directory->max_links = file->stat.st_nlink;
	if ((ret = ft_get_size_length(file->stat.st_size)) >
			directory->max_size_length)
		directory->max_links = ret;
	return (0);
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

int		ft_process_dir_files_list(t_directory *directory, t_lflags *lflags)
{
	t_list		*ptr;
	t_file		*file;

	ptr = directory->files;
	while (ptr != NULL)
	{
		file = ptr->content;
		ft_print_file(directory, file, lflags);
		ptr = ptr->next;
	}
	return (0);
}

void	ft_init_directory(t_directory *directory)
{
	directory->files = NULL;
	directory->total_bytes = 0;
	directory->max_length_uid = 0;
	directory->max_length_gid = 0;
	directory->max_links = 0;
	directory->max_size_length = 0;
}

int		ft_print_directories(t_list *directories, t_lflags *lflags)
{
	t_list *ptr;
	t_file *directory;

	ptr = directories;
	while (ptr != NULL)
	{
		directory = (t_file *)(ptr->content);
		if (ft_process_ls_directory(lflags, directory->name))
			return (1);
		ptr = ptr->next;
	}
	return (0);
}

int		ft_process_ls(t_lflags *lflags, int i, int argc, char **argv)
{
	struct stat	stat;

	t_list		*directories;
	t_directory	regular_file_group;

	ft_init_directory(&regular_file_group);
	directories = NULL;
	while (i < argc)
	{
		if (lstat(argv[i], &stat) == -1)
		{
			perror(argv[1]);
			return (1);
		}
		else
		{
			if (S_ISDIR(stat.st_mode) && !lflags->dir_as_file)
			{
				if (!ft_process_fill_files_list(stat, argv[i], &directories))
					return (1);
			}
			else
			{
				if (ft_process_fill_dir_files_list(stat, argv[i],
						&regular_file_group, lflags))
					return (1);
			}
		}
		i++;
	}
	ft_sort_files_list(&(regular_file_group.files), lflags);
	ft_sort_files_list(&directories, lflags);
	
	ft_process_dir_files_list(&regular_file_group, lflags);
	ft_print_directories(directories, lflags);
	return (0);
}

int		ft_process_ls_directory(t_lflags *lflags, char *path)
{
	t_directory		directory;
	DIR				*current_dir;
	
	ft_init_directory(&directory);
	if (!(current_dir = opendir(path)))
		return (1);
	if (ft_fill_dir_files_list(&directory, current_dir, path, lflags))
	{
		closedir(current_dir);
		ft_lstdel_value(&(directory.files));
		return (1);
	}
	ft_sort_files_list(&(directory.files), lflags);
	ft_process_dir_files_list(&directory, lflags);
	ft_lstdel_value(&(directory.files));
	closedir(current_dir);
	return (0);
}

void	ft_init_lflags(t_lflags *lflags)
{
	lflags->sort_format = LEXICOGRAPHIC;
	lflags->reverse_sort = 0;
	lflags->long_format = 0;
	lflags->recursive = 0;
	lflags->all = 0;
	lflags->dir_as_file = 0;
	lflags->colored = 0;
	lflags->show_owner = 1;
	lflags->last_access_flag = 0;
}

int main(int argc, char **argv)
{
	t_lflags			lflags;

	ft_init_lflags(&lflags);
	if (argc < 2)
		return (ft_process_ls_directory(&lflags, "."));
	else
	{
		if (ft_describe_options(argv[1]))
		{
			if (ft_parse_options(argv[1], &lflags))
				return (1);
			if (argc < 3)
				return (ft_process_ls_directory(&lflags, "."));
			return (ft_process_ls(&lflags, 2, argc, argv));
		}
		else
			return (ft_process_ls(&lflags, 1, argc, argv));
	}
}
