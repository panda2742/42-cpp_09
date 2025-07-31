# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ehosta <ehosta@student.42lyon.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/19 15:02:31 by ehosta            #+#    #+#              #
#    Updated: 2025/07/26 06:32:58 by ehosta           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

override NAME	:=	PmergeMe

override SOURCE_DIR		:=	src/
override INCLUDE_DIR	:=	include/
INCLUDES				:=	PmergeMe
SOURCES					:=	main PmergeMe
override SOURCE			:=	$(addprefix $(SOURCE_DIR), $(addsuffix .cpp, $(SOURCES)))
override INCLUDE		:=	$(addprefix $(INCLUDE_DIR), $(addsuffix .hpp, $(INCLUDES)))

BUILD_DIR		:=	.dist/
override OBJ	:=	$(patsubst $(SOURCE_DIR)%.cpp, $(BUILD_DIR)%.o, $(SOURCE))
override DEPS	:=	$(patsubst %.o, %.d, $(OBJ))
override DIRS	:=	$(sort $(dir $(NAME) $(OBJ) $(DEPS)))

CPPFLAGS	:=	-Wall -Wextra -Werror -MD -Wshadow --std=c++98 -g3
MAKEFLAGS	:=	--no-print-directory
GCC			:=	c++
RM			:=	rm -rf
VG			:=	valgrind
VGFLAGS		:=	--leak-check=full --show-leak-kinds=all --track-origins=yes --show-mismatched-frees=yes --track-fds=yes --trace-children=yes

.PHONY: all
all: $(NAME)

$(NAME): $(OBJ)
	$(GCC) $(CPPFLAGS) $(OBJ) -o $(NAME)

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.cpp Makefile $(INCLUDE) | $(DIRS)
	$(GCC) $(CPPFLAGS) -c -I$(INCLUDE_DIR) $< -o $@

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
	./$(NAME) `shuf -i 1-100000 -n 3000 | tr "\n" " "`
	./$(NAME)  "-1" "2"

.PHONY: vg
vg:
	@clear
	@$(MAKE) bonus
	@clear
	$(VG) $(VGFLAGS) ./$(NAME) "8 9 * 9 - 9 - 9 - 4 - 1 +"
	$(VG) $(VGFLAGS) ./$(NAME) "7 7 * 7 -"
	$(VG) $(VGFLAGS) ./$(NAME) "1 2 * 2 / 2 * 2 4 - +"
	$(VG) $(VGFLAGS) ./$(NAME) "1 * 2 / 2 * 2 4 - +"
	$(VG) $(VGFLAGS) ./$(NAME) "* 2 / 2 * 2 4 - +"
	$(VG) $(VGFLAGS) ./$(NAME) "(1 + 1)"

-include $(DEPS)