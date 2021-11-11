/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Matrix.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/11/10 15:56:19 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MATRIX_HPP
# define MATRIX_HPP

# include "General.hpp"
# include <iostream>
# include <array>

/*
** 4x4 32 bit floating point matrix
*/
struct Matrix : public std::array<float, 16>
{
	public :
		Matrix(void);	/* set matrix as identity */
		Matrix(const Matrix &src);
		virtual ~Matrix(void);

		Matrix	&transpose(void);
		void	set_projection(float fov, float near, float far, float aspect);
		
		Matrix	&rotate(float x, float y, float z);
		Matrix	&translate(float x, float y, float z);
		Matrix	&scale(float x, float y, float z);
		
		Matrix	&rotate(const vec3 &rotation);
		Matrix	&rotate_euler(const vec3 &euler_vector);
		Matrix	&translate(const vec3 &translation);
		Matrix	&scale(const vec3 &scaling);

		Matrix	operator*(const Matrix &rhs) const;
		Matrix	operator*(float rhs) const;

	private :
		void	_rotate_x(float angle);
		void	_rotate_y(float angle);
		void	_rotate_z(float angle);
};

std::ostream	&operator<<(std::ostream &lhs, const Matrix &rhs);

#endif
