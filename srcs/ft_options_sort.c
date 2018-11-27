/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_options.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 14:00:52 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/27 22:10:09 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_opt_r(t_lflags *lflags)
{
	lflags->order = -1;
	if (lflags->sort_format == TIME_MODIFIED)
		lflags->sort_func = &ft_sort_modification_time_inv;
	else if (lflags->sort_format == LAST_ACCESS)
		lflags->sort_func = &ft_sort_last_access_inv;
	else if (lflags->sort_format == LEXICOGRAPHIC)
		lflags->sort_func = &ft_sort_lexicographic_inv;
}

void	ft_opt_f(t_lflags *lflags)
{
	lflags->sort_format = UNSORTED;
	ft_opt_a(lflags);
	lflags->sort_func = &ft_sort_in_order;
}

void	ft_opt_u(t_lflags *lflags)
{
	if (lflags->sort_format == TIME_MODIFIED)
	{
		if (lflags->order == 1)
			lflags->sort_func = &ft_sort_last_access;
		else
			lflags->sort_func = &ft_sort_last_access_inv;
		lflags->sort_format = LAST_ACCESS;
	}
	lflags->last_access_flag = 1;
}

void	ft_opt_t(t_lflags *lflags)
{
	if (lflags->sort_format != UNSORTED)
	{
		if (lflags->last_access_flag)
		{
			if (lflags->order == 1)
				lflags->sort_func = &ft_sort_last_access;
			else
				lflags->sort_func = &ft_sort_last_access_inv;
			lflags->sort_format = LAST_ACCESS;
		}
		else
		{
			if (lflags->order == 1)
				lflags->sort_func = &ft_sort_modification_time;
			else
				lflags->sort_func = &ft_sort_modification_time_inv;
			lflags->sort_format = TIME_MODIFIED;
		}
	}
}
