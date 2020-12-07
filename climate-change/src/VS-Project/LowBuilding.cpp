#include "LowBuilding.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void LowBuilding::_register_methods() {
	register_method((char*)"_init", &LowBuilding::_init);
	register_method((char*)"_process", &LowBuilding::_process);
	register_method((char*)"_input", &LowBuilding::_input);
	register_method((char*)"_ready", &LowBuilding::_ready);
}

void LowBuilding::_init() {

}

void LowBuilding::_process(float delta) {

}

void LowBuilding::_input(InputEvent* e) {

}

void LowBuilding::_ready() {

}


LowBuilding::LowBuilding() {
	cost = 1000000; //counting price of lot + cost of workforce + cost of all materials used
	energyUse = 365; // 266.25 kWh/m^2 per year which gives 133125 kWh per year i.e 365 kwH per day
	maintenance = 0.1765; //cost in euros per kWh
	CO2Emission = 0.0065; //6.5g per kWh
	buildingTime = 450; //Time it takes to build an appartment building is about 15 months 
	satisfaction = 3;
	windowNumber = 10;
}

void LowBuilding::simulate_step(double days) {
	if (solar_panel() == true) {
		// energyPerDay -= 0 ; // adding solar panels actually does nor reduce the energy used
		maintenance += solarPanelCost;

		if (satisfaction < 10) { //so that we do not end up with a satisfaction above 10
			satisfaction += 1;
		}
	}

	//assuming this is a button to change all windows at once i.e. you cannot call this function more than once on this house	
	if (double_glazing() == true) {
		energyUse *= 0.75 ; //when having better insulation of windows, you don't have the 25% loss of heat anymore
		maintenance += windowCost * windowNumber; //200€ per window, if function adds one window at a time

		if (satisfaction < 10) { //so that we do not end up with a satisfaction above 10
			satisfaction += 2;
		}
	} 
	
	maintenance += 0.1765 * energyUse * days;
	CO2Emission += 0.0065 * energyUse * days;  
	
}


LowBuilding::~LowBuilding() {

}

bool LowBuilding::solar_panel() {
	return PanelsOn;
}

bool LowBuilding::double_glazing() {
	// placeholder must change
	return false;

}