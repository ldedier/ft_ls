/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:27 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/22 18:27:02 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <time.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/xattr.h>
# include <pwd.h>
# include <grp.h>
# include <stdio.h>
# include "libft.h"

# define XATTR_SIZE	10000

typedef struct		s_file
{
	struct stat		stat;
	char			*name;
	struct passwd	*user;
	struct group	*group;
}					t_file;

typedef enum		e_sort
{
	LEXICOGRAPHIC,
	TIME_MODIFIED,
	LAST_ACCESS,
	UNSORTED
}					t_sort;

typedef struct		s_lflags
{
	t_sort			sort_format;
	char			reverse_sort;
	char			last_access_flag;
	char			long_format;
	char			recursive;
	char			all;
	char			show_owner;
	char			dir_as_file;
	char			colored;
}					t_lflags;

typedef struct		s_directory
{
	t_list			*files;
	int				total_bytes;
	int				max_length_uid;
	int				max_length_gid;
	int				max_links;
	int				max_size_length;
}					t_directory;

typedef void		(*t_opt_func)(t_lflags *);

void				ft_opt_r_maj(t_lflags *lflags);
void				ft_opt_r(t_lflags *lflags);
void				ft_opt_g_maj(t_lflags *lflags);
void				ft_opt_g(t_lflags *lflags);
void				ft_opt_a(t_lflags *lflags);
void				ft_opt_d(t_lflags *lflags);
void				ft_opt_f(t_lflags *lflags);
void				ft_opt_r(t_lflags *lflags);
void				ft_opt_u(t_lflags *lflags);
void				ft_opt_t(t_lflags *lflags);
void				ft_opt_l(t_lflags *lflags);

int					ft_sort_lexicographic(void *file1, void *file2);
int					ft_sort_last_access(void *file1, void *file2);
int					ft_sort_modification_time(void *file1, void *file2);

int					ft_process_ls_directory(t_lflags *lflags, char *path);
#endif
