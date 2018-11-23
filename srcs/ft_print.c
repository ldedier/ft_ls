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
	if (directory->path && lflags->verbose)
		ft_printf("%s:\n", directory->path);
	if (directory->path && lflags->long_format)
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
