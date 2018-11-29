/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_short.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/29 00:18:05 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/29 00:52:55 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

int		ft_round(double value)
{
	if (value - (int)value)
		return ((int)value + 1);
	else
		return (value);
}

void	ft_print_line_display(t_display_tab disp, t_tree *files,
			t_directory *directory, t_lflags *lflags)
{
	int		j;
	int		first;
	t_file	*file;

	first = 1;
	j = 0;
	while (j < disp.nb_columns)
	{
		if (disp.line_number + j * disp.nb_lines < disp.nb_entries)
		{
			file = ft_tree_at_infix(files,
					disp.line_number + j * disp.nb_lines);
			ft_print_name_column(file, lflags, directory->name_max_length);
			ft_printf(" ");
		}
		j++;
	}
	ft_printf("\n");
}

void	ft_process_print_short(t_tree *tree, t_lflags *lflags)
{
	t_file	*file;

	if (tree != NULL)
	{
		ft_process_print_short(tree->left, lflags);
		file = (t_file *)(tree->content);
		ft_print_name(file, lflags, 0);
		ft_process_print_short(tree->right, lflags);
	}
}

int		ft_print_short(t_tree *files, t_directory *dir,
		t_lflags *lflags)
{
	if (lflags->display_format == COLUMN)
		return (ft_print_columns(files, dir, lflags));
	else
		ft_process_print_short(files, lflags);
	return (0);
}

int		ft_print_columns(t_tree *files, t_directory *directory,
		t_lflags *lflags)
{
	struct winsize	w;
	t_display_tab	disp;

	if (ioctl(0, TIOCGWINSZ, &w) == -1)
	{
		perror("ioctl");
		return (1);
	}
	disp.nb_entries = ft_treelen(files);
	disp.nb_columns = ft_max(1, w.ws_col / (directory->name_max_length + 1));
	disp.nb_lines = ft_round((double)disp.nb_entries / (double)disp.nb_columns);
	disp.line_number = 0;
	while (disp.line_number < disp.nb_lines)
	{
		ft_print_line_display(disp, files, directory, lflags);
		disp.line_number++;
	}
	return (0);
}
