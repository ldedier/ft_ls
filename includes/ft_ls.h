/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ls.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:27 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 14:32:58 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_LS_H
# define FT_LS_H

# include <time.h>
# include <dirent.h>
# include <sys/stat.h>
# include <sys/xattr.h>
# include <sys/types.h>
# include <sys/acl.h>
# include <pwd.h>
# include <grp.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include "libft.h"

# define MAX_COL	30
# define XATTR_SIZE	100000
# define DIR_COL	L_CYAN BOLD
# define CHR_COL	BLUE BYELLOW
# define BLK_COL	BLUE BCYAN
# define FIFO_COL	RED
# define SOCK_COL	RED

typedef struct		s_file
{
	struct stat		stat;
	char			*name;
	char			*user;
	char			*group;
	char			*destination;
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
	int				(*sort_func)(void *,void *);
	char			order;
	char			last_access_flag;
	char			long_format;
	char			recursive;
	char			all;
	char			show_owner;
	char			dir_as_file;
	char			colored;
	char			verbose;
	char			first_entry;
}					t_lflags;

typedef struct		s_error
{
	int				errno_save;
	char			*path;
}					t_error;

typedef struct		s_directory
{
	t_tree			*files;
	char			*path;
	int				total_blocks;
	int				max_length_uid;
	int				max_length_gid;
	nlink_t			max_links;
	int				max_links_length;
	off_t			max_size;
	int				max_size_length;
	char			has_devices;
}					t_directory;

typedef struct		s_env
{
	t_tree			*errors;
	t_directory		regular_file_group;
	t_tree			*directories;
	int				(*stat_func)(const char *restrict , struct stat *restrict);
	int				ret;
}					t_env;

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

int					ft_sort_lexicographic_err(void *file1, void *file2);
int					ft_sort_lexicographic(void *file1, void *file2);
int					ft_sort_last_access(void *file1, void *file2);
int					ft_sort_in_order(void *file1, void *file2);
int					ft_sort_modification_time(void *file1, void *file2);
int					ft_sort_lexicographic_inv(void *file1, void *file2);
int					ft_sort_last_access_inv(void *file1, void *file2);
int					ft_sort_modification_time_inv(void *file1, void *file2);

int					ft_update_directory_stats(t_file *file, t_directory *dir);
void				ft_update_directory_data(t_directory *dir);
int					ft_print_dir(t_directory *dir, t_lflags *lfs);

int					ft_process_error_dir(char *full_path, char *path, t_lflags *lflags);
int					ft_process_error_stat(char *path, int *ret);
int					ft_fill_path_error_tree(char *path, t_tree **errs);

int					ft_describe_options(char *str);
int					ft_parse_options(char *str, t_lflags *lflags);

void				ft_init_directory_no_path(t_directory *dir);
int					ft_init_directory(t_directory *dir, char *path);
void				ft_init_lflags(t_lflags *lflags);
void				ft_init_env(t_env *e, t_lflags *lflags);

//void				ft_sort_files_list(t_list **files, t_lflags *lflags);
//void				ft_sort_errors(t_list **errors);

t_file				*ft_process_fill_files_tree(struct stat stat, char *path,
						t_tree **files, t_lflags *lflags);

int					ft_process_fill_dir_files_tree(struct stat stat, char *path,
						t_directory *directory, t_lflags *lflags);
int					ft_fill_dir_files_tree(t_directory *directory,
						DIR *current_dir, char *path, t_lflags *lflags);
/*
** process ls
*/

int					ft_process_ls(t_lflags* lflags, int i,
						int argc, char **argv);
int					ft_process_ls_directory(t_lflags *lflags, char *full_path,
						char *path);

int					ft_print_errors(t_tree *errors);
int					ft_print_directories(t_tree *directories, t_lflags *lflgs);

void				ft_free_file(void *file);
void				ft_free_directory(t_directory *directory, t_lflags *lflags);
void				ft_free_error(void *error);
void				ft_free_files_tree(t_tree **tree, t_lflags *lflags);
void				ft_free_directories(t_tree **directories);
#endif
