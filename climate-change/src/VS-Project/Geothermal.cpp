#include "Geothermal.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void Geothermal::_register_methods()
{
	register_method((char*)"_init", &Geothermal::_init);
	register_method((char*)"_process", &Geothermal::_process);
	register_method((char*)"_input", &Geothermal::_input);
	register_method((char*)"_ready", &Geothermal::_ready);
	register_method((char*)"_init", &Geothermal::_init);
}

void Geothermal::_init()
{

}

void Geothermal::_process(float delta)
{

}

void Geothermal::_input(InputEvent* e)
{

}

void Geothermal::_ready()
{

}


Geothermal::Geothermal()
{

}

Geothermal::~Geothermal()
{
	/*
	maintenance = 7.45E-4; //maintenace and working cost in euros per kWh
	employment = 1.29; // average number of employees for one windmill
	satisfaction = 7; // on scale of 10
	CO2Emission = 0.011; // kg of CO2 emitted per kWh
	cost = 4E6; // cost in euros to build a new windmill
	buildingTime = 0.04; // years needed to build a new windmill (approximatley half a month)
	environmentalCost = 0.0009; // environmental and health costs in euros per kWh
	requiredLand = 6070; // square meters of land needed for installing one windmill
	*/
}

void Geothermal::simulate_step(double days)
{
	
}