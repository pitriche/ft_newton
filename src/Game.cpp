/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/05 17:41:26 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include "All.hpp"

Game::Game(void) : game_speed(1.0f), gravity(9.80665f) { }
Game::~Game(void) { }

void	Game::init(void)
{
	Object	tmp;

	/* initial camera position, unlocked */
	SDL_SetRelativeMouseMode(SDL_TRUE);
	this->pos[2] = -4.0f;
	this->pos[1] = 1.0f;
	this->pos_locked = 8.0f;

	tmp.type = Cube;
	tmp.mass = 1;
	tmp.position = {-1, 2.2f, 0};
	tmp.velocity = {0, 0, 0};
	tmp.angular_position = {0, 0, 0};
	tmp.angular_velocity = {1.4f, 1.3f, 1.5f};
	// tmp.dimension = {1, 1, 1};
	this->obj.push_back(tmp);

	tmp.type = Sphere;
	tmp.diameter = 0.1f;
	const int MULT = 13;
	for (unsigned k = 0; k < MULT; ++k)
		for (unsigned j = 0; j < MULT; ++j)
			for (unsigned i = 0; i < MULT; ++i)
			{
				tmp.position = {k * 1.0f, j * 1.0f + 5.0f, i * 1.0f};
				this->obj.push_back(tmp);
			}

	// tmp.position = {1, -1, 0};
	// tmp.angular_velocity = {0, 0, 1};
	// tmp.diameter = 1;
	// this->obj.push_back(tmp);

	// tmp.position = {0, 0.5, 0};
	// tmp.angular_velocity = {0.3f, 0.5f, 0.8f};
	// tmp.diameter = 1;
	// this->obj.push_back(tmp);

	// /* cardinal boxes */
	tmp.type = Cube;
	tmp.position = {3, 0, 0};
	tmp.angular_velocity = {0, 0, 0};
	tmp.dimension = {2, 0.5f, 0.5f};
	this->obj.push_back(tmp);

	tmp.position = {0, 0, 3};
	tmp.dimension = {0.2f, 0.2f, 4};
	this->obj.push_back(tmp);


	tmp.position = {0, 3, 0};
	tmp.dimension = {0.6f, 3, 0.6f};
	this->obj.push_back(tmp);


	// tmp.type = Sphere;
	// tmp.position = {0, 2, 0};
	// tmp.velocity = {0, 0, 0};
	// tmp.diameter = 10;
	// this->obj.push_back(tmp);

	tmp.type = Cube;
	tmp.position = {2, 2, 0};
	tmp.dimension = {1, 1, 1};
	this->obj.push_back(tmp);
}

/* ########################################################################## */

void		Game::_update_camera(float delta, const Keys &key)
{
	float	delta_x;
	float	delta_z;

	/* lock - unlock camera and cursor */
	if (this->camera_lock != (enum e_camera)key.mouse_middle)
	{
		this->camera_lock = key.mouse_middle ? Locked : Unlocked;
		if (this->camera_lock == Unlocked)
			SDL_SetRelativeMouseMode(SDL_TRUE);
		else if (this->camera_lock == Locked)
			SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	/* set then cap look angles */
	this->look_yaw += (key.mouse_x * MOUSE_SENSITIVITY_X) / 100000.0f;
	this->look_pitch += (key.mouse_y * MOUSE_SENSITIVITY_Y) / 100000.0f;
	if (this->look_pitch < -LOOK_CAP_PITCH)
		this->look_pitch = -LOOK_CAP_PITCH;
	if (this->look_pitch > LOOK_CAP_PITCH)
		this->look_pitch = LOOK_CAP_PITCH;
	if (this->camera_lock == Locked && this->look_pitch < LOOK_CAP_PITCH_LOCK)
		this->look_pitch = LOOK_CAP_PITCH_LOCK;

	/* set and cap positions */
	this->pos[1] += (unsigned)key.space * delta * CAMERA_SPEED -
	(unsigned)key.lshift * delta * CAMERA_SPEED;
	if (this->pos[1] < POS_CAP_Y)
		this->pos[1] = POS_CAP_Y;
	
	delta_x = (unsigned)key.d * delta * CAMERA_SPEED -
	(unsigned)key.a * delta * CAMERA_SPEED;
	delta_z = (unsigned)key.w * delta * CAMERA_SPEED -
	(unsigned)key.s * delta * CAMERA_SPEED;
	this->pos[0] += delta_z * sinf(this->look_yaw) +
	delta_x * sinf(this->look_yaw + (float)M_PI_2);
	this->pos[2] += delta_z * cosf(this->look_yaw) +
	delta_x * cosf(this->look_yaw + (float)M_PI_2);

	this->pos_locked += key.mouse_scroll / 1000.0f * SCROLL_SENSITIVITY;
	if (this->pos_locked < 0)
		this->pos_locked = 0;
}

void		Game::_update_objects(float delta, const Keys &key)
{
	(void)key;

	for (Object &obj : this->obj)
	{
		obj.position[0] += obj.velocity[0] * delta * this->game_speed;
		obj.position[1] += obj.velocity[1] * delta * this->game_speed;
		obj.position[2] += obj.velocity[2] * delta * this->game_speed;
		obj.angular_position[0] += obj.angular_velocity[0] * delta * this->game_speed;
		obj.angular_position[1] += obj.angular_velocity[1] * delta * this->game_speed;
		obj.angular_position[2] += obj.angular_velocity[2] * delta * this->game_speed;
	}

	/* wiggle all */
	// this->obj[2].position[0] = sinf(all.time.elapsed_frame() / 20.0f) * 4.8f;
	// this->obj[2].position[2] = cosf(all.time.elapsed_frame() / 20.0f) * 4.8f;
	// this->obj[1].position[0] = 1 + cosf(all.time.elapsed_frame() / 60.0f) * 1.8f;
	// this->obj[1].position[1] = sinf(all.time.elapsed_frame() / 60.0f) * 1.8f;
	// this->obj[2].dimension[0] = sinf(all.time.elapsed_frame() / 10.0f) * 1.8f;
	// this->obj[2].dimension[1] = cosf(all.time.elapsed_frame() / 8.0f) * 2.3f;
	// this->obj[2].dimension[2] = -cosf(all.time.elapsed_frame() / 3.0f) * 5.0f;
}

void		Game::update(float delta, const Keys &key)
{
	if (delta > MAX_DELTA)
		delta = MAX_DELTA;
	this->_update_camera(delta, key);
	this->_update_objects(delta, key);
}
