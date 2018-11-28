/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sorts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 17:18:42 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 17:47:37 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_sort_lexicographic_inv(void *file1, void *file2)
{
	t_file	*f1;
	t_file	*f2;

	f1 = (t_file *)file1;
	f2 = (t_file *)file2;
	return (ft_strcmp(f2->name, f1->name));
}

int		ft_sort_modification_time_inv(void *file1, void *file2)
{
	t_file	*f1;
	t_file	*f2;

	f1 = (t_file *)file1;
	f2 = (t_file *)file2;
	if (f2->stat.st_mtimespec.tv_sec == f1->stat.st_mtimespec.tv_sec)
	{
		if (f2->stat.st_mtimespec.tv_nsec == f1->stat.st_mtimespec.tv_nsec)
			return (ft_strcmp(f2->name, f1->name));
		else if (f2->stat.st_mtimespec.tv_nsec > f1->stat.st_mtimespec.tv_nsec)
			return (-1);
		else
			return (1);
	}
	else if (f2->stat.st_mtimespec.tv_sec > f1->stat.st_mtimespec.tv_sec)
		return (-1);
	else
		return (1);
}

int		ft_sort_last_access_inv(void *file1, void *file2)
{
	t_file	*f1;
	t_file	*f2;

	f1 = (t_file *)file1;
	f2 = (t_file *)file2;
	if (f2->stat.st_atimespec.tv_sec == f1->stat.st_atimespec.tv_sec)
	{
		if (f2->stat.st_atimespec.tv_nsec == f1->stat.st_atimespec.tv_nsec)
			return (ft_strcmp(f2->name, f1->name));
		else if (f2->stat.st_atimespec.tv_nsec > f1->stat.st_atimespec.tv_nsec)
			return (-1);
		else
			return (1);
	}
	else if (f2->stat.st_atimespec.tv_sec > f1->stat.st_atimespec.tv_sec)
		return (-1);
	else
		return (1);
}
