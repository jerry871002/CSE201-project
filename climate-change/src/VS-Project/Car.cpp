#include "Car.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <Timer.hpp>
#include <time.h>
#include <cstdlib>
# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;

void Car::_register_methods()
{
	register_method((char*)"_process", &Car::_process);
	register_method((char*)"_ready", &Car::_ready);
	register_method((char*)"_init", &Car::_init);
}

void Car::_init() {
	
	
}


void ComputeSpeed(double &Speed, double Acc, float delta) {
	if ((Speed <= 2 && Acc>0) or (Acc < 0 and Speed + Acc * delta > 0.2)) { Speed += Acc * delta; }			//Define max speed and min speed
}

void Car::_process(float delta)
{
	if (rot >= (M_PI / 2)) {
		
		straight(delta);

		if (position >= 13 && Acc == 2 && (this->get_rotation_degrees().y == 0 or this->get_rotation_degrees().y == 180)) {
			Acc = fmin((pow(0.2, 2) - 10 * pow(SPEED_T, 2)) / (2 * 6), 0);
		}
		else if (position >= 13 && Acc == 2 && (this->get_rotation_degrees().y == 90 or this->get_rotation_degrees().y == 270)  && dir == 1) {		
			Acc = fmin((pow(4, 2) - 10 * pow(SPEED_T, 2)) / (2 * 6), 0);			//Decrease also the speed before turning right (small radius)
		}

		ComputeSpeed(SPEED_T, Acc, delta);

		if (position >= 22 && dir == 1 or position >= 18 && dir == -1 or position >= 22 && dir == 0) {
			Acc = 2;
			rot = 0;
			motion = this->get_global_transform().get_origin();
			motion.x = round(motion.x); motion.y = round(motion.y);
			this->set_translation(motion);

			center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z)*Turn_R*dir;

			
		}
	}
	
	else if (position >= 26.0 - Turn_R && dir == 1 or position >= 30.0 - Turn_R && dir == -1 or position >= 26.0 - Turn_R && dir == 0) {
		
		if (dir == 0) {
			position = -8 ;
			rot = M_PI / 2;
			dir = -(rand() % 3 - 1);
			if (dir == -1) {
				Turn_R = 12;
			}
			else { Turn_R = 4; }
		}
		
		else {
			ComputeSpeed(SPEED_T, Acc, delta);

			turn(dir, delta);

			if (rot >= (M_PI / 2)) {
				// Make sure the angle is correct
				this->set_rotation_degrees(Vector3(round(this->get_rotation_degrees().x/90)*90, round(this->get_rotation_degrees().y/90)*90, round(this->get_rotation_degrees().z/90)*90));
				// Make sure the car is on the grid
				motion = this->get_global_transform().get_origin();
				motion.x = round(motion.x); motion.y = round(motion.y);
				this->set_translation(motion);	

				if (dir == -1) { position = 8; }
				else { position = 4; }

				dir = -(rand() % 3 - 1);

				if (dir == -1) {
					Turn_R = 12;
				}
				else { Turn_R = 4; }

			}
		}
	}
	
}

void Car::turn(int dir, float delta)
{
	double drot = (SPEED_T * delta) * 10;
	if (dir == 1) { drot /= 4; }
	else { drot /=  12 ; }
	rot += drot;
	
	this->global_translate(-center);			//define the center of rotation
	this->set_transform(this->get_transform().rotated(Vector3(0, 1, 0), -drot*dir));
	this->global_translate(center);				//reset the center of rotation

	if (dir == 1) {
		((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, -(90 / M_PI) * sin(rot* 2) , -(180 / M_PI) * rot * 4));
		((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, -(90 / M_PI)  * sin(rot * 2), -(180 / M_PI) * rot * 4));
		((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(90 / M_PI)  * sin(rot * 2) * 4));
	}
	else {
		((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, (90 / M_PI)  * sin(rot * 2) , -(180 / M_PI) * rot * 12));
		((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, (90 / M_PI)  * sin(rot * 2) , -(180 / M_PI) * rot * 12));
		((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * rot * 12));
	}
}

void Car::straight(float delta)
{
	this->translate(Vector3((SPEED_T*delta), 0, 0));
	position += (SPEED_T * delta)*10;
	((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position ));
	((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));
	((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));

}

void Car::_ready()
{


}

void godot::Car::_physics_process(float delta)
{

}



Car::Car()
{
	motion = Vector3(0, 0, 0);
	rot = (M_PI / 2);
	center = Vector3(0, 0, 0);
	dir = 1;
	Turn_R = 4;
	position = 0;
	SPEED_T = 0;
}

Car::~Car()
{

}