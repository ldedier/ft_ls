/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/21 16:09:22 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

/*
static t_opt_func g_opt_arr[NB_CHARS] =
{
	['l'] = ft_opt_l,
	['R'] = ft_opt_r_maj,
	['a'] = ft_opt_a,
	['r'] = ft_opt_r,
	['f'] = ft_opt_t,
	['u'] = ft_opt_u,
	['g'] = ft_opt_g,
	['d'] = ft_opt_d,
	['G'] = ft_opt_g_maj
};
*/

void	ft_opt_r_maj(t_lflags *lflags)
{
	lflags->recursive = 1;
}

static t_opt_func g_opt_arr[NB_CHARS] =
{
	['R'] = ft_opt_r_maj
};

int		ft_describe_options(char *str)
{
	int		i;

	if (str[0] != '-')
		return (0);
	i = 0;
	while (str[i])
	{
		if (i == 2)
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

int		ft_process_ls(t_lflags *lflags)
{
	(void)lflags;
	return (0);
}

void	ft_init_lflags(t_lflags *lflags)
{
	lflags->sort_format = LEXICOGRAPHIC;
	lflags->long_format = 0;
	lflags->recursive = 0;
	lflags->all = 0;
	lflags->show_owner = 1;
	lflags->dir_only = 0;
	lflags->colored = 0;
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
//			return (ft_process_ls_args(&lflags, 2, argc, argv));
		}
//		else
//			return (ft_process_ls_args(&lflags, 1, argc, argv));
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
