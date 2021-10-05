/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/19 09:29:13 by pitriche          #+#    #+#             */
/*   Updated: 2021/09/22 14:10:54 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "All.hpp"
#include "General.hpp"
#include "Utils.hpp"

All	all;

static void	loop(void)
{
	all.event.update();
	all.game.update(all.time.delta / 1000000000.0f, all.event.key);
	all.disp.update(all.game);
	all.time.update();
}

int			main(void)
{
	all.init();

	while (1)
		loop();
	return (0);
}
