#include <fstream>
#include "City.h"

using namespace godot;

City::City() {
	income = 0;
	population = 50000;
	numberOfEmplyees = 0;
	carbonEmission = 0;
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

void City::add_building(Struc* struc) {
	buildings.insert(struc);
}

void City::simulation() {
	//write the old values in a file 
	income = 0;
	numberOfEmplyees = 0;
	carbon = 0;
	energyDemand = 0;
	energySupply = 0;

	for (std::set<Struc*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		income += (*it)->income;
		numberOfEmplyees += (*it)->numberOfEmplyees;
		carbon += (*it)->carbon;
		energyDemand += (*it)->energyDemand;
		energySupply += (*it)->energySupply;
		(*it)->simulate_step(); //function that updates the building
	}
}

void City::write_stat_history_to_file() {
	std::ofstream out_file;
	out_file.open("stat_history.txt", std::ofstream::out | std::ofstream::app);
	out_file << timer << " " << income << " " << population << " " << numberOfEmplyees << " ";
	out_file << carbon << " " << energyDemand << " " << energySupply << std::endl;
	out_file.close();
}


float City::return_income() {
	return income;
}
