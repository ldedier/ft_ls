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

	listlen = listxattr(full_path, NULL, XATTR_SIZE,
		XATTR_SHOWCOMPRESSION);
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
		if(str[i] >= 'A' && str[i] <= 'Z')
			str[i] -= 'A' - 'a';
		i++;
	}
}

int		ft_print_time_recent(char **split)
{
	char **clock_split;
	
	if (!(clock_split = ft_strsplit(split[3], ':')))
		return (1);
	ft_to_lower_str(split[1]);
	ft_printf(" %2s %s %s:%s", split[2], split[1], clock_split[0],
		clock_split[1]);
	return (0);
}


int		ft_print_time(t_file *file)
{
	char *str;
	char **split;
	time_t	now;

	now = time(NULL);
	if (!(str = ft_strdup(ctime(&(file->stat.st_mtimespec.tv_sec)))))
		return (1);
	if (!(split = ft_strsplit(str, ' ')))
		return (1);
	
	/*
	int i = 0;
	while (split[i])
	{
		ft_printf("%d: %s\n", i, split[i]);
		i++;
	}
*/
	if (now - file->stat.st_mtimespec.tv_sec > 10000)
		return ft_print_time_recent(split); //tochange
	else
		return ft_print_time_recent(split);
}

void	ft_print_name(t_file *file)
{
	if (file->destination != NULL)
		ft_printf(" %s -> %s\n", file->name, file->destination);
	else
		ft_printf(" %s\n", file->name);

}

int		ft_print_long_format(t_directory *directory, t_file *file,
			t_lflags *lflags)
{
	char	str[12];
	char	*full_path;

	
	if (!(full_path = ft_strjoin_3(directory->path, "/", file->name)))
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
	ft_printf(" %*lld", directory->max_size_length, file->stat.st_size);
	ft_print_time(file);
	ft_print_name(file);
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
		ft_print_short_format(file, lflags);
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
	if (ft_strlen(directory->path) && lflags->verbose)
		ft_printf("%s:\n", directory->path);
	ft_update_directory_data(directory);
	if (ft_strlen(directory->path) && lflags->long_format)
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
