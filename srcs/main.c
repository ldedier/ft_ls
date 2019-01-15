/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/21 13:42:56 by ldedier           #+#    #+#             */
/*   Updated: 2019/01/05 20:28:45 by ldedier          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ls.h"

void	ft_print_main_error(int ret)
{
	if (ret == 2)
		ft_printf("internal malloc error\n");
}

int		ft_process_main(int argc, char **argv)
{
	t_lflags	lflags;
	int			i;

	ft_init_lflags(&lflags);
	i = 1;
	while (i < argc && ft_describe_options(argv[i]))
	{
		if (!ft_strcmp("--", argv[i]))
		{
			i++;
			break ;
		}
		if (ft_parse_options(argv[i], &lflags))
			return (1);
		i++;
	}
	if (i == argc)
		return (ft_process_ls_directory(&lflags, ".", "."));
	else
		return (ft_process_ls(&lflags, i, argc, argv));
}

int		main(int argc, char **argv)
{
	int ret;

	if ((ret = ft_process_main(argc, argv)))
	{
		ft_print_main_error(ret);
		return (1);
	}
	return (0);
}
