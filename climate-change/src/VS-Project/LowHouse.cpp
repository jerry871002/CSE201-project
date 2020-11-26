#include "LowHouse.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void LowHouse::_register_methods(){
	register_method((char*)"_process", &LowHouse::_process);
	register_method((char*)"_input", &LowHouse::_input);
	register_method((char*)"_ready", &LowHouse::_ready);
}

void LowHouse::_init(){

}

void LowHouse::_process(float delta){

}

void LowHouse::_input(InputEvent* e){

}

void LowHouse::_ready(){

}

LowHouse::LowHouse() {
//attributes from structure class
cost = 100000; //cost to build a new house (value for a low cost house, 1000€ / m^2)
energyuse = 68.49 kWh; //25000kWh per year i.e. 13.69 kWh per day (from heating and all )
maintenance = 0.1765; //cost in euros per kWh
CO2_output = 0,0065; //6.5g per kWh
building_time = 140; //in average, building a house takes about 140 days
satisfaction = 3; //assuming we are on a scale from 0 to 10
//attributes special to this class
insulation = 0.5; //I'm taking this attribute to be a percentage of insulation capacity, which is here low at first 

}

LowHouse::~LowHouse(){
}

void LowHouse::simulate_step(double days) {
	
}

