#include "Windmill.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Windmill::_register_methods()
{
	register_method((char*)"_init", &Windmill::_init);
	register_method((char*)"_process", &Windmill::_process);
	register_method((char*)"_input", &Windmill::_input);
	register_method((char*)"_ready", &Windmill::_ready);
}

void Windmill::_init()
{

}

void Windmill::_process(float delta)
{

}

void Windmill::_input(InputEvent* e)
{

}

void Windmill::_ready()
{

}


Windmill::Windmill()
{

}

Windmill::~Windmill()
{
	maintenance = 38; //maintenace and working cost in euros per kWh
	//employment = 800; // approximate number of employees for a standard wind farm
	//satisfaction = 2; // on scale of 10
	//CO2Emission = 0.012; // kg of CO2 emitted per kWh
	//cost = 10E9; // cost in euros to build a new plant
	//buildingTime = 5; // years needed to build a new plant
	environmentalCost = 0.0009; // environmental and health costs in euros per kWh
}

void Windmill::simulate_step(double days)
{

}