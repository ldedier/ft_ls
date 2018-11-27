/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_add_sort.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/27 18:55:09 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/27 19:49:03 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_tree_add_sort(t_tree **tree, void *content,
			int (sort*)(void*, void *), int mul)
{
	if (*tree == NULL)
	{
		if (!(*tree = ft_tree_new(content)))
			return (1);
	}
	else
	{
		if (mul * sort(*tree->content, content) > 0)
			return ft_tree_add_sort(&((*tree)->left), content, sort, mul);
		else
			return ft_tree_add_sort(&((*tree)->right), content, sort, mul);
	}
}
