#include "NuclearPowerPlant.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

String godot::NuclearPowerPlant::class_name()
{
	return "NuclearPowerPlant";
}
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
	age = 0; 
	radiation = 1.4E-12; //amount of radiation in millirem per kWh per person living in a 50km radius from the plant 
	//Just for perspective, coal plants actually release more radiation and an average person receives an exposure of
	//300 millirem per year from natural background sources of radiation
	nuclearWaste = 3.4E-9; //tons of high level nuclear waste produced per kWh (it is stored to be reused when radiation decays)
	naturalUranium = 3.4E-8; //tons of natural uranium necessary per kWh
	fissileMaterial = 1.4E-7; //kg of fissile material needed per kWh
	maintenance = 0.04; //maintenace and working cost in euros per kWh
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
	age += days;
	energyPerDay = 20000000; //kWh produced by standard plant in one day, we consider it to be the same for every plant in our simulation
	energyOutput += energyPerDay * days; // total kWh produced by a standard plant 
	fissileMaterial += 1.4E-7 * energyPerDay * days;
	naturalUranium += 3.4E-8 * energyPerDay * days;
	CO2Emission += 0.012 * energyPerDay * days;
	nuclearWaste += 3.4E-9 * energyPerDay * days;
	radiation += 1.4E-12 * energyPerDay * days;
	environmentalCost = 0.019 * energyPerDay * days;
	maintenance += 0.04 * energyPerDay * days;
	if (age >= 3650) {
		maintenance += 0.04 * 0.25; // after 10 years the maintenance and working costs increase by 1/4
	}
	if (age >= 10950) {
		maintenance += 0.04; // after 30 years the maintenance and working costs double
	}
	/*
	if age >= 127890{
		// 35 years is the average lifetime of a nuclear power plant, it then has to be replaced by a new plant or different power plant
	}*/
}