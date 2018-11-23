/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_options.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 14:09:29 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/23 14:09:29 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_opt_func g_opt_arr[NB_CHARS] =
{
	['l'] = ft_opt_l,
	['R'] = ft_opt_r_maj,
	['a'] = ft_opt_a,
	['r'] = ft_opt_r,
	['t'] = ft_opt_t,
	['f'] = ft_opt_f,
	['u'] = ft_opt_u,
	['g'] = ft_opt_g,
	['d'] = ft_opt_d,
	['G'] = ft_opt_g_maj
};

int		ft_describe_options(char *str)
{
	int		i;

	if (str[0] != '-')
		return (0);
	i = 0;
	while (str[i])
	{
		if (i == 1)
			return (1);
		i++;
	}
	return (0);
}

int		ft_parse_options(char *str, t_lflags *lflags)
{
	int i;

	i = 1;
	if (!ft_strcmp(str, "--"))
		return (0);
	while (str[i])
	{
		if (g_opt_arr[(int)str[i]] == NULL)
		{
			ft_dprintf(2, "ls: illegal option -- %c\n", str[i]);
			ft_dprintf(2, "usage: ls [-GRadfglrtu] [file ...]\n"); //to update?
			return (1);
		}
		g_opt_arr[(int)str[i]](lflags);
		i++;
	}
	return (0);
}
