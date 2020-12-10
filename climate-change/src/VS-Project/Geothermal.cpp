#include "Geothermal.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include<cstdlib>
#include<ctime>

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
	srand((int)time(0));
	double probability = (rand() % (10));
	if (probability < 2) {
		cost = 7E6; // when drilling the wells (accounts for 2 million euros approx) ther is a 20% chance of failure 
	}
	else {
		cost = 5E6; // cost in euros to build a new plant (plus all the research needed)
	}
	age = 0;
	satisfaction = 8; // on scale of 10
	CO2Emission = 0.09; // kg of CO2 emitted per kWh
	H2SEmission = 8.2E-5; //kg of H2S emitted per kWh
	CH4Emission = 1.6E-5; //kg of CH4 emitted per kWh
	NH3Emission = 1.7E-5; //kg of NH3 emitted per kWh
	maintenance = 0.08; //maintenace and working cost in euros per kWh
	buildingTime = 8; // years needed to build a new plant (plus research needed)
	environmentalCost = 0.0015; // environmental and health costs in euros per kWh
	employment = 50; // average number of employees linked to one plant
}

void Geothermal::simulate_step(double days)
{
	age += days;
	energyPerDay = 32800; //kWh produced by in one day
	energyOutput += energyPerDay * days; // total kWh produced by a standard plant 
	maintenance += 0.08*energyPerDay * days;
	if (age >= 3650) {
		maintenance += 0.08 * 0.25; // after 10 years the maintenance and working costs increase by 1/4
	}
	if (age >= 7300) {
		maintenance += 0.08; // after 20 years the maintenance and working costs double
	}
	CO2Emission += 0.09 * energyPerDay * days;
	NH3Emission += 1.7E-5 * energyPerDay * days;
	CH4Emission += 1.6E-5 * energyPerDay * days;
	H2SEmission += 8.2E-5 * energyPerDay * days;

	environmentalCost = 0.0015 * energyPerDay * days;

	/*
	if age >= 10950{
		// 30 years is the average lifetime of a geothermal plant, it then has to be replaced by a new plant or different power plant
	}*/
}