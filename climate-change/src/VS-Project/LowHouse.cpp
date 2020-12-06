#include "LowHouse.h"
#include <Math.hpp>
#include <GodotGlobal.hpp>

using namespace godot;

void LowHouse::_register_methods(){
	register_method((char*)"_init", &LowHouse::_init);
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
	energyUse= 68.49; //25000kWh per year i.e. 13.69 kWh per day (from heating and all )
	maintenance = 0.1765; //cost in euros per kWh
	CO2Emission = 0.0065; //6.5g per kWh
	buildingTime = 140; //in average, building a house takes about 140 days
	satisfaction = 3; //assuming we are on a scale from 0 to 10
	//attributes special to this class
	windowNumber = 10; 
}

LowHouse::~LowHouse(){
}

void LowHouse::simulate_step(double days) {
	// UNCOMMENT WHEN FUNCTIONS solar_panel() AND double_glazing() ARE DONE
	
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

//For the interface team
bool LowHouse::solar_panel() {
	return PanelsOn;
}

bool LowHouse::double_glazing() {
	// placeholder must change
	return false;
}




