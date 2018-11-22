/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sorts.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 17:18:42 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/22 18:34:27 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_sort_lexicographic(void *file1, void *file2)
{
	t_file	*f1;
	t_file	*f2;

	f1 = (t_file *)file1;
	f2 = (t_file *)file2;
	return (ft_strcmp(f1->entry.d_name, f2->entry.d_name));
}

int		ft_sort_modification_time(void *file1, void *file2)
{
	t_file	*f1;
	t_file	*f2;
	int		ret;

	f1 = (t_file *)file1;
	f2 = (t_file *)file2;
	if ((ret = (f2->stat.st_mtimespec.tv_sec - f1->stat.st_mtimespec.tv_sec)))
		return (ret);
	else
		return (ft_strcmp(f1->entry.d_name, f2->entry.d_name));
}

int		ft_sort_last_access(void *file1, void *file2)
{
	t_file	*f1;
	t_file	*f2;
	int		ret;

	f1 = (t_file *)file1;
	f2 = (t_file *)file2;
	if ((ret = (f2->stat.st_atimespec.tv_sec - f1->stat.st_atimespec.tv_sec)))
		return (ret);
	else
		return (ft_strcmp(f1->entry.d_name, f2->entry.d_name));
}
