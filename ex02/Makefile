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

override NAME	:=	PmergeMe

override INCLUDE_DIR	:=	include/
override TEMPLATE_DIR	:=	include/
override SOURCE_DIR		:=	src/
INCLUDES				:=	$(addprefix sort/, ASortable DequeSortable ListSortable VectorSortable) PmergeMe PMMException
TEMPLATES				:=	$(addprefix sort/, ASortable) PmergeMe
SOURCES					:=	$(addprefix sort/, DequeSortable ListSortable VectorSortable) main PmergeMe PMMException
override INCLUDE		:=	$(addprefix $(INCLUDE_DIR), $(addsuffix .hpp, $(INCLUDES)))
override TEMPLATE		:=	$(addprefix $(TEMPLATE_DIR), $(addsuffix .tpp, $(TEMPLATES)))
override SOURCE			:=	$(addprefix $(SOURCE_DIR), $(addsuffix .cpp, $(SOURCES)))

BUILD_DIR		:=	.dist/
override OBJ	:=	$(patsubst $(SOURCE_DIR)%.cpp, $(BUILD_DIR)%.o, $(SOURCE))
override DEPS	:=	$(patsubst %.o, %.d, $(OBJ))
override DIRS	:=	$(sort $(dir $(NAME) $(OBJ) $(DEPS)))

CPPFLAGS	:=	-Wall -Wextra -Werror -MD -Wshadow --std=c++98
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
bonus: all

.PHONY: clean
clean:
	$(RM) $(BUILD_DIR)

.PHONY: fclean
fclean: clean
	$(RM) $(NAME)

.PHONY: re
re: fclean
	$(MAKE)

.PHONY: run
run:
	@clear
	@$(MAKE) bonus
	@clear
	./$(NAME) 3 5 9 7 4
	./$(NAME) "-1" "2"
	./$(NAME) "ewfwfw1" "wihfeewhf"
	./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "`

.PHONY: vg
vg:
	@clear
	$(MAKE) bonus
	clear
	$(VG) $(VGFLAGS) ./$(NAME) 3 5 9 7 4
	$(VG) $(VGFLAGS) ./$(NAME) "-1" "2"
	$(VG) $(VGFLAGS) ./$(NAME) "ewfwfw1" "wihfeewhf"
	$(VG) $(VGFLAGS) ./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "`

.PHONY: perf
perf:
	@clear
	$(MAKE) bonus
	clear
	@truncate perf --size 0
	@./$(NAME) `shuf -i 1-100000 -n 1000 | tr "\n" " "` >> perf
	@./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "` >> perf
	@./$(NAME) `shuf -i 1-100000 -n 10000 | tr "\n" " "` >> perf
	@./$(NAME) `shuf -i 1-100000 -n 20000 | tr "\n" " "` >> perf
	@./$(NAME) `shuf -i 1-100000 -n 50000 | tr "\n" " "` >> perf
	@./$(NAME) `shuf -i 1-1000000 -n 100000 | tr "\n" " "` >> perf
	@cat perf | awk '{if(length($$0) > 141) print substr($$0,1,50) " ..."; else print $$0}'

-include $(DEPS)