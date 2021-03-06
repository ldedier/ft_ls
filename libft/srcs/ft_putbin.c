/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putbin.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/01/29 13:57:47 by ldedier           #+#    #+#             */
/*   Updated: 2018/05/08 22:56:23 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putbin(size_t nb, t_pf *pf)
{
	if (nb / 2 == 0)
	{
		ft_putchar_buff(nb % 2 + '0', pf);
	}
	else
	{
		ft_putbin(nb / 2, pf);
		ft_putchar_buff(nb % 2 + '0', pf);
	}
}
