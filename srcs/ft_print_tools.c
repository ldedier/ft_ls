/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 14:59:14 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 00:50:06 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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

void	ft_print_name_column(t_file *file, t_lflags *lflags, int padding)
{
	char str_col[MAX_COL];

	ft_memset(str_col, 0, MAX_COL);
	ft_fill_str_col(file, lflags, str_col);
	ft_printf("%s%-*s%s", str_col, padding, file->name,
		lflags->colored ? EOC : "");
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
		ft_printf("%s%s%s\n", str_col, file->name,
			lflags->colored ? EOC : "");
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
			ft_printf(" %*lld", ft_max(8, directory->max_size_length),
					file->stat.st_size);
	}
	else
		ft_printf(" %*lld", directory->max_size_length, file->stat.st_size);
}
