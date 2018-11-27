/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 14:03:09 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/23 14:03:09 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_put_file_symbol(t_file *file, char *str, char *full_path)
{
	if (S_ISREG(file->stat.st_mode))
		str[0] = '-';
	else if (S_ISDIR(file->stat.st_mode))
		str[0] = 'd';
	else if (S_ISLNK(file->stat.st_mode))
	{
		str[0] = 'l';
	 	if(!(file->destination = ft_strnew(file->stat.st_size)))
			return (1);
		if (readlink(full_path, file->destination,
				file->stat.st_size) < file->stat.st_size)
			return (1);
//		ft_printf("%s\n", full_path);
//		ft_printf("%s\n", file->destination);
	}
	else if (S_ISSOCK(file->stat.st_mode))
		str[0] = 's';
	else if (S_ISFIFO(file->stat.st_mode))
		str[0] = 'p';
	else if (S_ISBLK(file->stat.st_mode))
		str[0] = 'b';
	else if (S_ISCHR(file->stat.st_mode))
		str[0] = 'c';
	return (0);
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

int		ft_has_acls(char *full_path)
{
	acl_t acl;
	acl_entry_t acl_entry;
	
	if (!(acl = acl_get_link_np(full_path, ACL_TYPE_EXTENDED)))
		return (0);
	if (acl_get_entry(acl, ACL_FIRST_ENTRY, &acl_entry) == -1)
	{
		acl_free(acl);
		return (0);
	}
	else
	{
		acl_free(acl);
		return (1);
	}
}

void	ft_put_file_extended_attribute_or_acls(char *full_path, char *str)
{
	int		listlen;

	listlen = listxattr(full_path, NULL, XATTR_SIZE, XATTR_NOFOLLOW);
	if (listlen > 0)
		str[10] = '@';
	else if (ft_has_acls(full_path))
		str[10] = '+';
	else
		str[10] = ' ';
}

void	ft_to_lower_str(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] -= 'A' - 'a';
		i++;
	}
}

void	ft_to_true_year(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
			return ;
		}
		i++;
	}
}

int		ft_print_time_recent(char **split)
{
	char **clock_split;

	if (!(clock_split = ft_strsplit(split[3], ':')))
	{
		ft_free_split(split);
		return (1);
	}
	ft_to_lower_str(split[1]);
	ft_printf(" %2s %s %s:%s ", split[2], split[1], clock_split[0],
		clock_split[1]);
	ft_free_split(split);
	ft_free_split(clock_split);
	return (0);
}

int		ft_print_time_old(char **split)
{
	ft_to_lower_str(split[1]);
	ft_to_true_year(split[4]);
	ft_printf(" %2s %s %5s ", split[2], split[1], split[4]);
	ft_free_split(split);
	return (0);
}

int		ft_print_time_spec(struct timespec ts)
{
	char *str;
	char **split;
	time_t	now;

	now = time(NULL);
	if (!(str = ft_strdup(ctime(&(ts.tv_sec)))))
		return (1);
	if (!(split = ft_strsplit(str, ' ')))
		return (1);
	free(str);
	/*
	int i = 0;
	while (split[i])
	{
		ft_printf("%d: %s\n", i, split[i]);
		i++;
	}
*/
	if (now - ts.tv_sec > 15780000)
		return ft_print_time_old(split); //tochange
	else
		return ft_print_time_recent(split);
}

void	ft_fill_str_col(t_file *file, t_lflags *lflags, char *str)
{
	if (lflags->colored) 
	{
		if (S_ISREG(file->stat.st_mode) && file->stat.st_mode & S_IXUSR)
			ft_strcpy(str, RED);
		else if (S_ISDIR(file->stat.st_mode))
			ft_strcpy(str, DIR_COL);
		else if (S_ISLNK(file->stat.st_mode))
			ft_strcpy(str, MAGENTA);
		else if (S_ISSOCK(file->stat.st_mode))
			ft_strcpy(str, SOCK_COL);
		else if (S_ISFIFO(file->stat.st_mode))
			ft_strcpy(str, FIFO_COL);
		else if (S_ISBLK(file->stat.st_mode))
			ft_strcpy(str, BLK_COL);
		else if (S_ISCHR(file->stat.st_mode))
			ft_strcpy(str, CHR_COL);
	}
}

