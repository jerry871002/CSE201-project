#include "Windmill.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <Mesh.hpp>
# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;

String godot::Windmill::class_name()
{
	return "Windmill";
}

void Windmill::_register_methods()
{
	register_method((char*)"_init", &Windmill::_init);
	register_method((char*)"_process", &Windmill::_process);
	register_method((char*)"_input", &Windmill::_input);
	register_method((char*)"_ready", &Windmill::_ready);
	register_method((char*)"_init", &Windmill::_init);
}

void Windmill::_init()
{

}

void Windmill::_process(float delta)
{
	rot += delta * turnSpeed;
	((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, -130, (180 / M_PI) * rot));
}

void Windmill::_input(InputEvent* e)
{

}

void Windmill::_ready()
{

}


Windmill::Windmill()
{
	turnSpeed = 2;
	requiredLand = 0;
}

Windmill::~Windmill()
{
	age = 0;
	maintenance = 7.45E-4; //maintenace and working cost in euros per kWh
	employment = 1.29; // average number of employees for one windmill
	satisfaction = 8; // on scale of 10
	CO2Emission = 0.011; // kg of CO2 emitted per kWh
	cost = 4E6; // cost in euros to build a new windmill
	buildingTime = 0.04; // years needed to build a new windmill (approximatley half a month)
	environmentalCost = 0.0009; // environmental and health costs in euros per kWh
	requiredLand = 6070; // square meters of land needed for installing one windmill
}

void Windmill::simulate_step(double days)
{
	age += days;
	energyPerDay = 25000; //kWh produced by a standard windmill in one day (average size of 2.5MW windmill)
	energyOutput += energyPerDay * days; // total kWh produced by a standard plant 
	CO2Emission += 0.011 * energyPerDay * days;
	environmentalCost = 0.0009 * energyPerDay * days;
	maintenance += 7.45E-4 * energyPerDay * days;
	/*
	if age >= 7300{
		// 20 years is the average lifetime of a windmill, it then has to be replaced by a new one or destroyed
	}*/
}