/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/22 19:41:05 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

static t_opt_func g_opt_arr[NB_CHARS] =
{
	['l'] = ft_opt_l,
	['R'] = ft_opt_r_maj,
	['a'] = ft_opt_a,
	['r'] = ft_opt_r,
	['t'] = ft_opt_t,
	['f'] = ft_opt_f,
	['u'] = ft_opt_u,
	['g'] = ft_opt_g,
	['d'] = ft_opt_d,
	['G'] = ft_opt_g_maj
};

int		ft_describe_options(char *str)
{
	int		i;

	if (str[0] != '-')
		return (0);
	i = 0;
	while (str[i])
	{
		if (i == 1)
			return (1);
		i++;
	}
	return (0);
}

int		ft_parse_options(char *str, t_lflags *lflags)
{
	int i;

	i = 1;
	while (str[i])
	{
		if (g_opt_arr[(int)str[i]] == NULL)
		{
			ft_dprintf(2, "ls: illegal option -- %c\n", str[i]);
			ft_dprintf(2, "usage: ls [-GRadfglrtu] [file ...]\n"); //to update?
			return (1);
		}
		g_opt_arr[(int)str[i]](lflags);
		i++;
	}
	return (0);
}

int		ft_process_ls_args(t_lflags *lflags, int i, int argc, char **argv)
{
	(void)lflags;
	(void)argc;
	(void)argv;
	(void)i;
	return (0);
}

t_file	*ft_new_file(struct stat stat, struct dirent entry)
{
	t_file	*res;

	if (!(res = (t_file *)(malloc(sizeof(t_file)))))
		return (NULL);
	res->stat = stat;
	res->entry = entry;
	return (res);
}

int		ft_should_add_entry(struct dirent entry, struct stat stat,
			t_lflags *lflags)
{
	(void)stat;

	if (!lflags->all && entry.d_name[0] == '.' && !(lflags->dir_as_file &&
		!ft_strcmp((char *)entry.d_name, ".")))
		return (0);
	else if (lflags->dir_as_file &&
		ft_strcmp((char *)entry.d_name, "."))
		return (0);
	return (1);
}

int		ft_fill_stats_list(t_list **list, DIR *current_dir, t_lflags *lflags)
{
	struct stat		stat;
	struct dirent	*entry;
	t_file			*file;

	while ((entry = readdir(current_dir)) != NULL)
	{
		lstat(entry->d_name, &stat);
		if (ft_should_add_entry(*entry, stat, lflags))
		{
			if (!(file = ft_new_file(stat, *entry)))
			{
				ft_lstdel_value(list);
				closedir(current_dir);
				return (1);
			}
			if (ft_add_to_list_ptr_back(list, file, sizeof(t_file)))
			{
				ft_lstdel_value(list);
				closedir(current_dir);
				return (1);
			}
		}
	}
	return (0);
}

void	ft_sort_stats_list(t_list **stats, t_lflags *lflags)
{
	if (lflags->sort_format == LEXICOGRAPHIC)
		ft_lst_mergesort(stats, &ft_sort_lexicographic, lflags->reverse_sort);
	else if (lflags->sort_format == TIME_MODIFIED)
		ft_lst_mergesort(stats, &ft_sort_modification_time, lflags->reverse_sort);
	else if (lflags->sort_format == LAST_ACCESS)
		ft_lst_mergesort(stats, &ft_sort_last_access, lflags->reverse_sort);
}

void	ft_print_file_symbol(t_file *file)
{
	if (S_ISREG(file->stat.st_mode))
		ft_putchar('-');
	else if (S_ISDIR(file->stat.st_mode))
		ft_putchar('d');
	else if (S_ISLNK(file->stat.st_mode))
		ft_putchar('l');
	else if (S_ISSOCK(file->stat.st_mode))
		ft_putchar('s');
	else if (S_ISFIFO(file->stat.st_mode))
		ft_putchar('p');
	else if (S_ISBLK(file->stat.st_mode))
		ft_putchar('b');
	else if (S_ISCHR(file->stat.st_mode))
		ft_putchar('c');
}

