#include <fstream>
#include "City.h"
#include <Resource.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include <PackedScene.hpp>
#include <Node.hpp>
<<<<<<< Updated upstream
=======
#include <ctime>
#include <Input.hpp>
#include <Button.hpp>
#include <Viewport.hpp>
#include <HSlider.hpp>
#include <TextureProgress.hpp>
#include <Color.hpp>



#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "edit_text_files.cpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

>>>>>>> Stashed changes

using namespace godot;

City::City() {
	
	income = 0;
	population = 50000;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
<<<<<<< Updated upstream
=======
	healthcare = 0;
    needs = 0;
	timer = 0;
	totalSatisfaction = 100;
	totalCo2Emissions = 100;
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
=======
	register_method((char*)"_on_MenuShop_pressed", &City::_on_MenuShop_pressed);
	register_method((char*)"_on_Validate_pressed", &City::_on_Validate_pressed);
	register_method((char*)"_on_Game_Speed_changed", &City::_on_Game_Speed_changed);
	register_method((char*)"_on_Satisfaction_value_changed", &City::_on_Satisfaction_value_changed);
	register_method((char*)"_on_CO2_value_changed", &City::_on_CO2_value_changed);

	register_property<City, float>("time_speed", &City::time_speed, 1.0);

>>>>>>> Stashed changes
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
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");

	if (RestaurantScene.is_valid() && ShopScene.is_valid())
	{
		for (int x = 0; x < 2; x++)
		{
			for (int z = 0; z < 3; z++)
			{
				int type = rand() % 2;
				Node* node;
				if (type == 0) { node = RestaurantScene->instance(); }
				else { node = ShopScene->instance(); }
				node->set("scale", Vector3(10, 10, 10));
				node->set("translation", Vector3(30 * x, 0, 30 * z));
				//int rot = rand() % 2;
				//node->set("rotation_degrees", Vector3(0, 180 * rot, 0));
				this->add_child(node);
			}
		}
	}
	if (BugattiScene.is_valid() && ChironScene.is_valid())
	{
		for (int z = 0; z < 2; z++)
		{
			int type = rand() % 2;
			Node* node;
			if (type == 0) { node = BugattiScene->instance(); }
			else { node = ChironScene->instance(); }
			node->set("scale", Vector3(10, 10, 10));
			node->set("translation", Vector3(-13, 0, -13 + 30 * z));
			this->add_child(node);
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

<<<<<<< Updated upstream
=======
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
>>>>>>> Stashed changes


double City::return_totalSatisfaction() {
    return totalSatisfaction;
}

double City::return_totalCo2Emissions() {
	return totalCo2Emissions;
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
<<<<<<< Updated upstream
=======

void City::_on_Satisfaction_value_changed(totalSatisfaction)
{
	Color green = Color( 0, 1, 0, 1 );
	Color orange =  Color( 1, 0.65, 0, 1 );
	Color red = Color( 1, 0, 0, 1 );
	this->get_parent()->get_child(1)->get_child(5)->get_child(0)->set_tint_progress(green);
	if (totalSatisfaction < 70){
			this->get_parent()->get_child(1)->get_child(5)->get_child(0)->set_tint_progress(orange);
	}
	if (totalSatisfaction < 33){
			this->get_parent()->get_child(1)->get_child(5)->get_child(0)->set_tint_progress(red);
	}
}

void City::_on_C02_value_changed(totalCo2Emissions)
{
	Color green = Color( 0, 1, 0, 1 );
	Color orange =  Color( 1, 0.65, 0, 1 );
	Color red = Color( 1, 0, 0, 1 );
	this->get_parent()->get_child(1)->get_child(5)->get_child(0)->set_tint_progress(green);
	if (totalCo2Emissions < 70){
			this->get_parent()->get_child(1)->get_child(5)->get_child(0)->set_tint_progress(orange);
	}
	if (totalCo2Emissions < 33){
			this->get_parent()->get_child(1)->get_child(5)->get_child(1)->set_tint_progress(red);
	}
}

>>>>>>> Stashed changes
