# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkim3 <minkim3@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/12/03 13:37:57 by minkim3           #+#    #+#              #
#    Updated: 2023/01/18 16:15:15 by minkim3          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME 		= libft.a

CC 			= cc
CFLAGS 		= -Wall -Wextra -Werror
AR          = ar -rcs
RM          = rm -f
INCS        = libft.h
SRCS = ft_memcpy.c ft_putstr.c ft_memmove.c

OBJS        = $(SRCS:.c=.o)
OBJECTS = $(OBJS)
HEADER = libft.h ft_printf.h

all: $(NAME)
	
$(NAME): $(OBJECTS)
	$(AR) $@ $^
	printf "\n$(GREEN)$(NAME) Created!$(DEFAULT)"


%.o: %.c $(INCS) $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@
	printf "$(BLUE)[Libft] Compiling $<!$(DEFAULT)\033[K\r"

clean:      
	$(RM) $(OBJS) $(OBJS_BONUS)
	printf "$(BLUE)[Libft] Clean [OK]$(DEFAULT)\n"

fclean: clean
	$(RM) $(NAME)
	printf "$(BLUE)[Libft] Fclean [OK]$(DEFAULT)\n"

re:
	make fclean
	make all

.PHONY: all clean fclean re
.SILENT:


RED = \033[0;31m
PINK = \033[1;35m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
DEFAULT = \033[0m