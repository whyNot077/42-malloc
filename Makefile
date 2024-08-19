# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkim3 <minkim3@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/29 13:20:47 by minkim3           #+#    #+#              #
#    Updated: 2024/08/10 16:01:52 by minkim3          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

TITLE 			= Libft Malloc
NAME			= libft_malloc_$(HOSTTYPE).so
LIBNAME			= libft_malloc.so

CC              = cc
CFLAGS          = -Iinclude -MMD -Wall -Wextra -Werror
COMPILE_FLAGS   = -g -fsanitize=address
DEBUG_FLAGS     = -DDEBUG
BONUS_FLAGS     = -DBONUS
RM              = rm -f

DEBUG_SPECIFIC_FLAGS =
BONUS_SPECIFIC_FLAGS = 

P_LIBFT			= libft/
A_LIBFT         = libft.a
LIBFT			= $(addprefix $(P_LIBFT), $(A_LIBFT))

SRCS_PATH	    = src/
SRC             = heap.c malloc.c segregated_list.c \
show.c initialize.c vector.c free.c coalease.c mutex.c \
log.c
SOURCES			= $(addprefix $(SRCS_PATH), $(SRC))

TMP_DIR         = tmp/
OBJECTS         = $(addprefix $(TMP_DIR), $(SRC:.c=.o))

HEADER_PATH     = include/
S_HEADER        = malloc.h
HEADER          = $(addprefix $(HEADER_PATH), $(S_HEADER))

RED = \033[1;31m
PINK = \033[1;35m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
DEFAULT = \033[0m

all: $(NAME)

$(NAME): $(OBJECTS)
	make -C $(P_LIBFT)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) $(BONUS_SPECIFIC_FLAGS) $(DEBUG_SPECIFIC_FLAGS) $(OBJECTS) $(LIBFT) -shared -o $@
	@printf "\n$(GREEN)$(NAME) Created!$(DEFAULT)\n"

$(TMP_DIR)%.o: $(SRCS_PATH)%.c $(HEADER)
	@mkdir -p $(TMP_DIR)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) $(BONUS_SPECIFIC_FLAGS)  $(DEBUG_SPECIFIC_FLAGS) -I$(HEADER_PATH) -MMD -c $< -o $@
	@printf "$(BLUE)[$(TITLE)] Compiling $<$(DEFAULT)\033[K\r"

clean:
	$(RM) $(OBJECTS)
	$(RM) $(OBJECTS:.o=.d)
	make clean -C $(P_LIBFT)
	@printf "$(BLUE)[$(TITLE)] Clean [OK]$(DEFAULT)\n"

fclean: clean
	$(RM) $(NAME) $(LIBNAME)
	make fclean -C $(P_LIBFT)
	@printf "$(BLUE)[$(TITLE)] Fclean [OK]$(DEFAULT)\n"

re:
	make fclean
	make all

debug: DEBUG_SPECIFIC_FLAGS += $(DEBUG_FLAGS)
debug: $(NAME)
	@printf "\n$(YELLOW)Debug mode $(DEFAULT)\n"

bonus: BONUS_SPECIFIC_FLAGS += $(BONUS_FLAGS)
bonus: $(NAME)
	@printf "\n$(YELLOW)Bonus mode $(DEFAULT)\n"

.PHONY: all clean fclean re debug bonus
.SILENT:

-include $(OBJECTS:.o:.d)
