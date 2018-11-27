/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 14:07:00 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/27 18:20:21 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

t_error	*ft_new_error(char *path)
{
	t_error	*res;

	if (!(res = (t_error *)(malloc(sizeof(t_error)))))
		return (NULL);
	res->path = path;
	res->errno_save = errno;
	return (res);
}

int		ft_fill_path_error_list(char *path, t_list **errors)
{
	t_error *error;

	if (!(error = ft_new_error(path)))
	{
		ft_lstdel_value(errors);
		return (1);
	}
	if (ft_add_to_list_ptr_back(errors, error,
				sizeof(t_error)))
	{
		ft_lstdel_value(errors);
		return (1);
	}
	return (0);
}

int		ft_process_error_dir(char *full_path, char *path, t_lflags *lflags)
{
	char *str;

	if (lflags->first_entry)
		lflags->first_entry = 0;
	else
		ft_printf("\n");
	if (lflags->verbose)
		ft_printf("%s:\n", full_path);
	if (!(str = ft_strjoin("ft_ls: ", path)))
		return (1);
	perror(str);
	free(str);
	return (0);
}

int		ft_process_error_stat(char *path, int *ret)
{
	char *str;

	if (!(str = ft_strjoin("ft_ls: ", path)))
		return (1);
	perror(str);
	free(str);
	*ret = 1;
	return (0);
}
