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
	unsigned		w:1;
	unsigned		a:1;
	unsigned		s:1;
	unsigned		d:1;
	unsigned		lshift:1;
	unsigned		space:1;

	unsigned		mouse_right:1;
	int				mouse_x;	/* only when rmb pressed if mouse_middle=1 */
	int				mouse_y;
	int				mouse_scroll;

	unsigned		mouse_middle:1;	/* toggle, 1 for Locked 0 for Unlocked */
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
		void	_keychange(SDL_Keycode kc, unsigned key_state);
		void	_mousechange(SDL_Keycode kc, unsigned key_state);

		Event(const Event &src);
};

#endif
