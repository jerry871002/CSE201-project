#include "Car.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;

void Car::_register_methods()
{
	register_method((char*)"_process", &Car::_process);
	register_method((char*)"_ready", &Car::_ready);
	register_method((char*)"_init", &Car::_init);
}

void Car::_init() {
	motion = Vector3(SPEED_T, 0, 0);
	position = 0;
	rot = (M_PI / 2);
	center = Vector3(0, 0, 0);

}

void Car::_process(float delta)
{
	int dir = -1;

	if (rot >= (M_PI / 2)) {
		straight(delta);
		if (position >= (26.0 - (2.0 * Turn_R))) {
			rot = 0;
			center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z)*Turn_R*dir;
		}
	}
	
	
	if (position >= (26.0 - (2.0 * Turn_R))) {
		turn(dir, delta);
		if (rot >= (M_PI/2)) {
			this->rotate_y((M_PI / 2) - rot);
			if (dir == -1 && round((Vector3(0, 1, 0).dot(this->get_global_transform().get_basis().y))) == 0) { position = -8; } // && (Vector3(1, 0, 0).dot(this->get_global_transform().get_basis().y)) == 0
			else { position = 0;  }
		}
	}
	
}

void Car::turn(int dir, float delta)
{
	rot += (SPEED_T * delta) * M_PI;
	
	this->global_translate(-center);
	this->set_transform(this->get_transform().rotated(Vector3(0, 1, 0), -dir * (SPEED_T * delta) * M_PI));
	this->global_translate(center);

	if ((180 / M_PI) * rot < 40) {
		((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, -(180 / M_PI) * rot * dir, 0));
		((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, -(180 / M_PI) * rot * dir, 0));
	}
	if ((180 / M_PI) * rot > 50) {
		((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, ((180 / M_PI) * rot - 90) * dir, 0));
		((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, ((180 / M_PI) * rot - 90) * dir, 0));
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