#include "City.h"
#include "Transport.h"
#include "Player.h"

#include <Resource.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include <PackedScene.hpp>
#include <Node.hpp>
#include <ctime>
#include <Input.hpp>
#include <Button.hpp>
#include <Viewport.hpp>
#include <HSlider.hpp>
#include <TextureProgress.hpp>

#include <PoolArrays.hpp>

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


using namespace godot;

City::City() {

	income = 0;
	population = 50000;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
	healthcare = 0;
	totalSatisfaction = 100;

	time_speed = 1;


	//timer = 0;
	day_tick = 0;
	days_since_last_simulation = 0;

	srand((int)time(0));
}

City::~City()
{

};

void City::_register_methods()
{
	register_method((char*)"_physics_process", &City::_physics_process);
	register_method((char*)"_input", &City::_input);
	register_method((char*)"_ready", &City::_ready);
	register_method((char*)"_on_MenuShop_pressed", &City::_on_MenuShop_pressed);
	register_method((char*)"_on_Validate_pressed", &City::_on_Validate_pressed);
	register_method((char*)"_on_Game_Speed_changed", &City::_on_Game_Speed_changed);

	register_property<City, float>("time_speed", &City::time_speed, 1.0);

};

void City::_init()
{

};

void City::_process(float)
{

};


/*
This function calls simulation() every second
`day_tick` contains the integer part of `delta_counter`
everytime the integer part of `delta_counter` changes
we update `day_tick` and execute simulation()
*/
void City::_physics_process(float delta) {

	if (bool(time_speed)) 
	{ 
		simulation_counter += (double)delta; 
		date_counter += double(delta) * time_speed;
	} 
	 
	if (simulation_counter > 1)
	{
		simulation();
		simulation_counter -= 1;
	}

	if (date_counter > 1)
	{
		(this->days_since_last_simulation)++;
		this->update_date();
		date_counter -= 1;
	}

	if (this->notification_active) 
	{
		(this->notification_counter)++;
		if (this->notification_counter > this->notification_timeout) 
		{
			this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
			this->notification_active = false;
			this->notification_counter = 0;
		}
	}
}

void City::update_date() {
	this->day_tick += days_since_last_simulation;
	this->get_tree()->get_root()->get_node("Main/GUI/GUIComponents/TimeControls/Date")->set("text", return_game_date());
	this->day_tick -= days_since_last_simulation;
}

