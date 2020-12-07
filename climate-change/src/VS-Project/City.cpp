
#include "City.h"
#include "Car.h"
#include "Restaurant.h"
#include <SceneTree.hpp>
#include <fstream>
#include <ResourceLoader.hpp>
#include <Resource.hpp>
#include <ResourceFormatLoader.hpp>
#include <Script.hpp>
#include <ResourcePreloader.hpp>
#include <PackedScene.hpp>
#include <Tree.hpp>
#include <Node.hpp>
#include <cstdlib>
#include <stdlib.h>

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
	register_method((char*)"_physics_process", &City::_physics_process);
	register_method((char*)"_init", &City::_init);
}

void City::_init() 
{

}

void City::_ready() 
{
	std::cout << "HELLO" << std::endl;
	ResourceLoader* ResLo = ResourceLoader::get_singleton();

	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");


	if (RestaurantScene.is_valid())
	{
		Node* node = RestaurantScene->instance();
		node->set("scale", Vector3(10, 10, 10));
		node->set("translation", Vector3(0, 0, 0));
		this->add_child(node);
		this->add_building((Restaurant*)node);
		}
}

void City::_process(float delta)
{
	
}


void City::_physics_process(float delta) {
	delta_counter += (delta * time_speed);
	if (timer != (int64_t)delta_counter ) {
		timer = (int64_t)delta_counter;
		simulation();
	}
}

void City::_input(InputEvent*) 
{
	
};


void City::add_building(Restaurant* struc) {
	buildings.push_back(struc);
}

void City::simulation() {
	day_tick++;

	// TODO: write stats into the file

	// first reset all the values, otherwise they will keep increasing
	// even if the player doesn't do anything
	income = 0;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
	healthcare = 0;
	needs = 0;

	for (std::vector<Struc*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		income += (*it)->income;
		numberOfEmployees += (*it)->numberOfEmployees;
		carbonEmission += (*it)->get_emissions();
		energyDemand += (*it)->energyDemand;
		energySupply += (*it)->energySupply;
		healthcare += (*it)->healthcare;
		needs += (*it)->needs;
		(*it)->simulate_step(); // function that updates the building
	}

	std::cout << return_game_date() << std::endl;
	std::cout << "city income is " << return_income() << std::endl;
	std::cout << "city number of employees is " << return_numberOfEmployees() << std::endl;
	std::cout << "city carbon emissions are " << return_carbonEmission() << std::endl;
	std::cout << "city energy demand is " << return_energyDemand() << std::endl;
	std::cout << "city energy supply is " << return_energySupply() << std::endl;
	std::cout << "city healthcare is " << return_healthcare() << std::endl;
	std::cout << "city needs are " << return_needs() << std::endl << std::endl;
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

