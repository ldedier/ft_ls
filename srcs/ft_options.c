/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_options.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/22 14:00:52 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/22 14:44:07 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_opt_r_maj(t_lflags *lflags)
{
	if (!lflags->dir_as_file)
		lflags->recursive = 1;
}

void	ft_opt_g(t_lflags *lflags)
{
	lflags->show_owner = 0;
}

void	ft_opt_g_maj(t_lflags *lflags)
{
	lflags->colored = 1;
}

void	ft_opt_a(t_lflags *lflags)
{
	lflags->all = 1;
}

void	ft_opt_d(t_lflags *lflags)
{
	lflags->dir_as_file = 1;
	lflags->recursive = 0;

}

void	ft_opt_l(t_lflags *lflags)
{
	lflags->long_format = 1;
}
