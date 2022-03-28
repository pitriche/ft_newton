/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Collider.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:13:45 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/18 15:13:45 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COLLIDER_HPP
# define COLLIDER_HPP

# include "Object.hpp"

namespace Collider
{
	/* collide ground: plane at [0,0,0] with Y axis as normal */
	void	collide_floor(Object &obj, bool wind);
	void	collide_object(Object &obj1, Object &obj2);
}

#endif
