# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/11/05 00:42:18 by bgreenbl          #+#    #+#              #
#    Updated: 2022/01/19 14:01:51 by bgreenbl         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			=	miniRT
SRC_PATH		=	./srcs/
OBJ_PATH		=	./objs/
FILES			=	utils.c		mlx_utils.c		main.c	\
					handlers.c				\
					map_reader.c 		light_ops.c		parse.c			\
					inter_plane.c		vector.c		parse_param.c	\
					inter_sphere.c		vector_2.c		parse_figure.c	\
					inter_cylinder.c	vector_3.c		parse_utils.c	\
					inter_cylinder_2.c	vector_4.c		texture.c		\
					inter_cone.c		color_ops.c		texture_2.c		\
									color_ops_2.c	texture_3.c
MLX_PATH		=	./libs/mlx19/

MLX_LIB			=	$(MLX_PATH)libmlx.a
CC				=	gcc
CFLAGS			=	-Wall -Werror -Wextra -MMD

SRC				=	$(addprefix $(SRC_PATH), $(FILES))
#SRC			=	$(FILES)
OBJT			=	$(addprefix $(OBJ_PATH), $(FILES))
OBJ				=	$(OBJT:.c=.o)
DEP				=	$(OBJT:.c=.d)


all				:	make_dir $(NAME)

bonus			:	all

$(NAME)			:	$(OBJ)
	$(CC) -framework OpenGL -framework AppKit -o $(NAME) $^ $(MLX_LIB)

$(OBJ_PATH)%.o	:	$(SRC_PATH)%.c $(MLX_LIB)
	$(CC) $(CFLAGS) -I$(MLX_PATH) -Iincludes -c $< -o $@

$(OBJ_PATH)		:
	mkdir -p $@

make_dir 		:	$(OBJ_PATH)

$(MLX_LIB)		:
	make -C	$(MLX_PATH)
clean			:
	rm -f $(OBJ) $(DEP)
fclean			:	clean
	rm -f $(NAME)
	make clean -C $(MLX_PATH)
re				:	fclean all

norm			:
	norminette srcs includes
normv			:
	@norminette -v

.PHONY			:	all bonus clean fclean re norm normv make_dir

-include $(DEP)
