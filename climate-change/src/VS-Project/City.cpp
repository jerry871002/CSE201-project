#include "City.h"


using namespace godot;

City::City() {

	income = 0;
	population = 50000;
	employed = 0;
	carbon = 0;
	energyDemand = 0;
	energySupply = 0;

}

City::~City()
{

};

void City::_register_methods() 
{
	register_method((char*)"_process", &City::_process);
	register_method((char*)"_input", &City::_input);
	register_method((char*)"_ready", &City::_ready);
};

void City::_init() 
{
	
};

void City::_process(float) 
{
	
};

void City::_physics_process(float) 
{
	
};

void City::_input(InputEvent*) 
{
	
};

void City::_ready() 
{
	
};

void City::add_building(Struc* struc) 
{
	buildings.insert(struc);
}

void City::simulation() 
{
	for (std::set<Struc*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		//income += it->;
	}
}

float City::return_income() 
{
	return income;
}
