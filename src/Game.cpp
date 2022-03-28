/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Game.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pitriche <pitriche@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/09 11:14:54 by pitriche          #+#    #+#             */
/*   Updated: 2021/11/15 14:36:20 by pitriche         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Game.hpp"
#include <cmath>		/* sqrt */
#include <cstdlib>		/* rand */
#include "Collider.hpp"	/* all collisions */
#include "Utils.hpp"	/* square */

Game::Game(void) { }
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

/* push a cube of spheres */
static void	init_pile(std::vector<Object> &list, unsigned mult, float spacing, float size)
{
	for (unsigned k = 0; k < mult; ++k)
		for (unsigned j = 0; j < mult; ++j)
			for (unsigned i = 0; i < mult; ++i)
				_add_sphere(list, {k * spacing, j * spacing + 0.5f, i * spacing}, size, 1000);
}


static void	init_pool(std::vector<Object> &list, vec3 position)
{
	/* 10000 tons each */
	_add_cube(list, {position[0], 2, position[2] + 5}, {11, 4, 1},
		{0, 0, 0}, 10000000);
	_add_cube(list, {position[0], 2, position[2] - 5}, {11, 4, 1},
		{0, 0, 0}, 10000000);
	_add_cube(list, {position[0] + 5, 2, position[2]}, {1, 4, 9},
		{0, 0, 0}, 10000000);
	_add_cube(list, {position[0] - 5, 2, position[2]}, {1, 4, 9},
		{0, 0, 0}, 10000000);
}

static void	init_tower(std::vector<Object> &list, vec3 position)
{
	/* 100 kg each */
	position[1] = 1;
	_add_cube(list, position, {2, 2, 2}, {0, 0, 0}, 100);
	position[1] = 3;
	_add_cube(list, position, {1.9f, 2, 1.9f}, {0, 0, 0}, 100);
	position[1] = 5;
	_add_cube(list, position, {1.8f, 2, 1.8f}, {0, 0, 0}, 100);
	position[1] = 7;
	_add_cube(list, position, {1.7f, 2, 1.7f}, {0, 0, 0}, 100);
}

void		Game::init(void)
{
	Object	tmp;

	/* initial camera position, unlocked */
	SDL_SetRelativeMouseMode(SDL_TRUE);
	this->pos[2] = -7.0f;
	this->pos[1] = 1.0f;
	this->pos_locked = 8.0f;

	/* Catapult */
	_add_cube(this->sling, {-0.6f, -0.6f, 0}, {0.5f, 2, 0.5f}, {0, 0, 0.6f}, 1); /* body */
	_add_cube(this->sling, {0.6f, -0.6f, 0}, {0.5f, 2, 0.5f}, {0, 0, -0.6f}, 1);
	_add_cube(this->sling, {0, -2.4f, 0}, {0.5f, 2, 0.5f}, {0, 0, 0}, 1);
	_add_cube(this->sling, {0.6f, 0, -0.5f}, {0.25f, 0.25f, 1.5f}, {0, 0.6f, 0}, 1);	/* elastic */
	_add_cube(this->sling, {-0.6f, 0, -0.5f}, {0.25f, 0.25f, 1.5f}, {0, -0.6f, 0}, 1);
	_add_cube(this->sling, {0, 0, -1.1f}, {0.75f, 0.40f, 0.2f}, {0, 0, 0}, 1); /* pad */

	/* objects */
	init_pile(this->obj, 8, 6.0f, 5.0f);

	/* twin cube towers */
	init_tower(this->obj, {-5, 0, 5});
	init_tower(this->obj, {-8, 0, 5});

	/* piscine a boules */
	init_pool(this->obj, {-10, 0, 20});

	/* angled cube */
	_add_cube(this->obj, {-10, 2, -10}, {3, 2, 1}, {0, 1, 0}, 100);
}

