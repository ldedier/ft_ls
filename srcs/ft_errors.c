/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_errors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 14:07:00 by ldedier           #+#    #+#             */
/*   Updated: 2018/12/20 14:34:26 by ldedier          ###   ########.fr       */
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

int		ft_fill_path_error_tree(char *path, t_tree **errors)
{
	t_error *error;

	if (!(error = ft_new_error(path)))
		return (1);
	if (ft_tree_add_sorted(errors, error, &ft_sort_lexicographic_err))
		return (1);
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
		return (2);
	perror(str);
	free(str);
	return (0);
}

int		ft_print_error(t_error *error)
{
	char *str;

	if (!(str = strerror(error->errno_save)))
		return (1);
	ft_dprintf(2, "ft_ls: %s: %s\n", error->path, str);
	return (0);
}

int		ft_print_errors(t_tree *errors)
{
	t_error *error;

	if (errors != NULL)
	{
		if (ft_print_errors(errors->left))
			return (1);
		error = (t_error *)(errors->content);
		if (ft_print_error(error))
			return (1);
		if (ft_print_errors(errors->right))
			return (1);
	}
	return (0);
}
