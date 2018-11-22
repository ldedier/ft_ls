/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_options.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 14:00:52 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/22 18:29:18 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_opt_r(t_lflags *lflags)
{
	lflags->reverse_sort = 1;
}

void	ft_opt_f(t_lflags *lflags)
{
	lflags->sort_format = UNSORTED;
	ft_opt_a(lflags);
}

void	ft_opt_u(t_lflags *lflags)
{
	if (lflags->sort_format == TIME_MODIFIED)
		lflags->sort_format = LAST_ACCESS;
	lflags->last_access_flag = 1;
}

void	ft_opt_t(t_lflags *lflags)
{
	if (lflags->sort_format != UNSORTED)
	{
		if (lflags->last_access_flag)
			lflags->sort_format = LAST_ACCESS;
		else
			lflags->sort_format = TIME_MODIFIED;
	}
}
