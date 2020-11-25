#include "City.h"


using namespace godot;

City::City() {

	income = 0;
	population = 0;
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

void City::add_building(Structure* struc) 
{
	buildings.insert(struc);
}

void City::update_statistics() 
{
	for (std::set<Structure*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		//income += it->;
	}
}

float City::return_income() 
{
	return income;
}
