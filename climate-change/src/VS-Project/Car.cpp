#include "Car.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <Timer.hpp>
#include <time.h>
# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;

void Car::_register_methods()
{
	register_method((char*)"_process", &Car::_process);
	register_method((char*)"_ready", &Car::_ready);
	register_method((char*)"_init", &Car::_init);
}

void Car::_init() {
	motion = Vector3(0, 0, 0);
	position = 0;
	rot = (M_PI / 2);
	center = Vector3(0, 0, 0);
	dir = 0;

}

void Car::_process(float delta)
{
	if (rot >= (M_PI / 2)) {
		straight(delta);

		if (position >= (26.0 - (2.0 * Turn_R))) {
			rot = 0;
			dir = rand() % 3 - 1;
			//dir = 1;
			if (dir == 1) { center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z)*Turn_R*dir;	}
			else { center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z) * (Turn_R + 4) * dir; }
		}
	}
	
	
	if (position >= (26.0 - (2.0 * Turn_R))) {
		if (dir == 0) {
			straight(delta);
			if ((position >= 30)) {
				position = 0;
				rot = M_PI / 2;
			}
		}
		
		else {
			turn(dir, delta);

			if (rot >= (M_PI / 2)) {
				//make sure the angle is correct
				this->set_rotation_degrees(Vector3(round(this->get_rotation_degrees().x/90)*90, round(this->get_rotation_degrees().y/90)*90, round(this->get_rotation_degrees().z/90)*90));

				motion = this->get_global_transform().get_origin();
				motion.x = round(motion.x); motion.y = round(motion.y);
				this->set_translation(motion);							//make sure the car is on the grid
				position = 0;
			}
		}
	}
	
}

void Car::turn(int dir, float delta)
{
	double drot;
	if (dir == 1) { drot = (SPEED_T * delta) * M_PI /2; }
	else { drot = (SPEED_T * delta) * M_PI /2; }
	rot += drot;
	
	this->global_translate(-center);


	this->set_transform(this->get_transform().rotated(Vector3(0, 1, 0), -drot*dir));
	this->global_translate(center);

	if ((180 / M_PI) * rot < 30) {
		((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, -(180 / M_PI) * rot * dir * 1.5, 0));
		((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, -(180 / M_PI) * rot * dir * 1.5, 0));
	}
	if ((180 / M_PI) * rot > 60) {
		((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, ((180 / M_PI) * rot - 90) * dir * 1.5, 0));
		((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, ((180 / M_PI) * rot - 90) * dir * 1.5, 0));
	}

}

void Car::straight(float delta)
{
	this->translate(Vector3((SPEED_T*delta), 0, 0));
	position += (SPEED_T * delta)*10;
}

void Car::_ready()
{


}

void godot::Car::_physics_process(float delta)
{

}



Car::Car()
{

}

Car::~Car()
{

}