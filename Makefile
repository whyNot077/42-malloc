# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: minkim3 <minkim3@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/03/29 13:20:47 by minkim3           #+#    #+#              #
#    Updated: 2023/06/19 18:01:31 by minkim3          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

TITLE 			= Libft Malloc
NAME			= libft_malloc_$(HOSTTYPE).so
NAME_DEBUG		= libft_malloc_$(HOSTTYPE)_debug.so
NAME_BONUS      = libft_malloc_$(HOSTTYPE)_bonus.so
LIBNAME			= libft_malloc.so

CC              = cc
CFLAGS          = -Iinclude -MMD -Wall -Wextra -Werror
COMPILE_FLAGS   = -g -fsanitize=address
DEBUG_FLAGS     = -g -DDEBUG
BONUS_FLAGS     = -DBONUS
RM              = rm -f

DEBUG_SPECIFIC_FLAGS =

ifeq ($(DEBUG_SHOW),1)
DEBUG_SPECIFIC_FLAGS += -DDEBUG_SHOW
endif

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
DEBUG_OBJECTS   = $(addprefix $(TMP_DIR), $(SRC:.c=_debug.o))
BONUS_OBJECTS   = $(addprefix $(TMP_DIR), $(SRC:.c=_bonus.o))

HEADER_PATH     = include/
S_HEADER        = malloc.h
HEADER          = $(addprefix $(HEADER_PATH), $(S_HEADER))

# 컬러 코드
RED = \033[1;31m
PINK = \033[1;35m
GREEN = \033[1;32m
YELLOW = \033[1;33m
BLUE = \033[1;34m
DEFAULT = \033[0m

all: $(NAME) $(LIBNAME)

$(NAME): $(OBJECTS)
	make -C $(P_LIBFT)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) $(OBJECTS) $(LIBFT) -shared -o $@
	@printf "\n$(GREEN)$(NAME) Created!$(DEFAULT)\n"

$(NAME_DEBUG): $(DEBUG_OBJECTS)
	make -C $(P_LIBFT)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) $(DEBUG_FLAGS) $(DEBUG_SPECIFIC_FLAGS) $(DEBUG_OBJECTS) $(LIBFT) -shared -o $@
	@printf "\n$(GREEN)$(NAME_DEBUG) Created!$(DEFAULT)\n"

$(NAME_BONUS): $(BONUS_OBJECTS)
	make -C $(P_LIBFT)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) $(BONUS_FLAGS) $(BONUS_OBJECTS) $(LIBFT) -shared -o $@
	@printf "\n$(GREEN)$(NAME_BONUS) Created!$(DEFAULT)\n"

$(LIBNAME): $(NAME)
	ln -sf $(NAME) $(LIBNAME)
	@printf "\n$(GREEN)$(LIBNAME) -> $(NAME) Created!$(DEFAULT)\n"

$(TMP_DIR)%.o: $(SRCS_PATH)%.c $(HEADER)
	@mkdir -p $(TMP_DIR)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) -I$(HEADER_PATH) -MMD -c $< -o $@
	@printf "$(BLUE)[$(TITLE)] Compiling $<$(DEFAULT)\033[K\r"

$(TMP_DIR)%_debug.o: $(SRCS_PATH)%.c $(HEADER)
	@mkdir -p $(TMP_DIR)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) $(DEBUG_FLAGS) $(DEBUG_SPECIFIC_FLAGS) -I$(HEADER_PATH) -MMD -c $< -o $@
	@printf "$(BLUE)[$(TITLE)] Compiling $< in debug mode $(DEBUG_SPECIFIC_FLAGS) $(DEFAULT)\033[K\r"

$(TMP_DIR)%_bonus.o: $(SRCS_PATH)%.c $(HEADER)
	@mkdir -p $(TMP_DIR)
	$(CC) $(CFLAGS) $(COMPILE_FLAGS) $(BONUS_FLAGS) -I$(HEADER_PATH) -MMD -c $< -o $@
	@printf "$(BLUE)[$(TITLE)] Compiling $< in bonus mode $(DEFAULT)\033[K\r"

clean:
	$(RM) $(OBJECTS) $(DEBUG_OBJECTS) $(BONUS_OBJECTS)
	$(RM) $(OBJECTS:.o=.d)
	$(RM) $(DEBUG_OBJECTS:.o=.d)
	$(RM) $(BONUS_OBJECTS:.o=.d)
	make clean -C $(P_LIBFT)
	@printf "$(BLUE)[$(TITLE)] Clean [OK]$(DEFAULT)\n"

fclean: clean
	$(RM) $(NAME) $(NAME_DEBUG) $(NAME_BONUS) $(LIBNAME)
	make fclean -C $(P_LIBFT)
	@printf "$(BLUE)[$(TITLE)] Fclean [OK]$(DEFAULT)\n"

re:
	make fclean
	make all

debug: DEBUG_SPECIFIC_FLAGS += $(DEBUG_FLAGS)
debug: $(NAME_DEBUG)
	@printf "\n$(YELLOW)Debug mode $(DEBUG_SPECIFIC_FLAGS) $(DEFAULT)\n"

bonus: $(NAME_BONUS)
	@printf "\n$(YELLOW)Bonus mode $(DEFAULT)\n"

.PHONY: all clean fclean re debug bonus
.SILENT:

-include $(OBJECTS:.o:.d)
-include $(DEBUG_OBJECTS:.o:.d)
-include $(BONUS_OBJECTS:.o:.d)
