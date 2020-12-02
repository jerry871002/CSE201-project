#include <fstream>
#include "City.h"

using namespace godot;

City::City() {
	
	income = 0;
	population = 50000;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
	healthcare = 0;
	needs = 0;

	time_speed = 1;
	delta_counter = 0.0;
	timer = 0;
	day_tick = 0;

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
	buildings.push_back(struc);
}

void City::simulation() {
	day_tick++;
	//write the old values in a file

	for (std::vector<Struc*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		income += (*it)->income;
		numberOfEmployees += (*it)->numberOfEmployees;
		carbonEmission += (*it)->carbonEmission;
		energyDemand += (*it)->energyDemand;
		energySupply += (*it)->energySupply;
		healthcare += (*it)->healthcare;
		needs += (*it)->needs;
		(*it)->simulate_step(); //function that updates the building
	}
}

void City::write_stat_history_to_file() {
	std::ofstream out_file;
	out_file.open("stat_history.txt", std::ofstream::out | std::ofstream::app);
	out_file << timer << " " << income << " " << population << " " << numberOfEmployees << " ";
	out_file << carbonEmission << " " << energyDemand << " " << energySupply << " " << healthcare << " " << needs << std::endl;
	out_file.close();
}


double City::return_income() {
	return income;
}

double City::return_numberOfEmployees() {
	return numberOfEmployees;
}

double City::return_carbonEmission() {
	return carbonEmission;
}

double City::return_energyDemand() {
	return energyDemand;
}

double City::return_energySupply() {
	return energySupply;
}

double City::return_healthcare() {
	return healthcare;
}

double City::return_needs() {
	return needs;
}




std::string City::return_game_date() {
	std::string date = "Year ";
	date += std::to_string((day_tick / 365)+1) + ", ";
	int temp = day_tick % 365;
	if (day_tick % 365 == 0) { temp = 365; }
	if (temp <= 31) {
		date += "January " + std::to_string(temp);
		return date;
	}
	temp -= 31;
	if (temp <= 28) {
		date += "February " + std::to_string(temp);
		return date;
	}
	temp -= 28;
	if (temp <= 31) {
		date += "March " + std::to_string(temp);
		return date;
	}
	temp -= 31;
	if (temp <= 30) {
		date += "April " + std::to_string(temp);
		return date;
	}
	temp -= 30;
	if (temp <= 31) {
		date += "May " + std::to_string(temp);
		return date;
	}
	temp -= 31;
	if (temp <= 30) {
		date += "June " + std::to_string(temp);
		return date;
	}
	temp -= 30;
	if (temp <= 31) {
		date += "July " + std::to_string(temp);
		return date;
	}
	temp -= 31;
	if (temp <= 31) {
		date += "August " + std::to_string(temp);
		return date;
	}
	temp -= 31;
	if (temp <= 30) {
		date += "September " + std::to_string(temp);
		return date;
	}
	temp -= 30;
	if (temp <= 31) {
		date += "October " + std::to_string(temp);
		return date;
	}
	temp -= 31;

	if (temp <= 30) {
		date += "November " + std::to_string(temp);
		return date;
	}
	temp -= 30;

	if (temp <= 31) {
		date += "December " + std::to_string(temp);
		return date;
	}
	return "Time Representation Error";

}
