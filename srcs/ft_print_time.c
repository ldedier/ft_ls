/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_time.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/28 14:54:38 by ldedier           #+#    #+#             */
/*   Updated: 2018/11/28 16:42:56 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_to_true_year(char *str)
{
	int i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
		{
			str[i] = '\0';
			return ;
		}
		i++;
	}
}

int		ft_print_time_recent(char **split)
{
	char **clock_split;

	if (!(clock_split = ft_strsplit(split[3], ':')))
	{
		ft_free_split(split);
		return (1);
	}
	ft_printf(" %s %2s %s:%s ", split[1], split[2], clock_split[0],
		clock_split[1]);
	ft_free_split(split);
	ft_free_split(clock_split);
	return (0);
}

int		ft_print_time_old(char **split)
{
	ft_to_true_year(split[4]);
	ft_printf(" %s %2s %5s ", split[1], split[2], split[4]);
	ft_free_split(split);
	return (0);
}

int		ft_print_time_spec(struct timespec ts)
{
	char	*str;
	char	**split;
	time_t	now;

	now = time(NULL);
	if (!(str = ft_strdup(ctime(&(ts.tv_sec)))))
		return (1);
	if (!(split = ft_strsplit(str, ' ')))
	{
		free(str);
		return (1);
	}
	free(str);
	if (now - ts.tv_sec > 15723950 || now < ts.tv_sec)
		return (ft_print_time_old(split));
	else
		return (ft_print_time_recent(split));
}

int		ft_print_time(t_file *file, t_lflags *lflags)
{
	if (lflags->last_access_flag)
		return (ft_print_time_spec(file->stat.st_atimespec));
	else
		return (ft_print_time_spec(file->stat.st_mtimespec));
}
