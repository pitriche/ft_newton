/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Event.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: brunomartin <brunomartin@student.42.fr>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 18:57:00 by pitriche          #+#    #+#             */
/*   Updated: 2021/03/20 22:38:29 by brunomartin      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EVENT_HPP
# define EVENT_HPP

# include "SDL.h"

struct Keys
{
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	lshift;
	bool	space;

	bool	mouse_right;
	int		mouse_x;		/* only when rmb pressed if mouse_middle = 1 */
	int		mouse_y;
	int		mouse_scroll;

	bool	mouse_middle;	/* toggle, 1 for Locked 0 for Unlocked */

	/* object throw */
	bool		object_thrown;
	bool		object_type;	/* 0 for Cube, 1 for Sphere */
	bool		object_auto;	/* fire every frame */

	unsigned	object_number;	/* for shotgun */
	float		object_speed;
	float		object_mass;
	float		object_size;

	float		max_distance;	/* objects beyound this will be removed */
	float		time_speed;
	float		gravity;

};

/*
** Interface to event library, with currently pressed keys
*/
struct Event
{
	public :
		Event(void);
		virtual ~Event(void);

		Keys	key;

		void	update(void);

	protected:
	private:
		void	_keychange(SDL_Keycode kc, bool key_state);
		void	_mousechange(Uint8 button, bool button_state);

		Event(const Event &src);
};

#endif
