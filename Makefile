# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldedier <ldedier@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/21 13:40:59 by ldedier           #+#    #+#              #
#    Updated: 2018/11/29 00:28:27 by ldedier          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= ft_ls

CC      = gcc -g3

PWD = \"$(shell pwd)\"

DEBUG ?= 0

SRCDIR   = srcs
OBJDIR   = objs
BINDIR   = .
INCLUDESDIR = includes

LIBFTDIR = libft
LIBFT_INCLUDEDIR = includes
LIBFT = $(LIBFTDIR)/libft.a

OK_COLOR = \x1b[32;01m
EOC = \033[0m

SRCS_NO_PREFIX =	main.c ft_options.c ft_options_sort.c ft_sorts.c ft_print.c\
					ft_errors.c ft_options_func.c ft_init.c ft_process_ls.c\
					ft_fill.c ft_tools.c ft_process_dir.c\
					ft_ls_frees.c ft_update_dir.c ft_long_format_left.c\
					ft_print_time.c ft_print_tools.c ft_sorts_inv.c\
					ft_print_short.c


INCLUDES_NO_PREFIX = ft_ls.h

SOURCES = $(addprefix $(SRCDIR)/, $(SRCS_NO_PREFIX))
OBJECTS = $(addprefix $(OBJDIR)/, $(SRCS_NO_PREFIX:%.c=%.o))
INCLUDES = $(addprefix $(INCLUDESDIR)/, $(INCLUDES_NO_PREFIX))

INC = -I $(INCLUDESDIR) -I $(LIBFTDIR)/$(LIBFT_INCLUDEDIR)

CFLAGS = -DPATH=$(PWD) -Wall -Wextra -Werror $(INC)
LFLAGS = -L $(LIBFTDIR) -lft

ifeq ($(DEBUG), 1)
	LFLAGS += -fsanitize=address
	CFLAGS += -DDEBUG 
	CC += -g
endif

opti:
	@make all -j

all:
	@make -C $(LIBFTDIR) all
	@make $(BINDIR)/$(NAME)

debug:
	@make opti DEBUG=1

$(LIBFT):
	@make -C $(LIBFTDIR) all

$(BINDIR)/$(NAME): $(OBJECTS) $(LIBFT)
	@$(CC) -o $@ $^ $(CFLAGS) $(LFLAGS)
	@echo "$(OK_COLOR)$(NAME) linked with success !$(EOC)"

$(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDES)
	$(CC) -c $< -o $@ $(CFLAGS) 

clean:
	@make clean -C $(LIBFTDIR)
	@rm -f $(OBJECTS)

fclean: clean
	@make fclean -C $(LIBFTDIR)
	@rm -f $(BINDIR)/$(NAME)

re: fclean all

.PHONY: all clean fclean re