int		ft_print_long_format(t_file *file, t_lflags *lflags)
{
	(void)file;
	(void)lflags;
	ft_print_file_symbol(file);
	ft_printf("\n");
	return (0);
}

int		ft_print_short_format(t_file *file, t_lflags *lflags)
{
	(void)lflags;
	ft_printf("%s\n", file->entry.d_name);
	return (0);
}

int		ft_print_file(t_file *file, t_lflags *lflags)
{
	if (lflags->long_format)
	{
		if (ft_print_long_format(file, lflags))
			return (1);
	}
	else
		ft_print_short_format(file, lflags);
		//printf("%s\n", ctime(&(file->stat.st_mtimespec.tv_sec)));
	return (0);
}

int		ft_process_stats_list(t_list *stats, t_lflags *lflags)
{
	t_list		*ptr;
	t_file		*file;
	(void)lflags;

	ptr = stats;
	while (ptr != NULL)
	{
		file = ptr->content;
		ft_print_file(file, lflags);
		// printf("size: %lld\n", file->stat.st_size); /* you may use %lld */
		ptr = ptr->next;
	}
	return (0);
}

int		ft_process_ls(t_lflags *lflags)
{
	t_list			*stats;
	DIR				*current_dir;

	stats = NULL;
	if (!(current_dir = opendir(".")))
		return (1);
	if (ft_fill_stats_list(&stats, current_dir, lflags))
		return (1);
	ft_sort_stats_list(&stats, lflags);
	ft_process_stats_list(stats, lflags);
	ft_lstdel_value(&stats);
	closedir(current_dir);
	return (0);
}

void	ft_init_lflags(t_lflags *lflags)
{
	lflags->sort_format = LEXICOGRAPHIC;
	lflags->reverse_sort = 0;
	lflags->long_format = 0;
	lflags->recursive = 0;
	lflags->all = 0;
	lflags->dir_as_file = 0;
	lflags->colored = 0;
	lflags->show_owner = 1;
	lflags->last_access_flag = 0;
}

int main(int argc, char **argv)
{
	struct stat		sb;
	struct passwd	*pwuser;
	struct group	*grpnam;
	t_lflags			lflags;

	ft_init_lflags(&lflags);
	if (argc < 2)
		return (ft_process_ls(&lflags));
	else
	{
		if (ft_describe_options(argv[1]))
		{
			if (ft_parse_options(argv[1], &lflags))
				return (1);
			if (argc < 3)
				return (ft_process_ls(&lflags));
			return (ft_process_ls_args(&lflags, 2, argc, argv));
		}
		else
			return (ft_process_ls_args(&lflags, 1, argc, argv));
	}
	if (lstat(argv[1], &sb) == -1)
	{
		perror("stat()");
		exit(EXIT_FAILURE);
	}

	if (NULL == (pwuser = getpwuid(sb.st_uid)))
	{
		perror("getpwuid()");
		exit(EXIT_FAILURE);
	}

	if (NULL == (grpnam = getgrgid(sb.st_gid)))
	{
		perror("getgrgid()");
		exit(EXIT_FAILURE);
	}

	if (S_ISDIR(sb.st_mode))
	{
		ft_printf("dir !\n");
	}
	if (S_ISLNK(sb.st_mode))
	{
		ft_printf("symbolic link !\n");
	}
	if (S_ISREG(sb.st_mode))
	{
		ft_printf("regular !\n");
	}
	printf("%s:\n", argv[1]);
	printf("\tinode: %llu\n", sb.st_ino);
	printf("\towner: %u (%s)\n", sb.st_uid, pwuser->pw_name);
	printf("\tgroup: %u (%s)\n", sb.st_gid, grpnam->gr_name);
	printf("\tperms: %o\n", sb.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
	printf("\tlinks: %d\n", sb.st_nlink);
	printf("\tsize: %lld\n", sb.st_size); /* you may use %lld */
	printf("\tatime: %s", ctime(&sb.st_atimespec.tv_sec));
	printf("\tmtime: %s", ctime(&sb.st_mtimespec.tv_sec));
	printf("\tctime: %s", ctime(&sb.st_ctimespec.tv_sec));

	printf("\n");
	return 0;
}
