#include "CoalPowerPlant.h"
// #include <Math.hpp> do npt have that file on my directory 
#include <GodotGlobal.hpp>

using namespace godot;

String godot::CoalPowerPlant::class_name()
{
	return "CoalPowerPlant";
}
void CoalPowerPlant::_register_methods()
{
	register_method((char*)"_init", &CoalPowerPlant::_init);
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
	age = 0; // age of the building in days
	coal = 4.06E-4; // tons of coal needed to produce 1 kWh
	maintenance = 0.05; //maintenace and working cost in euros per kWh
	employment = 800; // approximate number of employees in 1 plant 
	satisfaction = 4; // on scale of 10
	CO2Emission = 0.868; // kg of CO2 emitted per kWh
	SO2Emission = 0.00152; // kg of SO2 emitted per kWh
	NOxEmission = 8.49E-4; // kg of NOx emitted per kWh
	ashOutput = 0.0619; // kg of ash produced per kWh
	mercuryEmission = 1.137E-8; // kg of mercury emitted per kWh
	cost = 3E9; // cost in euros to build a new plant
	buildingTime = 8; // years needed to build a new plant
	environmentalCost = 0.06; // environmental and health costs in euros per kWh
}


CoalPowerPlant::~CoalPowerPlant()
{

}

void CoalPowerPlant::simulate_step(double days) 
{
	age += days;
	energyPerDay = 9589041; //kWh produced by standard plant in one day, we consider it to be the same for every plant in our simulation
	energyOutput += energyPerDay * days; // total kWh produced by a standard plant 
	if (efficiency_supercritical() == true) {
		energyPerDay = 9589041 * (1 - 0.04);
		maintenance += 0.054 * energyPerDay * days;
	}
	if (efficiency_cogeneration() == true) {
		energyPerDay = 9589041 * (1 - 0.09);
		maintenance += 0.058 * energyPerDay * days;
	} 
	else {
		maintenance += 0.05 * energyPerDay * days;
	}
	if (age >= 3650) { 
		maintenance += 0.05 * 0.25; // after 10 years the maintenance and working costs increase by 1/4
	} 
	if (age >= 10950) {
		maintenance += 0.05; // after 30 years the maintenance and working costs double
	}

	coal += 4.06E-4 * energyPerDay * days;
	CO2Emission += 0.768 * energyPerDay * days;  
	SO2Emission += 0.00152 * energyPerDay * days; 
	NOxEmission += 8.49E-4 * energyPerDay * days; 
	ashOutput += 0.0619 * energyPerDay * days; 
	mercuryEmission += 1.137E-8 * energyPerDay * days;
	environmentalCost = 0.06 * energyPerDay * days;

	/*
	if age >= 18250{
		// 50 years is the average lifetime of a coal fired plant, it then has to be replaced by a new coal plant or different power plant
	}*/
}