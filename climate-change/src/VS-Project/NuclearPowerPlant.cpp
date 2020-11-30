#include "NuclearPowerPlant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void NuclearPowerPlant::_register_methods()
{
	register_method((char*)"_process", &NuclearPowerPlant::_process);
	register_method((char*)"_input", &NuclearPowerPlant::_input);
	register_method((char*)"_ready", &NuclearPowerPlant::_ready);
}

void NuclearPowerPlant::_init()
{

}

void NuclearPowerPlant::_process(float delta)
{

}

void NuclearPowerPlant::_input(InputEvent* e)
{

}

void NuclearPowerPlant::_ready()
{

}


NuclearPowerPlant::NuclearPowerPlant()
{
	maintenance = 166; //maintenace and working cost in euros per kWh
	employment = 800; // approximate number of employees in 1 plant 
	satisfaction = 2; // on scale of 10
	CO2_output = 0.012; // kg of CO2 emitted per kWh
	//mercury_output = ; // kg of mercury emitted per kWh
	cost = 10E9; // cost in euros to build a new plant
	building_time = 5; // years needed to build a new plant
	environmental_cost = 0.0019; // environmental and health costs in euros per kWh
}

NuclearPowerPlant::~NuclearPowerPlant()
{

}

void NuclearPowerPlant::simulate_step(double days) 
{
	energy_per_day = 20000000; //kWh produced by standard plant in one day, we consider it to be the same for every plant in our simulation

}