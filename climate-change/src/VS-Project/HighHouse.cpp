#include "HighHouse.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void HighHouse::_register_methods() {
	register_method((char*)"_process", &HighHouse::_process);
	register_method((char*)"_input", &HighHouse::_input);
	register_method((char*)"_ready", &HighHouse::_ready);
}

void HighHouse::_init() {

}

void HighHouse::_process(float delta) {

}

void HighHouse::_input(InputEvent* e) {

}

void HighHouse::_ready() {

}


HighHouse::HighHouse() {
//attributes from structure class
cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
energyUse = 54.79; //20000kWh per year i.e. 54.79 kWh per day (from heating and all )
maintenance = 0.1765; //cost in euros per kWh
CO2Emission = 0.0065; //6.5g per kWh
building_time = 140; //in average, building a house takes about 140 days
satisfaction = 10; //assuming we are on a scale from 0 to 10
}

void HighHouse::simulate_step() {

	maintenance += 0.1765 * energyUse * days;
	CO2Emission += 0.0065 * energyUse * days;  
}


HighHouse::~HighHouse() {

}