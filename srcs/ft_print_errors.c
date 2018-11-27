/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_errors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 16:33:41 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/27 18:19:08 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_print_error(t_error *error)
{
	char *str;
	
	if (!(str = strerror(error->errno_save)))
		return (1);
	ft_dprintf(2, "ft_ls: %s: %s\n", error->path, str);
	return (0);
}

int		ft_print_errors(t_list *errors)
{
	t_list *ptr;
	t_error *error;

	ptr = errors;
	while (ptr != NULL)
	{
		error = (t_error *)(ptr->content);
		if (ft_print_error(error))
			return (1);
		ptr = ptr->next;
	}
	return (0);
}
