#include "HighHouse.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <cstdlib>
#include <ctime>

using namespace godot;

void HighHouse::_register_methods()
{
	register_method((char*)"_init", &HighHouse::_init);
	register_method((char*)"_process", &HighHouse::_process);
	register_method((char*)"_input", &HighHouse::_input);
	register_method((char*)"_ready", &HighHouse::_ready);
}

void HighHouse::_init()
{

}

void HighHouse::_process(float delta)
{

}

void HighHouse::_input(InputEvent* e)
{

}

void HighHouse::_ready()
{

}

HighHouse::HighHouse() {
	srand((int)time(0));
	//minimum wage 53 € per day - get money even on saturday and saturday but assuming working only for 
	// i take max to be 333€
	numberOfInhabitants = (rand() % (6) + 1);
	if (numberOfInhabitants >= 2) {
		//have two salaries 
		houseIncome = (rand() % (333-53)) + 53 + (rand() % (333-53)) + 53;  //random number in [53,333]
	} else {
		houseIncome = (rand() % (333-53)) + 53;
	}
	//attributes from structure class
	cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
	energyUse = 54.79; //20000kWh per year i.e. 54.79 kWh per day (from heating and all )
	maintenance = 0.1765; //cost in euros per kWh
	CO2Emission = 0.0065; //6.5g per kWh
	buildingTime = 140; //in average, building a house takes about 140 days
	satisfaction = 10; //assuming we are on a scale from 0 to 10
	age = 0;
}

void HighHouse::simulate_step(double days) {

	maintenance += 0.1765 * energyUse * days;
	CO2Emission += 0.0065 * energyUse * days;
	age += days;  
}

HighHouse::~HighHouse() {

}