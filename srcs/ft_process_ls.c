/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_ls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/23 15:02:07 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 17:44:10 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_fill_arg_file(char *arg, struct stat *st, t_env *e, t_lflags *lflags)
{
	int ret;

	ret = 0;
	if (e->stat_func(arg, st) == -1)
	{
		ret = 1;
		if (ft_fill_path_error_tree(arg, &(e->errors)))
			return (2);
	}
	else
	{
		if (!lflags->dir_as_file && (S_ISDIR(st->st_mode)))
		{
			if (!ft_process_fill_files_tree(*st, arg,
						&(e->directories), lflags))
				return (2);
		}
		else
		{
			if (ft_process_fill_dir_files_tree(*st, arg,
						&(e->regular_file_group), lflags))
				return (2);
		}
	}
	return (ret);
}

int		ft_print_env(t_env *e, t_lflags *lflags)
{
	int			save;

	ft_print_errors(e->errors);
	if (e->regular_file_group.files)
	{
		if (ft_print_dir(&(e->regular_file_group), lflags))
			return (1);
	}
	if ((save = ft_print_directories(e->directories, lflags)) == 2)
		return (1);
	e->ret |= save;
	return (0);
}

int		ft_process_ls(t_lflags *lflags, int i, int argc, char **argv)
{
	t_env		e;
	struct stat	st;
	int			save;

	ft_init_env(&e, lflags);
	if (i < argc - 1)
		lflags->verbose = 1;
	while (i < argc)
	{
		if ((save = ft_fill_arg_file(argv[i], &st, &e, lflags)) == 2)
		{
			ft_free_env(&e, lflags);
			return (2);
		}
		e.ret |= save;
		i++;
	}
	if (ft_print_env(&e, lflags))
	{
		ft_free_env(&e, lflags);
		return (2);
	}
	ft_free_env(&e, lflags);
	return (e.ret);
}