/* ########################################################################## */
/* #####################		Camera update			##################### */
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

	/* set and cap positions (camera speed is time_speed independant)*/
	// delta /= key.time_speed;
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

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// this->obj[1].position = this->pos + (vec3){-1, 0, 5};
	// Line line;
	// line.origin = {-1, 1.5, 1.58579};
	// line.dir = {1, 1e-38, 1e-38};
	// line.length = 2;
	// Utils::debug_draw_line(this->debug, line);
	// std::cout << _cube_line_collision(this->obj[0], line) << "<< Fix\n";

	// line.origin = this->pos + (vec3){-1, 0, 1};
	// line.dir = {-1, 1e-10, 1e-10};
	// // vec3_rotate(line.dir, 1e-10f, -M_PI_2, 0);
	// line.length = key.object_speed;

	// Utils::debug_draw_line(this->debug, line);
	// std::cout << _cube_line_collision(this->obj[0], line) << "<< moving, line " << line << "\n";
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	this->pos_locked += key.mouse_scroll / 1000.0f * SCROLL_SENSITIVITY;
	if (this->pos_locked < 0)
		this->pos_locked = 0;
}

/* ########################################################################## */
/* #####################		Objects update			##################### */
/* ########################################################################## */

void		Game::_throw_object(const Keys &key)
{
	const float	spread = 0.15f;
	vec3		velocity;
	vec3		position;
	vec3		angular_position;
	vec3		dimension;

	if (!key.object_thrown || key.mouse_middle)	/* only on unlocked view */
		return ;
	for (unsigned i = 0; i < key.object_number; ++i)
	{
		/* velocity, add spread if multiple objects */
		if (key.object_number > 1)
		{
			velocity = {rand() * spread / INT_MAX, rand() * spread / INT_MAX,
				1.0f};
			velocity -= {spread * 0.5f, spread * 0.5f, 0.0f};/* center spread */
		}
		else
			velocity = {0.0f, 0.0f, 1.0f};
		velocity *= key.object_speed;
		vec3_rotate(velocity, this->look_pitch, this->look_yaw, 0);

		/* position, spawn object 2m in front of camera */
		position = this->pos + (velocity / (key.object_speed + 0.01f)) * 2.0f;

		/* rotate cubes to follow camera */
		angular_position = {this->look_pitch, this->look_yaw, 0};

		/* scale cubes */
		dimension = {key.object_size, key.object_size, key.object_size};

		if (!key.object_type)
			_add_cube(this->obj, position, dimension, angular_position,
				key.object_mass);
		else
			_add_sphere(this->obj, position, key.object_size, key.object_mass);
		this->obj.back().velocity = velocity;
	}
}

/* ########################################################################## */
/* #####################		Object and physics		##################### */
/* ########################################################################## */


static bool 	_compare_z(const Object &a, const Object &b)
{ return (a.position[1] < b.position[1]); }

/* compute drag coefficient from mach number, approximation of sound barrier */
static float	_compute_Cx(float mach)
{
	if (mach < 0.6f)	/* mach [0 - 0.6] -> 1*base */
		return (BASE_CX);
	if (mach < 1.0f)	/* mach [0.6 - 1] -> 1-2.2*base */
		return (BASE_CX * (1 + (-1.8f + 3 * mach)));
	if (mach < 2.0f)	/* mach [1 - 2] -> 2.2-1.8*base */
		return (BASE_CX * (1 + (1.3f - 0.2f * mach)));
	else				/* mach > 2 -> 1.8*base */
		return (BASE_CX * 1.8f);
}

/* compute air density from altitude, using Scale height (7.6km) */
static float	_compute_rho(float altitude)
{ return (AIR_DENSITY * std::exp(-altitude / 7600.0f)); }

