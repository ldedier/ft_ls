/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 00:49:53 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

char	*ft_get_full_path(t_directory *dir, t_file *file)
{
	char *full_path;

	if (dir->path)
	{
		if (!(full_path = ft_strjoin_3(dir->path, "/", file->name)))
			return (NULL);
	}
	else
	{
		if (!(full_path = ft_strdup(file->name)))
			return (NULL);
	}
	return (full_path);
}

int		ft_off_tlen(off_t val)
{
	int res;

	res = 1;
	while (val / 10 != 0)
	{
		val /= 10;
		res++;
	}
	return (res);
}

int		ft_nlink_tlen(nlink_t val)
{
	int res;

	res = 1;
	while (val / 10 != 0)
	{
		val /= 10;
		res++;
	}
	return (res);
}

int		ft_print_directories(t_tree *tree, t_lflags *lflags)
{
	int			ret;
	int			save;
	t_file		*directory;

	ret = 0;
	if (tree != NULL)
	{
		if ((save = ft_print_directories(tree->left, lflags)) == 2)
			return (2);
		ret |= save;
		directory = (t_file *)(tree->content);
		if ((save = ft_process_ls_directory(lflags, directory->name,
						directory->name)) == 2)
			return (2);
		ret |= save;
		if ((save = ft_print_directories(tree->right, lflags)) == 2)
			return (2);
		ret |= save;
	}
	return (ret);
}