void	ft_print_name(t_file *file, t_lflags *lflags, int show_dest)
{
	char str_col[MAX_COL];

	ft_memset(str_col, 0, MAX_COL);
	ft_fill_str_col(file, lflags, str_col);
	if (show_dest && file->destination != NULL)
		ft_printf("%s%s%s -> %s\n", str_col, file->name, 
			lflags->colored ? EOC : "", file->destination);
	else
		ft_printf("%s%s%s\n", str_col, file->name, lflags->colored ? EOC : "");
}

char	*ft_get_full_path(t_directory *dir, t_file *file)
{
	char *full_path;

	if (dir->path)
	{
		if (!(full_path = ft_strjoin_3(dir->path, "/", file->name)))
			return (NULL);
	}
	else
	{
		if (!(full_path = ft_strdup(file->name)))
			return (NULL);
	}
	return (full_path);
}

void	ft_print_majors_minors(t_directory *directory, t_file *file)
{
	(void)directory;
	ft_printf(" %3d, %3d", file->stat.st_rdev >> 24, file->stat.st_rdev & 127);
}

void	ft_print_size(t_directory *directory, t_file *file)
{
	if (directory->has_devices)
	{
		if (S_ISBLK(file->stat.st_mode) || S_ISCHR(file->stat.st_mode))
			ft_print_majors_minors(directory, file);
		else
			ft_printf(" %*lld", directory->max_size_length + 7, file->stat.st_size);
	}
	else
		ft_printf(" %*lld", directory->max_size_length, file->stat.st_size);
}

int		ft_print_time(t_file *file, t_lflags *lflags)
{
	if (lflags->sort_format == LAST_ACCESS)
		return ft_print_time_spec(file->stat.st_atimespec);
	else
		return ft_print_time_spec(file->stat.st_mtimespec);
}

int		ft_print_long_format(t_directory *directory, t_file *file,
			t_lflags *lflags)
{
	char	str[12];
	char	*full_path;

	if (!(full_path = ft_get_full_path(directory, file)))
		return (1);
	(void)lflags;
	ft_put_file_symbol(file, str, full_path);
	ft_put_file_permissions(file, str);
	ft_put_file_extended_attribute_or_acls(full_path, str);
	str[11] = '\0';
	ft_printf("%s", str);
	ft_printf(" %*d", directory->max_links_length, file->stat.st_nlink);
	if (lflags->show_owner)
		ft_printf(" %-*s ", directory->max_length_uid, file->user);
	ft_printf(" %-*s ", directory->max_length_gid, file->group);
	ft_print_size(directory, file);
	ft_print_time(file, lflags);
	ft_print_name(file, lflags, 1);
	free(full_path);
	return (0);
}

int		ft_print_short_format(t_file *file, t_lflags *lflags)
{
	(void)lflags;
	ft_printf("%s\n", file->name);
	return (0);
}

int		ft_print_dir_file(t_directory *directory, t_file *file, t_lflags *lflags)
{
	if (lflags->long_format)
	{
		if (ft_print_long_format(directory, file, lflags))
			return (1);
	}
	else
		ft_print_name(file, lflags, 0);
	return (0);
}

int		ft_print_dir(t_directory *directory, t_lflags *lflags)
{
	t_list		*ptr;
	t_file		*file;
	
	if (lflags->first_entry)
		lflags->first_entry = 0;
	else
		ft_printf("\n");
	if (directory->path && lflags->verbose)
		ft_printf("%s:\n", directory->path);
	ft_update_directory_data(directory);
	if (directory->path && lflags->long_format && directory->files)
		ft_printf("total %d\n", directory->total_blocks);
	ptr = directory->files;
	while (ptr != NULL)
	{
		file = ptr->content;
		ft_print_dir_file(directory, file, lflags);
		ptr = ptr->next;
	}
	return (0);
}
