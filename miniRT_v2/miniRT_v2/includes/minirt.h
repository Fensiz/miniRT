#ifndef MINIRT_H
# define MINIRT_H
# include <stddef.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include "vector.h"
# include "color.h"
# define MALLOC	2
# define FD		3
# define RANGE	101
# define SCENE	105
# define FATAL	106
# define FILE_FORMAT_ERR	107
# define SPHERE	110
# define PLANE	111
# define CYLINDER	112
# define CONE	113
# define BLACK	16711680//0x333333
# define WHITE	0xffffff

# define BUFF_SIZE 64
# define EPSILON 0.00001
# define BOUNCE_LIMIT 3

typedef struct	s_sphere
{
	t_vector	center;
	double		radius;
	int			inside;
}				t_sphere;

typedef struct	s_plane
{
	t_vector	point;
}				t_plane;


typedef struct	s_cylinder
{
	t_vector	center;
	t_vector	nv;
	double		radius;
	double		height;
	double		dist[2];
}				t_cylinder;

typedef struct	s_cone
{
	t_vector	center;
	t_vector	nv;
	double		radius;
	double		height;
}				t_cone;

union			u_figure
{
	t_sphere	sp;
	t_plane		pl;
	t_cylinder	cy;
	t_cone		co;
};

typedef struct		s_figure
{
	int				type;
	union u_figure	figure;
	int				color;
	int				specular;
	double			reflection;
	int				texture;
	t_vector		normal;
	struct s_figure	*next;
}					t_figure;
typedef struct		s_camera
{
	void			*image;
	int				init;
	int				cam_id;
	t_vector		origin;
	t_vector		direction;
	int				fov;

	char			*img_addr;
	int				bits_per_pixel;
	int				line_length;
	int				endian;
	struct s_camera	*next;
}					t_camera;

typedef struct		s_mlx
{
	void			*mlx;
	void			*window;
	t_camera		*camera;
	t_camera		*begin;
}					t_mlx;
typedef struct		s_light
{
	t_vector		origin;
	double			brightness;
	int				color;
	struct s_light	*next;
}					t_light;
typedef struct s_map
{
	int	size;
	int	*map;
	struct s_map *next;
}	t_map;
typedef struct		s_scene
{
	int				width;
	int				height;
	int				cam_count;
	t_light			*light;
	double			ambient_light;
	int				ambient_light_color;
	int				background;
	t_map			*map;
	//double			nv_map[800][600];
}					t_scene;
typedef struct		s_inter
{
	int				color;
	int				reflection_color;
	t_vector		normal;
	t_vector		old_normal;
	t_vector		point;
	t_figure		*figure;
}					t_inter;
typedef struct s_uv
{
	double	u;
	double	v;
}	t_uv;
typedef struct s_interv
{
	double		id[2];
	t_vector	ip[2];
}	t_interv;

void	load_map(t_scene *scene);
void	ft_error(int code, char *error_text);
void	ft_fatal(int i);
size_t	ft_strlen(const char *s);
char	*ft_strjoin(char const *s1, char const *s2);
void	*memalloc(unsigned int size);
int		ft_isspace(char c);
int		ft_isdigit(char c);
void	skip_spaces(char **str);
int		ft_atoi(char **str);
double	ft_atof(char **str);
void	*ft_memset(void *b, int c, size_t len);
void	init_mlx(t_mlx *mlx, t_scene *scene);
int		trace_ray(t_vector_2p ray, int depth, t_figure *lst, t_scene *scene);
void	apply_texture(t_figure *figure, t_inter *inter, t_scene *scene);
int	key_handler(int keycode, void *mlx_arr);
int	red_cross_handler(void *mlx_arr);
char	*file_to_str(int fd);

void		calc_light(t_vector_2p ray, t_inter *inter, t_scene scene, t_figure *lst);
t_vector	reflect_ray(t_vector ray, t_vector normal);

t_vector	rot_from_n_to_y1(t_vector v, t_vector n);
t_vector	rot_from_y1_to_n(t_vector v, t_vector n);

/* ubtersections */
int		solve_square_exp(double k[3], double x[2]);
double	sphere_intersection(t_vector_2p ray, t_figure *figure);
double	solve_plane(t_vector_2p ray, t_vector plane_p, t_vector plane_nv);
double	plane_intersection(t_vector_2p ray, t_figure *lst);
double	cylinder_intersection(t_vector_2p ray, t_figure *lst);
double	cy_intersection(t_vector_2p ray, t_vector *normal, t_figure *lst);
double	cone_intersection(t_vector_2p ray, t_figure *lst);
#endif
