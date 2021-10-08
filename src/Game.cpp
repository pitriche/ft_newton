/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/10/08 17:56:55 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include <cmath>		/* sqrt */
#include "Collider.hpp"	/* all collisions */

Game::Game(void) : game_speed(1.0f), gravity(9.80665f) { }
Game::~Game(void) { }

/* ########################################################################## */
/* #####################		Objects init			##################### */
/* ########################################################################## */


static void	_add_cube(std::vector<Object> &list, vec3 position, vec3 dimension,
	vec3 rotation, float mass)
{
	Object	tmp;

	tmp.type = Cube;
	tmp.position = position;
	tmp.dimension = dimension;
	tmp.angular_position = rotation;
	tmp.mass = mass;
	tmp.angular_velocity = {0, 0, 0};
	tmp.velocity = {0, 0, 0};
	tmp.radius = vec3_length(tmp.dimension) / 2.0f;
	list.push_back(tmp);
}

/* add axis aligned, immobile cube of mass 1 */
static void	_add_cube(std::vector<Object> &list, vec3 position, float side)
{ _add_cube(list, position, {side, side, side}, {0.0f, 0.0f, 0.0f}, 1.0f); }

static void	_add_sphere(std::vector<Object> &list, vec3 position, float diameter,
	float mass)
{
	Object	tmp;

	tmp.type = Sphere;
	tmp.position = position;
	tmp.dimension = {0, 0, 0};			/* diameter used */
	tmp.angular_position = {0, 0, 0};	/* no rotation */
	tmp.mass = mass;
	tmp.angular_velocity = {0, 0, 0};
	tmp.velocity = {0, 0, 0};
	tmp.radius = diameter / 2.0f;
	list.push_back(tmp);
}

void	Game::init(void)
{
	Object	tmp;

	/* initial camera position, unlocked */
	SDL_SetRelativeMouseMode(SDL_TRUE);
	this->pos[2] = -4.0f;
	this->pos[1] = 1.0f;
	this->pos_locked = 8.0f;


	for (unsigned y = 5; y < 50; y += 5)
		_add_sphere(this->obj, {0, (float)y, 0}, 2, 1);
	_add_sphere(this->obj, {0, 5, 0}, 2, 1);

	_add_cube(this->obj, {-1, 2.2f, 0}, 1);
	this->obj.back().angular_velocity = {1.4f, 1.3f, 1.5f};
	this->obj.back().velocity = {0, 30.3f, -0};

	const int MULT = 8;
	for (unsigned k = 0; k < MULT; ++k) for (unsigned j = 0; j < MULT; ++j) for (unsigned i = 0; i < MULT; ++i)
	{
		_add_cube(this->obj, {k * 1.0f + 3.0f, j * 1.0f + 0.5f, i * 1.0f}, 0.5f);
		_add_sphere(this->obj, {k * -1.0f - 3.0f, j * 1.0f + 0.5f, i * 1.0f + j * 0.001f}, 0.75f, 1);
	}
	/* cardinal boxes */
	// _add_cube(this->obj, {3, 0, 0}, {0.5f, 2, 0.5f}, {0, 0, 1.57f}, 1);
	// _add_cube(this->obj, {0, 0, 3}, {0.2f, 4, 0.2f}, {1.57f, 0, 0}, 1);
	// _add_cube(this->obj, {0, 3, 0}, {0.6f, 3, 0.6f}, {0, 0, 0}, 1);

	_add_cube(this->obj, {0, 3, -5}, {1, 1, 1}, {0.615f, 0, (float)M_PI_4}, 1);
	this->obj.back().angular_velocity = {0, 50, 0};
	_add_cube(this->obj, {0, 5, -5}, {1, 1, 1}, {0.615f, 0, (float)M_PI_4}, 1);
	_add_cube(this->obj, {0, 7, -5}, {1, 1, 1}, {0.615f, 0, (float)M_PI_4}, 1);
	this->obj.back().angular_velocity = {0, 50, 0};
	_add_sphere(this->obj, {0, 9, -5}, 1, 1.5f);
}

/* ########################################################################## */
/* #####################		Main game logic			##################### */
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

/* ########################################################################## */

static bool _compare_z(const Object &a, const Object &b)
{ return (a.position[1] < b.position[1]); }

void		Game::_update_objects(float delta)
{
	int	obj_id;

	/* sort by Z */
	std::sort(this->obj.begin(), this->obj.end(), _compare_z);
	obj_id = 0;
	for (Object &obj : this->obj)
	{
		if (!obj.rest)
			obj.velocity[1] -= this->gravity * delta;

		/* collisions */
		Collider::collide_floor(obj, delta);
		for (unsigned i = (unsigned)obj_id + 1; i < this->obj.size(); ++i)
			Collider::collide_object(obj, this->obj[i]);
		++obj_id;

		/* update position and rotation */
		obj.position += obj.velocity * delta;
		obj.angular_position += obj.angular_velocity * delta;
	}
}

/* ########################################################################## */

void		Game::update(float delta, const Keys &key)
{
	/* cap delta time */
	if (delta > MAX_DELTA)
		delta = MAX_DELTA;
	this->_update_camera(delta, key);
	this->_update_objects(delta);
}
