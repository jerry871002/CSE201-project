#include "AgriculturalProduction.h"

#include <random>
using namespace godot;

/*String godot::AgriculturalProduction::class_name(){
	return "AgriculturalProduction";
}

void AgriculturalProduction::_register_methods(){
	register_method((char*)"_init", &AgriculturalProduction::_init);
	register_method((char*)"_process", &AgriculturalProduction::_process);
	register_method((char*)"_input", &AgriculturalProduction::_input);
	register_method((char*)"_ready", &AgriculturalProduction::_ready);
}

void AgriculturalProduction::_init(){

}

void AgriculturalProduction::_process(float delta){

}

void AgriculturalProduction::_input(InputEvent* e){

}

void AgriculturalProduction::_ready(){

}


AgriculturalProduction::AgriculturalProduction(){
	int agricultureType = rand()%3;
	agriculture_type(agricultureType);
}

AgriculturalProduction::AgriculturalProduction(int type){
	agriculture_type(type);
}

void AgriculturalProduction::agriculture_type(int type){
	agricultureType= type; // 0 - wheat, 1 - meat, 2 - vegetables
	switch(agricultureType){
		case(0):{ // wheat

		break;
		}

	}
}
void AgriculturalProduction::simulate_step(double days){
	switch(agricultureType){
		case(0):{ // wheat

		break;
		}
}
}
double AgriculturalProduction::get_satisfaction(){
	return this->satisfaction;
}
double AgriculturalProduction::get_co2emissions(){
	return this->CO2Emission;
}		
double AgriculturalProduction::get_energyuse(){
	return this->energyUse;
}
double AgriculturalProduction::get_environmentalcost(){
	return 0;
}

// void AgriculturalProduction::test() {
// 	std::cout << "AgriculturalProduction" << std::endl;
// }*/