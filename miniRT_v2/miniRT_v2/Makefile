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

NAME		=	miniRT
SRC_PATH	=	./srcs/
FILES		=	utils.c		mlx_utils.c		main.c	\
				texture.c	handlers.c				\
				map_reader.c 		light_ops.c		\
				inter_plane.c		vector.c		\
				inter_sphere.c		vector_2.c		\
				inter_cylinder.c	vector_3.c		\
				inter_cylinder_2.c	color_ops.c		\
				inter_cone.c		color_ops_2.c
MLX_PATH	=	./libs/mlx19/

MLX_LIB		=	$(MLX_PATH)libmlx.a			
CC			=	gcc
CFLAGS		=	-Wall -Werror -Wextra -MMD

SRC			=	$(addprefix $(SRC_PATH), $(FILES))
OBJ			=	$(SRC:.c=.o)
DEP			=	$(SRC:.c=.d)

all			:	$(NAME)

bonus		:	all

$(NAME)		:	$(OBJ)
	$(CC) -framework OpenGL -framework AppKit -o $(NAME) $^ $(MLX_LIB)

%.o			:	%.c $(MLX_LIB)
	$(CC) $(CFLAGS) -I$(MLX_PATH) -Iincludes -c $< -o $@

$(MLX_LIB)	:
	make -C	$(MLX_PATH)
clean		:
	rm -f $(OBJ) $(DEP)
#fclean		:	clean
#	rm -f $(NAME)
#	make clean -C $(MLX_PATH)
re			:	fclean all

norm		:
	norminette srcs includes
normv		:
	@norminette -v

.PHONY		:	all bonus clean fclean re norm normv

-include $(DEP)