static void		_compute_drag(Object &obj, float delta)
{
	float	v2;			/* velocity squared */
	float	v;			/* velocity */
	float	q;			/* dynamic pressure */
	float	mach;		/* mach number */
	float	area;		/* object surface area */
	float	drag;		/* drag in newtons */
	float	a;			/* acceleration (deceleration) due to drag */
	float	final_mult;	/* portion of the speed left after drag is applied */

	/* q= 1/2 * rho * V^2 */
	v2 = Utils::square(obj.velocity[0]) + Utils::square(obj.velocity[1]) +
	Utils::square(obj.velocity[2]);
	v = std::sqrt(v2);
	q = 0.5f * _compute_rho(obj.position[1]) * v2;

	/* Rx = q * A * Cx */
	area = Utils::square(obj.radius) * (float)M_PI;
	mach = v / (20.05f * std::sqrtf(AIR_TEMPERATURE));
	drag = _compute_Cx(mach) * area * q;
	a = drag / obj.mass;
	final_mult = v != 0 ? ((v - (a * delta)) / v) : 1.0f;
	Utils::float_cap(final_mult, MAX_DRAG_FRAME, 1.0f);	/* prevent reverse */
	obj.velocity *= final_mult;
	// std::cout << "Mach number=" << mach << " | Cx=" << _compute_Cx(mach) << std::endl;
}


static void		_compute_wind(Object &obj, const vec3 &wind_dir, float wind_speed,
	float delta)
{
	float	q;			/* dynamic pressure */
	float	mach;		/* mach number */
	float	area;		/* object surface area */
	float	drag;		/* drag in newtons */
	float	a;			/* acceleration due to wind drag */

	/* q= 1/2 * rho * V^2 */
	q = 0.5f * _compute_rho(obj.position[1]) * Utils::square(wind_speed);

	/* Rx = q * A * Cx */
	area = Utils::square(obj.radius) * (float)M_PI;
	mach = wind_speed / (20.05f * std::sqrtf(AIR_TEMPERATURE));
	drag = _compute_Cx(mach) * area * q;
	a = drag / obj.mass;

	obj.velocity += wind_dir * a * delta;
	std::cout << "Wind Mach number=" << mach << " | Cx=" << _compute_Cx(mach) << std::endl;
}

void			Game::_update_objects(float delta, const Keys &key)
{
	int		obj_id;
	vec3	wind_dir;

	/* number object actions */
	if (key.init_pile)
		init_pile(this->obj, 6, 3.1f, 3.0f);
	if (key.init_pool)
		init_pool(this->obj, this->pos);
	if (key.init_tower)
		init_tower(this->obj, this->pos);
	if (key.reset_all)
		this->obj.clear();

	/* sort by Z */
	std::sort(this->obj.begin(), this->obj.end(), _compare_z);

	/* remove objects beyound max distance, and corrupted objects */
	for (unsigned i = 0; i < this->obj.size(); ++i)
		if (vec3_length(this->obj[i].position - this->pos) > key.max_distance ||
			isnan(this->obj[i].position[1]) || isinf(this->obj[i].position[1]))
			this->obj.erase(this->obj.begin() + i);

	/* compute cube points and apply drag */
	wind_dir = {0.0f, 0.0f, 1.0f};
	vec3_rotate(wind_dir, this->look_pitch, this->look_yaw, 0);
	for (Object &obj : this->obj)
	{
		if (obj.type == Cube)
			obj.compute_points();
		_compute_drag(obj, delta);
		if (key.mouse_right)
			_compute_wind(obj, wind_dir, 50, delta);
	}

	obj_id = 0;
	for (Object &obj : this->obj)
	{
		obj.velocity[1] -= key.gravity * delta;

		/* collisions */
		Collider::collide_floor(obj, key.mouse_right);
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
	this->debug.clear();

	/* cap delta time */
	if (delta > MAX_DELTA)
		delta = MAX_DELTA;

	this->_update_camera(delta, key);
	delta *= key.time_speed;	/* will mess up physics at high values */
	this->_throw_object(key);
	this->_update_objects(delta, key);
}
