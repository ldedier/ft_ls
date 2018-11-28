/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_long_format_left.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 14:48:58 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 16:11:52 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_add_destination_link(t_file *file, char *full_path)
{
	if (!(file->destination = ft_strnew(file->stat.st_size == 0 ?
					1024 : file->stat.st_size)))
		return (1);
	if (file->stat.st_size)
	{
		if (readlink(full_path, file->destination,
					file->stat.st_size) < file->stat.st_size)
			return (1);
	}
	else
	{
		if (readlink(full_path, file->destination, 1024) == -1)
			return (1);
	}
	return (0);
}

int		ft_put_file_symbol(t_file *file, char *str, char *full_path)
{
	if (S_ISREG(file->stat.st_mode))
		str[0] = '-';
	else if (S_ISDIR(file->stat.st_mode))
		str[0] = 'd';
	else if (S_ISLNK(file->stat.st_mode))
	{
		str[0] = 'l';
		if (ft_add_destination_link(file, full_path))
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
	if (file->stat.st_mode & S_ISUID)
		str[3] = 's';
	else
		str[3] = file->stat.st_mode & S_IXUSR ? 'x' : '-';
	str[4] = file->stat.st_mode & S_IRGRP ? 'r' : '-';
	str[5] = file->stat.st_mode & S_IWGRP ? 'w' : '-';
	if (file->stat.st_mode & S_ISGID)
		str[6] = 's';
	else
		str[6] = file->stat.st_mode & S_IXGRP ? 'x' : '-';
	str[7] = file->stat.st_mode & S_IROTH ? 'r' : '-';
	str[8] = file->stat.st_mode & S_IWOTH ? 'w' : '-';
	if (file->stat.st_mode & S_ISVTX)
		str[9] = 't';
	else
		str[9] = file->stat.st_mode & S_IXOTH ? 'x' : '-';
}

int		ft_has_acls(char *full_path)
{
	acl_t		acl;
	acl_entry_t	acl_entry;

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
