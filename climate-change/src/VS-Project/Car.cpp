#include "Car.h"
<<<<<<< Updated upstream
=======

>>>>>>> Stashed changes
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <Timer.hpp>
#include <time.h>
#include <cstdlib>

# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;
<<<<<<< Updated upstream
=======
using namespace std;

Car::Car()
{
	motion = Vector3(0, 0, 0);
	rot = (M_PI / 2);
	center = Vector3(0, 0, 0);
	dir = 0;
	position = 0;
	SPEED_T = 0;
	
}

Car::~Car()
{

}
>>>>>>> Stashed changes

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

void ComputeAcceleration(double& Acc, double Speed0, double Speed1, double d) {
	Acc = fmin(10 * (pow(Speed1, 2) - pow(Speed0, 2)) / (2 * d), 0);
}

<<<<<<< Updated upstream
template <typename T> void RoundPosition(T obj, Vector3 &Motion) {
=======


int Car::get_direction(Vector3 pos, double rot) {
	int rotInt = (int)((rot / 90)+4) % 4 ;
	vector<int> out;
	
	if ((int)round(pos.x / 30) >= sizeof(traffic) or (int)round(pos.z / 30) >= sizeof(traffic[0])) {
		myCity->add_car();
		this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->remove_child(this);
		myCity->add_car();
		return(0);
	}
	
	
	int i = -1;
	for (const int& n : traffic[(int)round(pos.x / 30)][(int)round(pos.z / 30)][(int)rotInt]) { //buildings[(int)round(pos.x / 30)][(int)round(pos.y / 30)][(int)rotInt])
		if (n == 1) {
			out.push_back(i);
		}
		i++;
	}
	


	if (out.size() == 0) {
		
		myCity->add_car();
		this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->remove_child( this);
		myCity->add_car();
		return(0);
	}

	return(out[rand() % out.size()]);
}

template <typename T> void round_position(T obj, Vector3 &Motion) {
>>>>>>> Stashed changes
	Vector3 motion = obj->get_global_transform().get_origin();
	motion.x = round(motion.x); motion.y = round(motion.y);
	obj->set_translation(motion);
}

template <typename T> void AlignOnAxes(T obj) {
	obj->set_rotation_degrees(Vector3(round(obj->get_rotation_degrees().x / 90) * 90, round(obj->get_rotation_degrees().y / 90) * 90, round(obj->get_rotation_degrees().z / 90) * 90));
}

void Car::_process(float delta)
{
	if (rot >= (M_PI / 2)) {
		
		straight(delta);

		int real_rot = round(this->get_rotation_degrees().y /90);

		if (position >= 13 && Acc > 0 && real_rot % 2 == 0) { ComputeAcceleration(Acc, SPEED_T, 0.2, 5);}
		else if (position >= 18 && Acc > 0 && real_rot % 2 == 1 && dir == 1) { ComputeAcceleration(Acc, SPEED_T, 0.7, 4);} //Decrease also the speed before turning right (small radius)

		ComputeSpeed(SPEED_T, Acc, delta);

		if (position >= 22 && dir == 1 or position >= 18 && dir == -1 or position >= 22 && dir == 0) {
			Acc = 0.5;	rot = 0;
			RoundPosition(this, motion);
			center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z)*Turn_R*dir;
		}
	}
	
	else if (position >= 22 && dir == 1 or position >= 18 && dir == -1 or position >= 22 && dir == 0) {

<<<<<<< Updated upstream
		ComputeSpeed(SPEED_T, Acc, delta);
		turn(dir, delta);
=======
		compute_speed(SPEED_T, Acc, delta);
		if (this->move_and_collide(Vector3(0, 0, 0), true, true, true) == NULL) {
			turn(dir, delta); }
		else {
			//godot_object* colObj = move_and_collide(Vector3(0, 0, 0), true, true, true)->get_collider();
			//Car* colNode = (Car*)(colObj->godot_instance_from_id( (int*)(colObj->get_instance_id())) ->instance());
		}
		prevPosition = this->get_global_transform().get_origin();
>>>>>>> Stashed changes

		if ((dir != 0 && rot >= (M_PI / 2)) or dir == 0) {
			rot = M_PI / 2;
			AlignOnAxes(this);
			RoundPosition(this, motion);

			switch (dir) {
			case -1: position = 8; break;
			case 0: position = -8; break;
			default: position = 4; break;
			}

			dir = -(rand() % 3 - 1);

			switch (dir) {
			case -1: Turn_R = 12; break;
			default: Turn_R = 4; break;
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
	if (this->move_and_collide(Vector3(0, 0, 0), true, true, false) != NULL) {
		
	}
	else {
		position += (SPEED_T * delta) * 10;
	}
	
	
	((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position ));
	((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));
	((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));

}

void Car::_ready()
{
<<<<<<< Updated upstream

=======
	prevPosition = this->get_global_transform().get_origin();
	myCity = (City*)(this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld"));
	//dir = get_direction(this->get_global_transform().get_origin(), this->get_rotation_degrees().y);
>>>>>>> Stashed changes

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
	position = 0;
	SPEED_T = 0;
}

Car::~Car()
{

}