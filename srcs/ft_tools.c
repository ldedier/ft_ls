/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 14:44:54 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

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
