#include "HighBuilding.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>
#include <cstdlib>
#include <ctime>

using namespace godot;

void HighBuilding::_register_methods() {
	register_method((char*)"_init", &HighBuilding::_init);
	register_method((char*)"_process", &HighBuilding::_process);
	register_method((char*)"_input", &HighBuilding::_input);
	register_method((char*)"_ready", &HighBuilding::_ready);
}

void HighBuilding::_init() {

}

void HighBuilding::_process(float delta) {

}

void HighBuilding::_input(InputEvent* e) {

}

void HighBuilding::_ready() {

}


HighBuilding::HighBuilding() {
	//We consider a 500 m^2 building 
	srand((int)time(0));
	//5 appartments pay between 300-500 € per month
	coOwnershipBudget = (rand() % (83-50) + 50);

	cost = 1000000; //counting price of lot + cost of workforce + cost of all materials used
	energyUse = 292; // 213 kWh/m^2 per year which gives 106500 kWh per year i.e 292 kwH per day
	maintenance = 0.1765; //cost in euros per kWh
	CO2Emission = 0.0065; //6.5g per kWh
	buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
	satisfaction = 7;
	age = 0;

}

void HighBuilding::simulate_step(double days) {

	maintenance += 0.1765 * energyUse * days;
	CO2Emission += 0.0065 * energyUse * days;  
	age += days;
	
}

HighBuilding::~HighBuilding() {

}