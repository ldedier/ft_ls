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

void __attribute__((destructor)) end();

void	end(void)
{
//	ft_printf("destructor loop\n");
//	while(1);
}

int main(int argc, char **argv)
{
	t_lflags	lflags;

	ft_init_lflags(&lflags);
	if (argc < 2)
		return (ft_process_ls_directory(&lflags, "."));
	else
	{
		if (ft_describe_options(argv[1]))
		{
			if (ft_parse_options(argv[1], &lflags))
				return (1);
			if (argc < 3)
				return (ft_process_ls_directory(&lflags, "."));
			return (ft_process_ls(&lflags, 2, argc, argv));
		}
		else
			return (ft_process_ls(&lflags, 1, argc, argv));
	}
}
