/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:27 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/21 13:42:31 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <time.h>
# include <dirent.h>
# include <sys/stat.h>
# include <pwd.h>
# include <grp.h>
# include <stdio.h>
# include "libft.h"

typedef enum		e_sort
{
	LEXICOGRAPHIC,
	TIME_MODIFIED,
	LAST_ACCESS,
	REVERSE,
	UNSORTED
}					t_sort;

typedef struct		s_lflags
{
	t_sort			sort_format;
	char			long_format;
	char			recursive;
	char			all;
	char			show_owner;
	char			dir_only;
	char			colored;
}					t_lflags;

typedef void		(*t_opt_func)(t_lflags *);

#endif
