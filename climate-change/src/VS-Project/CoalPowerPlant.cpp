#include "CoalPowerPlant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void CoalPowerPlant::_register_methods()
{
	register_method((char*)"_process", &CoalPowerPlant::_process);
	register_method((char*)"_input", &CoalPowerPlant::_input);
	register_method((char*)"_ready", &CoalPowerPlant::_ready);
}

void CoalPowerPlant::_init()
{

}

void CoalPowerPlant::_process(float delta)
{

}

void CoalPowerPlant::_input(InputEvent* e)
{

}

void CoalPowerPlant::_ready()
{

}


CoalPowerPlant::CoalPowerPlant()
{
	coal = 4.06E-4 // tons of coal needed to produce 1 kWh
	maintenance = 36; //maintenace and working cost in euros per kWh
	employment = 800; // approximate number of employees in 1 plant 
	satisfaction = 5; // on scale of 10
	CO2_output = 0.868; // kg of CO2 emitted per kWh
	SO2_output = 0.00152; // kg of SO2 emitted per kWh
	NOx_output = 8.49E-4; // kg of NOx emitted per kWh
	ash_output = 0.0619; // kg of ash produced per kWh
	mercury_output = 1.137E-8; // kg of mercury emitted per kWh
	cost = 3E9; // cost in euros to build a new plant
	building_time = 8; // years needed to build a new plant
	environmental_cost = 0.06; // environmental and health costs in euros per kWh
}


CoalPowerPlant::~CoalPowerPlant()
{

}

void CoalPowerPlant::simulate_step(double days) 
{
	energy_per_day = 9589041; //kWh produced by standard plant in one day, we consider it to be the same for every plant in our simulation
	energy_output += energy_per_day * days; // total kWh produced by a standard plant 
	if efficiency_supercritical() == true{
		energy_per_day = 9589041 * (1 - 0.04);
		maintenance += 38 * energy_per_day * days;
	}
	if efficiency_cogeneration() == true{
		energy_per_day = 9589041 * (1 - 0.09);
		maintenance += 40 * energy_per_day * days;
	} 
	else {
		maintenance += 36 * energy_per_day * days;
	}
	if total_time >= 3650{ 
		maintenance += maintenance * 1.25; // after 10 years the maintenance and working costs increase by 1/4
	} 
	if total_time >= 10950{
		maintenance += maintenance * 2; // after 30 years the maintenance and working costs double
	}
	coal += 4.06E-4 * energy_per_day * days;
	CO2_output += 0.768 * energy_per_day * days;  
	SO2_output += 0.00152 * energy_per_day * days; 
	NOx_output += 8.49E-4 * energy_per_day * days; 
	ash_output += 0.0619 * energy_per_day * days; 
	mercury_output += 1.137E-8 * energy_per_day * days;
	environmental_cost = 0.06 * energy_per_day * days;

	/*
	if total_time >= 18250{
		// 50 years is the average lifetime of a coal fired plant, it then has to be replaced by a new coal plant or different power plant
	}*/
}