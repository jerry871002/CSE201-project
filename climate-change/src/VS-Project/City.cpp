#include <fstream>
#include "City.h"
#include "Car.h"
#include "Restaurant.h"
#include <SceneTree.hpp>

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
#include <iostream>

using namespace godot;

City::City() {
	
	income = 0;
	population = 50000;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;

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
	std::cout << "HELLO" << std::endl;
	ResourceLoader* ResLo = ResourceLoader::get_singleton();

	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	//Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");

	if (RestaurantScene.is_valid() && ShopScene.is_valid())
	{
		for (int x = 0; x < 5; x++)
		{
			for (int z = 0; z < 5; z++)
			{
				int type = rand() % 2;
				Node* node;
				if (type == 0) { node = RestaurantScene->instance(); }
				else { node = ShopScene->instance(); }
				node->set("scale", Vector3(10, 10, 10));
				node->set("translation", Vector3(30 * x, 0, 30 * z));
				int rot = rand() % 2;
				//node->set("rotation_degrees", Vector3(0, 180 * rot, 0));
				this->add_child(node);
			}
		}
	}
};

void City::add_building(Structure* struc) {
	buildings.insert(struc);
}

void City::simulation() {
	day_tick++;
	//write the old values in a file 
	income = 0;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;

	for (std::set<Structure*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	{
		/* 
		commented out until we know what variables to call in every structure
		
		income += (*it)->income;
		std::cout << "in LOOP income " << (*it)->income << std::endl;
		numberOfEmployees += (*it)->numberOfEmployees;
		carbonEmission += (*it)->carbonEmission;
		energyDemand += (*it)->energyDemand;
		energySupply += (*it)->energySupply;
		(*it)->simulate_step(); //function that updates the building

		*/
	}
}

void City::write_stat_history_to_file() {
	std::ofstream out_file;
	out_file.open("stat_history.txt", std::ofstream::out | std::ofstream::app);
	out_file << timer << " " << income << " " << population << " " << numberOfEmployees << " ";
	out_file << carbonEmission << " " << energyDemand << " " << energySupply << std::endl;
	out_file.close();
}


double City::return_income() {
	return income;
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
