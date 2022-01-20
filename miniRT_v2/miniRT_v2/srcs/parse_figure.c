/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_figure.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bgreenbl <bgreenbl@student.21-school.      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/20 14:10:20 by bgreenbl          #+#    #+#             */
/*   Updated: 2022/01/20 14:10:21 by bgreenbl         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	parse_plane(t_figure **figure_list, char **str)
{
	t_figure	*curr;
	char		*type;

	type = "plane";
	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = PLANE;
	skip_spaces(str);
	curr->figure.pl.point = parse_vector(str);
	curr->normal = vector_norm(parse_vector(str));
	curr->color = parse_color(str);
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, type);
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, type);
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 7, type);
}

void	parse_sphere(t_figure **figure_list, char **str)
{
	t_figure	*curr;
	char		*type;

	type = "sphere";
	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = SPHERE;
	skip_spaces(str);
	curr->figure.sp.center = parse_vector(str);
	curr->figure.sp.radius = ft_atof(str) / 2;
	check_value(curr->figure.sp.radius, 0, INFINITY, type);
	curr->color = parse_color(str);
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, type);
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, type);
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 7, type);
}

void	parse_cylinder(t_figure **figure_list, char **str)
{
	t_figure	*curr;
	char		*type;

	type = "cylinder";
	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = CYLINDER;
	skip_spaces(str);
	curr->figure.cy.center = parse_vector(str);
	curr->figure.cy.nv = vector_norm(parse_vector(str));
	curr->figure.cy.radius = ft_atof(str) / 2;
	check_value(curr->figure.cy.radius, 0, INFINITY, type);
	curr->figure.cy.height = ft_atof(str);
	check_value(curr->figure.cy.height, 0, INFINITY, type);
	curr->color = parse_color(str);
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, type);
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, type);
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 7, type);
}

void	parse_cone(t_figure **figure_list, char **str)
{
	t_figure	*curr;
	char		*type;

	type = "cone";
	(*str) += 2;
	curr = ft_addback_figure(figure_list);
	curr->type = CONE;
	skip_spaces(str);
	curr->figure.co.center = parse_vector(str);
	curr->figure.co.nv = vector_norm(parse_vector(str));
	curr->figure.co.radius = ft_atof(str) / 2;
	check_value(curr->figure.co.radius, 0, INFINITY, type);
	curr->figure.co.height = ft_atof(str);
	check_value(curr->figure.co.height, 0, INFINITY, type);
	curr->color = parse_color(str);
	curr->specular = ft_atoi(str);
	check_value(curr->specular, 0, INFINITY, type);
	curr->reflection = ft_atof(str);
	check_value(curr->reflection, 0, 1, type);
	curr->texture = ft_atoi(str);
	check_value(curr->texture, 0, 7, type);
}