void City::_input(InputEvent*)
{

	Input* i = Input::get_singleton();


	if (i->is_action_pressed("ui_test"))
	{
		add_car();
	}

	if (i->is_action_pressed("ui_turn"))
	{
		this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
		this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
		if (!(this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->get("visible"))) {
			((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/Player")))->set("movable", true);
		}
		this->_on_Game_Speed_changed();

	}

	if (i->is_action_pressed("ui_accept") && this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->get("visible"))
	{
		_on_Validate_pressed();
	}
};


void City::generate_initial_city_graphics()
{
	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> MallScene = ResLo->load("res://Resources/Mall.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");
	Ref<PackedScene> WindmillScene = ResLo->load("res://Resources/Windmill.tscn", "PackedScene");

	if (RestaurantScene.is_valid() && ShopScene.is_valid())
	{
		for (int x = -3; x < 4; x++)
		{
			for (int z = -3; z < 4; z++)
			{
				
				int type = rand() % 2;
				if (type == 0) { add_shop(Vector3(30 * x, 0, 30 * z), RestaurantScene); }
				else { add_shop(Vector3(30 * x, 0, 30 * z), ShopScene); }
				
				/*{ node = RestaurantScene->instance(); }
				else { node = ShopScene->instance(); }
				node->set("scale", Vector3(10, 10, 10));
				node->set("translation", Vector3(30 * x, 0, 30 * z));
				this->add_child(node);
				all_shops.push_back((Shop*)node);
				*/
			}
		}
	}
	if (MallScene.is_valid())
	{
		Node* node = MallScene->instance();
		node->set("translation", Vector3(225, 0, 105));
		this->add_child(node);
		all_shops.push_back((Shop*)node);
	}
	if (WindmillScene.is_valid())
	{
		Node* node = WindmillScene->instance();
		node->set("translation", Vector3(120, 0, -150));
		this->add_child(node);
		all_shops.push_back((Shop*)node);
	}
}

void City::set_initial_visible_components()
{
	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	// Repeat for all menus
	this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("position", Vector2(20, 20));
	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("visible", true);
}


void City::_ready()
{
	this->generate_initial_city_graphics();
	this->set_initial_visible_components();
	
}

void godot::City::_on_MenuShop_pressed(String name)
{
	
	active_button = name;

	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->set("text", String(""));

	String ButtonInfo = this->get_button_info_text();
	this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("text", ButtonInfo);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", true);

	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", true);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);

}

String City::get_button_info_text() {

	// This method returns text giving information about the various policies 
	// The placeholder text for the input box changed to correpsond to the old value of the "policy"

	// I DON'T KNOW HOW TO DO THE SECOND PART YET 
	// I GUESS THESE VALUES SHOULD BE STORED IN CITY AND NOT FETCHED FROM RANDOM OBJECTS

	if (this->active_button == String("ChangePanelProbabilityForAllShops")) 
	{
		//this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->set("placeholder_text", String(""));
		return String("Please input a value between 0 and 1. This value will be the new probability that solar panels are installed in a year for all shops. Ultimately, this policy will be implemented as a subsidy, and thus the input will refer to a certain amount that the city will be willing to contribute to the installation of solar panels on shops. Hence, this amount will be subtracted from the budget as soon as solar panels are installed on a shop.");
	} 
	else if (this->active_button == String("ChangePanelProbabilityForRestaurants")) 
	{ 
		return String("Please input a value between 0 and 1. This value will be the new probability that solar panels are installed in a year for restaurants in the city.");
	}
	else {
		return String("No information has been specified for this policy.");
	}
}

void godot::City::_on_Validate_pressed()
{

	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
	String mytext = this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->get("text");
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);

	((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/Player")))->set("movable", true);
	this->_on_Game_Speed_changed();

	if (mytext.is_valid_float()) {
		this->implement_shop_policies((double)mytext.to_float());
	}
	else { 
		this->trigger_notification(String("You did not enter an appropriate value for the policy."));
	}
}

void City::trigger_notification(String text = String("Seems like there was a mistake. Please try again.")) 
{
	std::cout << "INVALID INPUT: EXPECTED FLOAT IN SPECIFIED RANGE" << std::endl;
	this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("text", text);
	this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", true);
	this->notification_active = true;
}

void City::implement_shop_policies(double value) {

	Godot::print(this->active_button);

	if (this->active_button == String("ChangePanelProbabilityForAllShops")) {
		if (value >= 0 && value < 1) {
			Godot::print("PANEL PROBABILITY WILL BE CHANGED FOR ALL SHOPS");
			for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
			{
				(*it)->set("panel_probability", value);
			}
		}
		else { 
			this->trigger_notification(String("The value you provided was not in the specified range."));
		}
	}
	else if (this->active_button == String("ChangePanelProbabilityForRestaurants")) {
		if (value >= 0 && value < 1) {
			Godot::print("PANEL PROBABILITY WILL BE CHANGED ONLY FOR RESTAURANTS");
			for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
			{
				if ((String)(*it)->get("object_type") == (String)("Restaurant")) {
					(*it)->set("panel_probability", value);
				}
			}
		}
		else { 
			this->trigger_notification();
		}
	}
}

void godot::City::_on_Game_Speed_changed()
{
	time_speed = round(pow(2, (int)(this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->get_child(0)->get("value")) - 1) - 0.1);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", false);
	((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/Player")))->set("movable", true);
}


void City::add_car() {


	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");

	if (BugattiScene.is_valid() && ChironScene.is_valid())
	{

		// randomly choose between bugatti and chiron

		int type = rand() % 2;
		Transport* node;
		if (type == 0) { node = (Transport*)BugattiScene->instance(); }
		else { node = (Transport*)ChironScene->instance(); }
		node->set("scale", Vector3(10, 10, 10));
		node->set("translation", Vector3(-13, 0, -13 + 30 * (0 + 1)));
		this->add_child((Node*)node);


		//income -= node->cost;

		//all_transports.push_back((Transport*)node);
	}
}


void City::add_shop( Vector3 pos, Ref<PackedScene> scene) {
	if (scene.is_valid()) {
		Node* node;
		node = scene->instance();
		node->set("scale", Vector3(10, 9 + ((double(rand()) / RAND_MAX) * 2), 10));
		node->set("translation", pos);
		this->add_child(node);
		all_shops.push_back((Shop*)node);


		double x = ((Structure*)node)->get_position()[0] / 30; // needs to be double for identifying a 2 by 2 building 
		double y = ((Structure*)node)->get_position()[1] / 30; // can be int only for small building 



		//traffic stuff
		if (x < sizeOfCity && y < sizeOfCity) {
			if (x > int(x) - 0.1 && x < int(x) + 0.1) { // check that it's a small building
				positionOfBuildings[int(x)][int(y)] = 1;
			}
			else {
				positionOfBuildings[int(x)][int(y)] = 2; // assign numbers to the four squares of the 2 by 2 buidling to know it's position by knowing just the coordinates and the number of one square
				positionOfBuildings[int(x) + 1][int(y)] = 3;
				positionOfBuildings[int(x) + 1][int(y) + 1] = 4;
				positionOfBuildings[int(x)][int(y) + 1] = 5;
			}
			update_traffic(int(x), int(y), true, positionOfBuildings[int(x)][int(y)]);
		}
	}
}

int* City::building_coordinates_identification(int x, int y, int number) {
	//returns coordinates of a center for the upper left square of any buiding  
	if (number == 1) {
		int a[] = { x, y };
		return a;
	}
	if (number == 2) {
		int a[] = { x , y };
		return a;
	}
	if (number == 3) {
		int a[] = { x - 1, y };
		return a;
	}
	if (number == 4) {
		int a[] = { x - 1, y - 1 };
		return a;
	}
	if (number == 5) {
		int a[] = { x , y - 1 };
		return a;
	}
}

void City::update_traffic(int x, int y, bool newBuilding, int number) {
	if (positionOfBuildings[x][y] != 0) { // nothing happens if the building isn't there
		if (number == 1) {  // the case when it's a 1 by 1 buidling
			traffic[x][y][0][2] = 1;
			traffic[x][y][1][2] = 1;
			traffic[x][y][2][2] = 1;
			traffic[x][y][3][2] = 1;
			if (x + 1 < sizeOfCity && y + 1 < sizeOfCity && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 3)) {
				traffic[x][y][3][0] = 1;
			}
			if (y + 1 < sizeOfCity && (positionOfBuildings[x + 1][y + 1] == 1 || positionOfBuildings[x + 1][y + 1] == 2 || positionOfBuildings[x + 1][y] == 5)) {
				traffic[x][y][3][1] = 1;
			}
			if (x - 1 >= 0 && y + 1 > sizeOfCity && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 3 || positionOfBuildings[x - 1][y + 1] == 4)) {
				traffic[x][y][2][0] = 1;
			}
			if (x - 1 >= 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
				traffic[x][y][2][1] = 1;
			}
			if (x - 1 >= 0 && y - 1 > 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
				traffic[x][y][1][0] = 1;
			}
			if (y - 1 >= 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
				traffic[x][y][1][1] = 1;
			}
			if (x + 1 < sizeOfCity && y - 1 >= 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
				traffic[x][y][0][0] = 1;
			}
			if (x + 1 < sizeOfCity && (positionOfBuildings[x + 1][y] == 1 || positionOfBuildings[x + 1][y] == 2 || positionOfBuildings[x + 1][y] == 3)) {
				traffic[x][y][0][1] = 1;
			}

			if (newBuilding == true) {  // update all the possible buildings around 
				// no need to check if they exist since if they don't the function checks that with the first line 
				if (x - 1 >= 0) {
					if (y - 1 >= 0) {
						update_traffic(x - 1, y - 1, false, positionOfBuildings[x - 1][y - 1]);
					}
					update_traffic(x - 1, y, false, positionOfBuildings[x - 1][y]);
					if (y + 1 < sizeOfCity) {
						update_traffic(x - 1, y + 1, false, positionOfBuildings[x - 1][y + 1]);
					}
				}
				if (y - 1 >= 0) {
					update_traffic(x, y - 1, false, positionOfBuildings[x][y - 1]);
				}
				if (y + 1 < sizeOfCity) {
					update_traffic(x, y + 1, false, positionOfBuildings[x][y + 1]);
				}
				if (x + 1 < sizeOfCity) {
					if (y - 1 >= 0) {
						update_traffic(x + 1, y - 1, false, positionOfBuildings[x + 1][y - 1]);
					}
					update_traffic(x + 1, y, false, positionOfBuildings[x + 1][y]);
					if (y + 1 < sizeOfCity) {
						update_traffic(x + 1, y + 1, false, positionOfBuildings[x + 1][y + 1]);
					}
				}
			}
		}
		else { // the case when it's a 2 by 2 building 
			int* a = building_coordinates_identification(x, y, number);
			x = a[0];
			y = a[1];

			traffic[x][y][0][1] = 1;
			if (y - 1 >= 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
				traffic[x][y][0][0] = 1;
			}

			traffic[x + 1][y][0][2] = 1;
			if (x + 2 < sizeOfCity && y - 1 >= 0 && (positionOfBuildings[x + 2][y - 1] == 1 || positionOfBuildings[x + 2][y - 1] == 2 || positionOfBuildings[x + 2][y - 1] == 5)) {
				traffic[x + 1][y][0][0] = 1;
			}
			if (x + 2 < sizeOfCity && (positionOfBuildings[x + 2][y] == 1 || positionOfBuildings[x + 2][y] == 2 || positionOfBuildings[x + 2][y] == 3)) {
				traffic[x + 1][y][0][1] = 1;
			}

			traffic[x + 1][y][3][1] = 1;
			if (x + 2 < sizeOfCity && (positionOfBuildings[x + 2][y + 1] == 1 || positionOfBuildings[x + 2][y + 1] == 2 || positionOfBuildings[x + 2][y + 1] == 3)) {
				traffic[x + 1][y][3][0] = 1;
			}

			traffic[x + 1][y + 1][3][2] = 1;
			if (x + 2 < sizeOfCity && y + 2 < sizeOfCity && (positionOfBuildings[x + 2][y + 2] == 1 || positionOfBuildings[x + 2][y + 2] == 2 || positionOfBuildings[x + 2][y + 2] == 3)) {
				traffic[x + 1][y + 1][3][0] = 1;
			}
			if (y + 2 < sizeOfCity && (positionOfBuildings[x + 2][y] == 1 || positionOfBuildings[x + 2][y] == 3 || positionOfBuildings[x + 2][y] == 4)) {
				traffic[x + 1][y + 1][3][1] = 1;
			}

			traffic[x + 1][y][2][1] = 1;
			if (y + 2 < sizeOfCity && (positionOfBuildings[x][y + 2] == 1 || positionOfBuildings[x][y + 2] == 3 || positionOfBuildings[x][y + 2] == 4)) {
				traffic[x + 1][y][2][0] = 1;
			}

			traffic[x][y + 1][2][2] = 1;
			if (x - 1 >= 0 && y + 2 < sizeOfCity && (positionOfBuildings[x - 1][y + 2] == 1 || positionOfBuildings[x - 1][y + 2] == 3 || positionOfBuildings[x - 1][y + 2] == 4)) {
				traffic[x][y + 1][2][0] = 1;
			}
			if (x - 1 >= 0 && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 4 || positionOfBuildings[x - 1][y + 1] == 5)) {
				traffic[x][y + 1][2][1] = 1;
			}

			traffic[x + 1][y][1][1] = 1;
			if (x - 1 >= 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
				traffic[x + 1][y][1][0] = 1;
			}

			traffic[x][y][1][2] = 1;
			if (x - 1 >= 0 && y - 1 >= 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
				traffic[x][y][1][0] = 1;
			}
			if (y - 1 >= 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
				traffic[x][y][1][1] = 1;
			}

			if (newBuilding == true) {  // update all the possible buildings around 
				// no need to check if they exist since if they don't the function checks that with the first line 
				if (x - 1 >= 0) {
					if (y - 1 >= 0) {
						update_traffic(x - 1, y - 1, false, positionOfBuildings[x - 1][y - 1]);
					}
					update_traffic(x - 1, y, false, positionOfBuildings[x - 1][y]);
					update_traffic(x - 1, y + 1, false, positionOfBuildings[x - 1][y + 1]);
					if (y + 2 < sizeOfCity) {
						update_traffic(x - 1, y + 2, false, positionOfBuildings[x - 1][y + 2]);
					}
				}
				if (y - 1 >= 0) {
					update_traffic(x, y - 1, false, positionOfBuildings[x][y - 1]);
					update_traffic(x + 1, y - 1, false, positionOfBuildings[x + 1][y - 1]);
				}
				if (y + 2 < sizeOfCity) {
					update_traffic(x, y + 2, false, positionOfBuildings[x][y + 2]);
					update_traffic(x + 1, y + 2, false, positionOfBuildings[x + 1][y + 2]);
				}
				if (x + 2 < sizeOfCity) {
					if (y - 1 >= 0) {
						update_traffic(x + 2, y - 1, false, positionOfBuildings[x + 2][y - 1]);
					}
					update_traffic(x + 2, y, false, positionOfBuildings[x + 2][y]);
					update_traffic(x + 2, y + 1, false, positionOfBuildings[x + 2][y + 1]);
					if (y + 2 < sizeOfCity) {
						update_traffic(x + 2, y + 2, false, positionOfBuildings[x + 2][y + 2]);
					}
				}
			}
		}
	}
}



void City::simulation()
{
	// std::cout << "Simulation" << std::endl;
	/*
	
	//write the old values in a file 
	income = 0;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
    healthcare = 0;
	*/

	day_tick += this->time_speed;
	this->days_since_last_simulation = 0;
	Godot::print(return_game_date());
	this->get_tree()->get_root()->get_node("Main/GUI/GUIComponents/TimeControls/Date")->set("text", return_game_date());
	this->carbonEmission = 0;

	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
	{
		//Godot::print( "DEBUG: THIS OBJECT IS A ");
		//Godot::print((String)(*it)->get("object_type"));
		(*it)->set("updatable", true);
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
	}
	
	std::cout << "DEBUG: TOTAL CARBON EMISSION = " << this->carbonEmission << std::endl;
	

	//for (std::vector<Structure*>::iterator it = buildings.begin(); it != buildings.end(); ++it)
	//{
		/*
		commented out until we know what variables to call in every structure

		income += (*it)->income;
		std::cout << "in LOOP income " << (*it)->income << std::endl;
		numberOfEmployees += (*it)->numberOfEmployees;
		carbonEmission += (*it)->carbonEmission;
		energyDemand += (*it)->energyDemand;
		energySupply += (*it)->energySupply;
        healthcare += (*it)->healthcare;
		(*it)->simulate_step(); //function that updates the building
		
		*/

	//}
	/*
	for (std::vector<Transport*>::iterator it = all_transports.begin(); it != all_transports.end(); ++it)
	{
	
		    // count up all the vehicle stuff
		

	}
	*/
}

/*
int * return_date(int day_tick) {
	int date[3];
	int Y=1,M=1,D=1;
	int julian = (1461 * (Y + 4800 + (M - 14)/12))/4 +(367 * (M - 2 - 12 / ((M - 14)/12)))/12 - (3 * ((Y + 4900 + (M - 14)/12)/100))/4 + D - 32075+day_tick+35;
	int gregorian = julian + 1401 + (int)((((int)(4*day_tick+274277) / 146097)*3) / 4) -38;
	int e = 4*gregorian+3;
	int h = 5*((int)(e%1461)/4)+2;
	int day = ((int)(h%153)/5)+1;
	int month = (((int)h/153)+2)%12+1;
	int year = (int)(e/1461) - 4716 + (int)((14-month)/12);
	date[0]= day;
	date[1] = month;
	date[2] = year;
	return date;
}
string return_string_date(int day, int month, int year) {
	return to_string(day) + ", " + to_string(month) + ", " + to_string(year);
}
double find_avg(double array[],int leap) {
	int size;
	double sum=0;
	if (leap==0) {
		size=366;
	}
	else {
		size=365;
	}
	for (int i=0; i<size; i++) {
		sum+=array[i];
	}
	return sum/size;
}
double stat = 0;
int day_tick = 0;
double stats[366];
int daycount=0;
/*/

void City::write_stat_history_to_file() {
	/*
		int *date;
		date = return_date(day_tick);
		int day = *date;
		int month = *(date+1);
		int year = *(date+2);

		if (day==1 && month==1 && year!=1) {
			daycount=0;
			int leap = (year-1)%4;
			add_data("alltimestats", to_string(year-1), to_string(find_avg(stats,leap)));
			double stats[366];
			remove(get_path("statsyear" + to_string(year-1)).c_str());
		}

		stats[daycount]=stat;
		daycount+=1;
		add_data("statsyear" + to_string(year), return_string_date(day,month,year), to_string(stat));

		int *daysbef;
		daysbef = return_date(day_tick-30);
		int daydaysbef=*daysbef;
		int monthdaysbef=*(daysbef+1);
		int yeardaysbef=*(daysbef+2);
		delete_line("statsyear" + to_string(year), return_string_date(daydaysbef,monthdaysbef,yeardaysbef));
	*/
}


double City::return_income() {
	return income;
}

double City::return_totalSatisfaction() {
	return totalSatisfaction;
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


template<typename T> String to_godot_string(T s)
{
	std::string standardString = std::to_string(s);
	godot::String godotString = godot::String(standardString.c_str());
	return godotString;
}

String City::return_game_date() {
	String date = String("Year ");
	date += to_godot_string(int(int(this->day_tick / 365) + 1));
	date += String(", ");
	int temp = int(this->day_tick) % 365;
	if (this->day_tick % 365 == 0) { temp = 365; }
	if (temp <= 31) {
		date += String("January ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 31;
	if (temp <= 28) {
		date += String("February ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 28;
	if (temp <= 31) {
		date += String("March ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 31;
	if (temp <= 30) {
		date += String("April ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 30;
	if (temp <= 31) {
		date += String("May ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 31;
	if (temp <= 30) {
		date += String("June ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 30;
	if (temp <= 31) {
		date += String("July ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 31;
	if (temp <= 31) {
		date += String("August ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 31;
	if (temp <= 30) {
		date += String("September ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 30;
	if (temp <= 31) {
		date += String("October ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 31;

	if (temp <= 30) {
		date += String("November ");
		date += to_godot_string(int(temp));
		return date;
	}
	temp -= 30;

	if (temp <= 31) {
		date += String("December ");
		date += to_godot_string(int(temp));
		return date;
	}
	return "Time Representation Error";

}



