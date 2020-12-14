#include "Energy.h"
#include <GodotGlobal.hpp>
#include <core/Godot.hpp>
#include <Math.hpp>

using namespace godot;

/// <summary>
/// ENERGY SUPER CLASS
/// </summary>

Energy::Energy() {

}

Energy::~Energy() {

}

/// <summary>
/// POWERPLANT CLASS
/// </summary>

NuclearPowerPlant::NuclearPowerPlant() {
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

NuclearPowerPlant::~NuclearPowerPlant() {}

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

/// <summary>
/// WINDMILL CLASS
/// </summary>

Windmill::Windmill() {
	turnSpeed = 2;

	requiredLand = 0;
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

Windmill::~Windmill() {}

void Windmill::_process(float delta)
{
	rot += delta * turnSpeed;
	((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, -130, (180 / M_PI) * rot));
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