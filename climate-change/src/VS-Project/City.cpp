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

void City::simulation() {
	//write the old values in a file 
	income = 0;
	employed = 0;
	carbon = 0;
	energyDemand = 0;
	energySupply = 0;

	for (std::set<Struc*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		income += (*it)->income;
		employed += (*it)->employed;
		carbon += (*it)->carbon;
		energyDemand += (*it)->energyDemand;
		energySupply += (*it)->energySupply;
		(*it)->simulate_step(); //function that updates the building
	}
}

void City::update_statistics() 
{

	for (std::set<Struc*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{

	}

}

float City::return_income() 
{
	return income;
}
