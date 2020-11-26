#include "City.h"

using namespace godot;

City::City() {

	income = 0;
	population = 50000;
	employed = 0;
	carbon = 0;
	energyDemand = 0;
	energySupply = 0;

	time_speed = 1;
	delta_counter = 0.0;
	timer = 0;
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

void City::_physics_process(float delta) {
	delta_counter += (delta * time_speed);
	if (timer != (int64_t)delta_counter) {
		timer = (int64_t)delta_counter;
		// call city_simulate()
	}
}

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

void City::update_statistics() 
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
