#include "City.h"
#include "Transport.h"
#include "Player.h"
#include "edit_text_files.h"

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

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif





using namespace godot;

int traffic_system[10][10][4][3] = { 1 };

City::City() {

	income = 0;
	population = 50000;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
	environmentalCost = 0;
	totalSatisfaction = 50;

	time_speed = 1;


	//timer = 0;
	day_tick = 0;
	days_since_last_simulation = 0;

	// in order to find date
	daycount=0;

	// in order to write stats to csv files
	stat = 0;

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
	register_method((char*)"add_shop", &City::add_shop);

	register_property<City, float>("time_speed", &City::time_speed, 1.0);
	register_property<City, int>("day_tick", &City::day_tick, 0);

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
	this->get_tree()->get_root()->get_node("Main/GUI/GUIComponents/TimeControls/Date")->set("text", return_word_date());
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
		this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
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
	std::cout << "DEBUG: generate city graphics started" << std::endl;
	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> MallScene = ResLo->load("res://Resources/Mall.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");
	Ref<PackedScene> WindmillScene = ResLo->load("res://Resources/Windmill.tscn", "PackedScene");
	Ref<PackedScene> LowHouseScene = ResLo->load("res://Resources/LowHouse.tscn", "PackedScene");
	Ref<PackedScene> HighHouseScene = ResLo->load("res://Resources/HighHouse.tscn", "PackedScene");
	Ref<PackedScene> ParkScene = ResLo->load("res://Resources/Park.tscn", "PackedScene");

	if (RestaurantScene.is_valid() && ShopScene.is_valid())
	{
		for (int x = 0; x < 8; x++)
		{
			for (int z = 0; z < 9; z++)
			{
				Vector3 pos = Vector3(30 * x, 0, 30 * z);
				std::cout << "DEBUG: About to create a random shop" << std::endl;
				int type = rand() % 8;
				if (type == 0) { add_shop(pos, RestaurantScene); }
				else if (type == 1) { add_shop(pos, ShopScene); }
				else if (type <= 5) { add_house(pos, LowHouseScene); }
				else { add_house(pos, HighHouseScene); }
				
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
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	// Repeat for all menus
	this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("position", Vector2(20, 20));
	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("visible", true);
}


void City::_ready()
{

	std::cout << "DEBUG: Ready started" << std::endl;
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
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);

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
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
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

void City::add_shop(Vector3 pos, Ref<PackedScene> scene) {

	std::cout << "DEBUG: add shop called" << std::endl;

	std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
	if (scene.is_valid()) {
		std::cout << "DEBUG: creating node" << std::endl;
		Node* node;
		std::cout << "DEBUG: instanciating" << std::endl;
		node = scene->instance();
		std::cout << "DEBUG: setting scale and translation" << std::endl;
		node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
		node->set("translation", pos);

		std::cout << "DEBUG: add child" << std::endl;
		this->add_child(node);
		std::cout << "DEBUG: add shop to vector" << std::endl;
		all_shops.push_back((Shop*)node);


		std::cout << "DEBUG: traffic stuff called" << std::endl;
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
		std::cout << "DEBUG: add shop done" << std::endl;
	}
}


void City::add_house(Vector3 pos, Ref<PackedScene> scene) {

	std::cout << "DEBUG: add house called" << std::endl;

	std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
	if (scene.is_valid()) {
		std::cout << "DEBUG: creating node" << std::endl;
		Node* node;
		std::cout << "DEBUG: instanciating" << std::endl;
		node = scene->instance();
		std::cout << "DEBUG: setting scale and translation" << std::endl;
		node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
		node->set("translation", pos);

		std::cout << "DEBUG: add child" << std::endl;
		this->add_child(node);
		std::cout << "DEBUG: add house to vector" << std::endl;
		all_houses.push_back((Housing*)node);



		std::cout << "DEBUG: traffic stuff called" << std::endl;
		//traffic stuff
		double x = ((Structure*)node)->get_position()[0] / 30; // needs to be double for identifying a 2 by 2 building 
		double y = ((Structure*)node)->get_position()[1] / 30; // can be int only for small building 
		
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
		std::cout << "DEBUG: add shop done" << std::endl;
	}
}

/*
int* City::building_coordinates_identification(int x, int y, int number) {
	//returns coordinates of a center for the upper left square of any buiding  
	if (number == 1) {
		int a[] = { x, y };
		return a; // LOCAL VARIABLE NEEDS FIXING - SEE WARNING MESSAGE
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
	return NULL;
}
*/
void City::update_traffic(int x, int y, bool newBuilding, int number) {
	if (positionOfBuildings[x][y] != 0) { // nothing happens if the building isn't there
		if (number == 1) {  // the case when it's a 1 by 1 buidling
			traffic[x][y][0][2] = 1;
			traffic[x][y][1][2] = 1;
			traffic[x][y][2][2] = 1;
			traffic[x][y][3][2] = 1;
			traffic_system[x][y][0][2] = 1;
			traffic_system[x][y][1][2] = 1;
			traffic_system[x][y][2][2] = 1;
			traffic_system[x][y][3][2] = 1;
			if (x + 1 < sizeOfCity && y + 1 < sizeOfCity && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 3)) {
				traffic[x][y][3][0] = 1;
				traffic_system[x][y][3][0] = 1;
			}
			if (y + 1 < sizeOfCity && (positionOfBuildings[x + 1][y + 1] == 1 || positionOfBuildings[x + 1][y + 1] == 2 || positionOfBuildings[x + 1][y] == 5)) {
				traffic[x][y][3][1] = 1;
				traffic_system[x][y][3][1] = 1;
			}
			if (x - 1 >= 0 && y + 1 > sizeOfCity && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 3 || positionOfBuildings[x - 1][y + 1] == 4)) {
				traffic[x][y][2][0] = 1;
				traffic_system[x][y][2][0] = 1;
			}
			if (x - 1 >= 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
				traffic[x][y][2][1] = 1;
				traffic_system[x][y][2][1] = 1;
			}
			if (x - 1 >= 0 && y - 1 > 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
				traffic[x][y][1][0] = 1;
				traffic_system[x][y][1][0] = 1;
			}
			if (y - 1 >= 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
				traffic[x][y][1][1] = 1;
				traffic_system[x][y][1][1] = 1;
			}
			if (x + 1 < sizeOfCity && y - 1 >= 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
				traffic[x][y][0][0] = 1;
				traffic_system[x][y][0][0] = 1;
			}
			if (x + 1 < sizeOfCity && (positionOfBuildings[x + 1][y] == 1 || positionOfBuildings[x + 1][y] == 2 || positionOfBuildings[x + 1][y] == 3)) {
				traffic[x][y][0][1] = 1;
				traffic_system[x][y][0][1] = 1;
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
			if (number == 3) {
				x =  x - 1;
			}
			if (number == 4) {
				x = x - 1;
				y = y - 1;
			}
			if (number == 5) {
				y = y - 1;
			}
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


	day_tick += this->time_speed;
	this->days_since_last_simulation = 0;
	Godot::print(return_word_date());
	this->get_tree()->get_root()->get_node("Main/GUI/GUIComponents/TimeControls/Date")->set("text", return_word_date());
	this->income = 0;
	this->population = 50000;
	this->carbonEmission = 0;
	this->numberOfEmployees = 0;
	this->energyDemand = 0;
	this->energySupply = 0;
	this->totalSatisfaction = 50;

	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
	{
		//Godot::print( "DEBUG: THIS OBJECT IS A ");
		//Godot::print((String)(*it)->get("object_type"));
		((Structure*)(*it))->set("updatable", true);
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		this->numberOfEmployees += (double)((*it)->get("employment"));
		this->income += (double)((*it)->get("employment"))*(double)((*it)->get("averageWage"));
		this->energyDemand += (double)((*it)->get("energyUse"));
		this->environmentalCost += (double)((*it)->get("environmentalCost"));
	}

	std::cout << "DEBUG: TOTAL CARBON EMISSION = " << this->carbonEmission << std::endl; 
	for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
	{
		(*it)->set("updatable", true);
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		this->totalSatisfaction += (double)((*it)->get("satisfaction")) * 10;
	}
	//totalSatisfaction /= all_houses.size();

	for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
	{
		(*it)->set("updatable", true);
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		this->totalSatisfaction += (double)((*it)->get("satisfaction")) * 10;
		this->energySupply += (double)((*it)->get("energyperDay"));
		this->numberOfEmployees += (double)((*it)->get("employment"));
		this->income += (double)((*it)->get("employment"))*(double)((*it)->get("averageWage"));
	}
	
	for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
	{
		(*it)->set("updatable", true);
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		this->totalSatisfaction += (double)((*it)->get("satisfaction")) * 10;
		this->energyDemand += (double)((*it)->get("energyUse"));
		this->environmentalCost += (double)((*it)->get("environmentalCost"));
		this->numberOfEmployees += (double)((*it)->get("employment"));
		this->income += (double)((*it)->get("employment"))*(double)((*it)->get("averageWage"));
	}

	/*
	for (std::vector<Transport*>::iterator it = all_transports.begin(); it != all_transports.end(); ++it)
	{
		    // count up all the vehicle stuff
	}
	*/
}



template<typename T> String to_godot_string(T s)
{
    std::string standardString = std::to_string(s);
    godot::String godotString = godot::String(standardString.c_str());
    return godotString;
}

int * return_date(int day_tick) {
    int static date[3];
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


String return_number_date(int day, int month, int year) {
    return to_godot_string(day) + String(", ") + to_godot_string(month) + String(", ") + to_godot_string(year);
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


String City::return_word_date() {

    int* datenumber = return_date(int(this->day_tick));
    int year=datenumber[2];
    String date = String("Year ");
    date += to_godot_string(datenumber[2]);
    date += String(", ");

    if (datenumber[1] == 1) {
        date += String("January ");
    }
    if (datenumber[1] == 2) {
        date += String("February ");
    }
    if (datenumber[1] == 3) {
        date += String("March ");
    }
    if (datenumber[1] == 4) {
        date += String("April ");
    }
    if (datenumber[1] == 5) {
        date += String("May ");
    }
    if (datenumber[1] == 6) {
        date += String("June ");
    }
    if (datenumber[1] == 7) {
        date += String("July ");
    }
    if (datenumber[1] == 8) {
        date += String("August ");
    }
    if (datenumber[1] == 9) {
        date += String("September ");
    }
    if (datenumber[1] == 10) {
        date += String("October ");
    }
    if (datenumber[1] == 11) {
        date += String("November ");
    }
    if (datenumber[1] == 12) {
        date += String("December ");
    }

    date += to_godot_string(datenumber[0]);
    return date;
}





/// edit text files found in /addons/file.samples

/*
string get_path(tring documentName) {
    return "../../addons/easy_charts/file.samples/" + documentName + ".csv";
}

// Function to add a line of the form "2015;76" to the csv file named documentName.
// To do so, call add_data("pollution", "2015", "76");
void add_data(string documentName, String year, String value) {
    std::fstream file;
    String path = get_path(documentName);
    file.open(path, std::ios::out | std::ios::app);
    file << year << ";" << value << '\n';
    file.close();
}

// Suppresses all data stored in the file documentName
void clear_completely(String documentName) {
    std::fstream file;
    String path = get_path(documentName);
    file.open(path, ios::out | ios::trunc);
    file.close();
}

// Suppresses all data stored in the file documentName except the first line.
void clear(String documentName) {
    std::fstream file;
    String path = get_path(documentName);

    file.open(path);
    String line;
    getline(file, line);
    file.close();

    file.open(path, std::ios::out | std::ios::trunc);
    file.close();

    file.open(path);
    file << line << '\n';
    file.close();
}

// Copies the data stored in documentNameFrom to an empty file documentNameTo.
void copy(String documentNameFrom, String documentNameTo) {
    std::fstream fileFrom;
    std::fstream fileTo;
    String path1 = get_path(documentNameFrom);
    String path2 = get_path(documentNameTo);
    fileFrom.open(path1);
    fileTo.open(path2);
    while (fileFrom.good()) {
        String line;
        getline(fileFrom, line, '\n');
        if (line.length() > 0) {
            fileTo << line << '\n';
        }
    }
    fileFrom.close();
    fileTo.close();
}

// Function to modify a line of the csv file named documentName.
// For example, if you want to change the line "2015;76" into "2015;01" of the pollution.csv file, call change_data("pollution", "2015", "01");
// By calling change_data("pollution", "2015", "01");, you will change all lines of the form "2015;**" into "2015;01".
// If no line in the file is of the form "2015;**", calling change_data("pollution", "2015", "01") won't do anything.
void change_data(String documentName, String dataToChange, String newValue) {
    std::fstream file;
    std::fstream temp;
    String path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        String line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            String sub = line.substr(0, pos);
            if (sub == dataToChange) {
                temp << sub << ";" << newValue << '\n';
            } else {
                temp << line << '\n';
            }
        }
    }

    file.close();
    temp.close();

    clear_completely(documentName);

    copy("datas_on_rows", documentName);
    clear_completely("datas_on_rows");
}

// Function to delete a line of the csv file named documentName.
// By calling delete_line("pollution", "2015");, you will delete all lines of the form "2015;**".
// If no line in the file is of the form "2015;**", calling delete_line("pollution", "2015") won't do anything.
void delete_line(String documentName, String dataToDelete) {
    std::fstream file;
    std::fstream temp;
    String path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        String line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            String sub = line.substr(0, pos);
            if (sub != dataToDelete) {
                temp << line << '\n';
            }
        }
    }

    file.close();
    temp.close();

    clear_completely(documentName);

    copy("datas_on_rows", documentName);
    clear_completely("datas_on_rows");
}

*/

void City::write_stat_history_to_file() {

	/*
    int *date; 
    date = return_date(day_tick);
    int day = *date;
    int month = *(date+1);
    int year = *(date+2);

    
    if (day==1 && month==1 && year!=1 && year!=2) {
        daycount=0;
	    int leap = (year-1)%4;
        edit_text_files::add_data(String("alltimestats"), to_godot_string(year-1), to_godot_string(find_avg(stats,leap)));
        double stats[366];
        remove(get_path(String("statsyear") + to_godot_string(year-2)).c_str());
    }
        

    stats[daycount]=stat;
    daycount+=1;
    edit_text_files::add_data(String("statsyear") + to_godot_string(year), return_number_date(day,month,year), to_godot_string(stat));

        
    int *daysbef;
    daysbef = return_date(day_tick-300);
    int daydaysbef=*daysbef;
    int monthdaysbef=*(daysbef+1);
    int yeardaysbef=*(daysbef+2);
    edit_text_files::delete_line(String("statsyear") + to_godot_string(year), return_number_date(daydaysbef,monthdaysbef,yeardaysbef));
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



//in order to check for errors on mac
int main() {
	City c=City();
	c.simulation();
	std::cout << "DEBUG: TOTAL CARBON EMISSION = " << c.return_carbonEmission() << std::endl; 
	return 0;
}
