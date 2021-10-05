/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Display.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/20 18:57:00 by pitriche          #+#    #+#             */
/*   Updated: 2021/09/17 15:43:34 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DISPLAY_HPP
# define DISPLAY_HPP

# include "Game.hpp"	/* Game */
# include "SDL.h"		/* SDL_Window */

/*
** Interface to the graphic library
*/
struct Display
{
	public :
		Display(void);
		virtual ~Display(void);

		SDL_Window	*window;

		unsigned	res_x;
		unsigned	res_y;

		void		init(void);
		void		update(const Game &game);

	protected:
	private:
		Display(const Display &);
};

#endif
