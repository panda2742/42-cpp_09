# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: panda <panda@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 15:02:31 by ehosta            #+#    #+#              #
#    Updated: 2025/08/09 22:00:48 by panda            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

override NAME		:=	PmergeMe

override INCLUDE_DIR	:=	include/
override TEMPLATE_DIR	:=	include/
override SOURCE_DIR		:=	src/
INCLUDES				:=	$(addprefix sort/, ASortable SortableDeque SortableList SortableVector) PmergeMe PMMException utils
TEMPLATES				:=	$(addprefix sort/, ASortable) PmergeMe
SOURCES					:=	$(addprefix sort/, SortableDeque SortableList SortableVector) main PmergeMe PMMException utils
override INCLUDE		:=	$(addprefix $(INCLUDE_DIR), $(addsuffix .hpp, $(INCLUDES)))
override TEMPLATE		:=	$(addprefix $(TEMPLATE_DIR), $(addsuffix .tpp, $(TEMPLATES)))
override SOURCE			:=	$(addprefix $(SOURCE_DIR), $(addsuffix .cpp, $(SOURCES)))

CPPFLAGS	:=	-Wall -Wextra -Werror -MD -Wshadow -g3 --std=c++98
TURBO_FLAGS	:=	-Ofast -finline-functions -m64 -D TURBO_IS_ON=1
TURBO		?=	0


ifeq ($(TURBO),1)
override NAME	:=	PmergeMe_with_turbo
CPPFLAGS		+= $(TURBO_FLAGS)
BUILD_DIR		:= .dist_turbo/
else
override NAME	:=	PmergeMe
BUILD_DIR		:= .dist/
endif


override OBJ	:=	$(patsubst $(SOURCE_DIR)%.cpp, $(BUILD_DIR)%.o, $(SOURCE))
override DEPS	:=	$(patsubst %.o, %.d, $(OBJ))
override DIRS	:=	$(sort $(dir $(NAME) $(OBJ) $(DEPS)))

MAKEFLAGS	:=	--no-print-directory
GCC			:=	c++
RM			:=	rm -rf
VG			:=	valgrind
VGFLAGS		:=	--leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ)
	$(GCC) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp $(INCLUDE) $(TEMPLATE) | $(DIRS)
	$(GCC) $(CPPFLAGS) -c -I$(INCLUDE_DIR) -I$(TEMPLATE_DIR) $< -o $@

$(DIRS):
	@mkdir -p $@

.PHONY: bonus
bonus:
	@$(MAKE) fclean
	@$(MAKE) TURBO=1

.PHONY: clean
clean:
	$(RM) .perf
	$(RM) *.profraw
	$(RM) $(BUILD_DIR)
	$(RM) .dist_turbo/

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)
	$(RM) PmergeMe_with_turbo

.PHONY: re
re: fclean
	$(MAKE)

.PHONY: run
run: bonus
	@clear
	./$(NAME) 3 5 9 7 4
	./$(NAME) "-1" "2"
	./$(NAME) "ewfwfw1" "wihfeewhf"
	./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "`

.PHONY: vg
vg: bonus
	@clear
	$(VG) $(VGFLAGS) ./$(NAME) 3 5 9 7 4
	$(VG) $(VGFLAGS) ./$(NAME) "-1" "2"
	$(VG) $(VGFLAGS) ./$(NAME) "ewfwfw1" "wihfeewhf"
	$(VG) $(VGFLAGS) ./$(NAME) 3 5 9 7 4 5
	$(VG) $(VGFLAGS) ./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "`

.PHONY: boost
boost: bonus
	@clear
	@echo "y\n" | ./scripts/perf.sh 3000
	@cat .perf

-include $(DEPS)