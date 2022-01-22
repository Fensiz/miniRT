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
FILES			=	free.c		mlx_utils.c		minirt.c	handlers.c	\
					map_reader.c 		light_ops.c		parse.c			\
					inter_plane.c		vector.c		parse_param.c	\
					inter_sphere.c		vector_2.c		parse_figure.c	\
					inter_cylinder.c	vector_3.c		parse_utils.c	\
					inter_cylinder_2.c	vector_4.c		texture.c		\
					inter_cone.c		color_ops.c		texture_2.c		\
					utils.c				color_ops_2.c	texture_3.c		\
					utils_2.c			utils_3.c		ray.c			\
					utils_4.c
MLX_PATH		=	./libs/mlx19/
CC				=	gcc
CFLAGS			=	-Wall -Werror -Wextra -MMD #-fsanitize=address
    
ifeq ($(shell uname -s),Linux)
CFLAGS 			+=	`pkg-config --cflags x11 xext`
LIBS 			+=	`pkg-config --libs x11 xext`
LINUX			=	-lm -lX11 -lGLU -lGL $(LIBS)
MLX_PATH		=	./libs/mlx19_linux/
endif

MLX_LIB			=	$(MLX_PATH)libmlx.a
SRC				=	$(addprefix $(SRC_PATH), $(FILES))
OBJT			=	$(addprefix $(OBJ_PATH), $(FILES))
OBJ				=	$(OBJT:.c=.o)
DEP				=	$(OBJT:.c=.d)

ifeq ($(shell uname -s),Darwin)
FRMW			=	-framework OpenGL -framework AppKit
endif

all				:	make_dir tch $(NAME)
all_mod			:	make_dir $(NAME)
	touch	.alltch
ifdef BS
ifeq ("$(wildcard .bnstch)","$(wildcard .alltch)")
tch				:	.bnstch
.bnstch			:
	@touch	.bnstch
else ifdef BS
tch				:	.bnstch
.bnstch			:
	@rm -f .alltch
	@make clean
	@touch	.bnstch
endif
else
ifeq ("$(wildcard .bnstch)","$(wildcard .alltch)")
tch				:	.alltch
.alltch			:
	@touch	.alltch
else
tch				:	.alltch
.alltch			:
	@rm -f .bnstch
	@make clean
	@touch	.alltch
endif
endif

bonus			:
	@make BS="-D BNS" all

$(NAME)			:	$(OBJ)
	$(CC) $(FRMW) -o $(NAME) $^ $(MLX_LIB) $(LIBS) $(LINUX)

$(OBJ_PATH)%.o	:	$(SRC_PATH)%.c $(MLX_LIB) Makefile
	$(CC) $(CFLAGS) -I$(MLX_PATH) -Iincludes $(BS) $(LINUX) -c $< -o $@

$(OBJ_PATH)		:
	mkdir -p $@

make_dir 		:	$(OBJ_PATH)

$(MLX_LIB)		:
	make -C	$(MLX_PATH)
	
clean			:
	rm -f $(OBJ) $(DEP)
	
fclean			:	clean
	rm -f $(NAME)
	rm -f .alltch
	rm -f .bnstch
	make clean -C $(MLX_PATH)
	
re				:	fclean all_mod

norm			:
	norminette srcs includes
normv			:
	@norminette -v

.PHONY			:	all bonus clean fclean re norm normv make_dir

-include $(DEP)
