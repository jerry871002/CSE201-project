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
	radiation = 1.4E-12; //amount of radiation in millirem per kWh per person living in a 50km radius from the plant 
	//Just for perspective, coal actually plants release more radiation and an average person receives an exposure of
	//300 millirem per year from natural background sources of radiation
	nuclearWaste = 3.4E-9; //tons of high level nuclear waste produced per kWh (it is stored to be reused when radiation decays)
	naturalUranium = 3.4E-8; //tons of natural uranium necessary per kWh
	fissileMaterial = 1.4E-7; //kg of fissile material needed per kWh
	maintenance = 166; //maintenace and working cost in euros per kWh
	employment = 800; // approximate number of employees in 1 plant 
	satisfaction = 2; // on scale of 10
	CO2Emission = 0.012; // kg of CO2 emitted per kWh
	cost = 10E9; // cost in euros to build a new plant
	buildingTime = 5; // years needed to build a new plant
	environmentalCost = 0.0019; // environmental and health costs in euros per kWh
}

NuclearPowerPlant::~NuclearPowerPlant()
{

}

void NuclearPowerPlant::simulate_step(double days) 
{
	energyPerDay = 20000000; //kWh produced by standard plant in one day, we consider it to be the same for every plant in our simulation
	energyOutput += energyPerDay * days; // total kWh produced by a standard plant 
	fissileMaterial += 1.4E-7 * energyPerDay * days;
	naturalUranium += 3.4E-8 * energyPerDay * days;
	CO2Emission += 0.012 * energyPerDay * days;
	nuclearWaste += 3.4E-9 * energyPerDay * days;
	radiation += 1.4E-12 * energyPerDay * days;
	environmentalCost = 0.06 * energyPerDay * days;
	maintenance += 166 * energyPerDay * days;
	if (total_days >= 3650) {
		maintenance += 166 * 0.25; // after 10 years the maintenance and working costs increase by 1/4
	}
	if (total_days >= 10950) {
		maintenance += 166; // after 30 years the maintenance and working costs double
	}
	/*
	if total_days >= 127890{
		// 35 years is the average lifetime of a nuclear power plant, it then has to be replaced by a new plant or different power plant
	}*/
}