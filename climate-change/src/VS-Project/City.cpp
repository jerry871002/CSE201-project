#include "City.h"
#include "Transport.h"
#include "Player.h"

#include <fstream>
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
	timer = 0;

	sizeOfCity = 10;
	//positionOfBuildings[sizeOfCity][sizeOfCity] = { 0 };
	//traffic[sizeOfCity][sizeOfCity][4][3] = { 0 };

	time_speed = 4;
	delta_counter = 0.0;


	//timer = 0;
	day_tick = 0;
	srand((int)time(0));
}

City::~City()
{

};

void City::_register_methods()
{
	register_method((char*)"_process", &City::_process);
	register_method((char*)"_input", &City::_input);
	register_method((char*)"_ready", &City::_ready);
	register_method((char*)"_on_MenuShop_pressed", &City::_on_MenuShop_pressed);
	register_method((char*)"_on_Validate_pressed", &City::_on_Validate_pressed);
	register_method((char*)"_on_Game_Speed_changed", &City::_on_Game_Speed_changed);
	
};

void City::_init()
{

};

void City::_process(float delta)
{

	//std::cout << "DEBUG: PROCESS CALLED" << std::endl;
	counter += delta*time_speed;
	if (counter > 1) 
	{
		simulation();
		counter -= 1;


	}
	//delta_counter += (delta * time_speed);
	//simulation();
	//if (day_tick != (int)delta_counter) {
	//	day_tick = (int)delta_counter;
	//	simulation();
	//	std::cout << "DEBUG: yet another lonely second in _process" << std::endl;
	//}
};



/*
This function calls simulation() every second

`day_tick` contains the integer part of `delta_counter`
everytime the integer part of `delta_counter` changes
we update `day_tick` and execute simulation()
*/
void City::_physics_process(float delta) {
	
}


void City::_input(InputEvent*)
{

	Input* i = Input::get_singleton();


	// VERTICAL MOTION

	if (i->is_action_pressed("ui_test")) 
	{
		add_car();
	}

	if (i->is_action_pressed("ui_turn")) 
	{
		//this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	}

	if (i->is_action_pressed("ui_accept") && this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->get("visible")) 
	{
		_on_Validate_pressed();
	}
};



void City::_ready()
{

	std::cout << "DEBUG: started ready " << std::endl;
	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");

	if (RestaurantScene.is_valid() && ShopScene.is_valid())
	{
		
				// randomly choose between restaurant and shop

				std::cout << "DEBUG: restaurant before instanciating " << std::endl;
				Node* node = RestaurantScene->instance();
				

				// REMOVE COMMENT ONCE INITIALIZE COMMAND IS CREATED
				// ((Restaurant*)node)->Restaurant::initialize();

				//Node* node = RestaurantScene->instance();
				std::cout << "DEBUG: restaurant instanciating " << std::endl;
				node->set("scale", Vector3(10, 10, 10));
				node->set("translation", Vector3(0, 0, 0));
				//int rot = rand() % 2;
				//node->set("rotation_degrees", Vector3(0, 180 * rot, 0));
				this->add_child(node);

				// REMOVE COMMENT ONCE INHERITANCE IS FIXED
				this->add_shop((Shop*)node);
				std::cout << "DEBUG: restaurant added to city your mom fat " << std::endl;

	}
	

	if (BugattiScene.is_valid() && ChironScene.is_valid())
	{
		// TODO: This loop is only going to run once, maybe remove the loop?
		for (int z = 0; z < 1; z++) // Car removed to test
		{
			// randomly choose between bugatti and chiron

			int type = rand() % 2;
			Transport* node;
			if (type == 0) { node = (Transport*)BugattiScene->instance(); }
			else { node = (Transport*)ChironScene->instance(); }
			node->set("scale", Vector3(10, 10, 10));
			node->set("translation", Vector3(-13, 0, -13));
			this->add_child((Node*)node);
			std::cout << node->kmPerDay;
		}
	}
	
	std::cout << "DEBUG: finished ready " << std::endl;
	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("position", Vector2(20, 20));
	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("visible", true);
};



void godot::City::_on_MenuShop_pressed(String name)
{
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", true);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);

	active_button = name;

}

void godot::City::_on_Validate_pressed()
{
	
	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
	String mytext = this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->get("text");
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);

	//((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/KinematicBody")))->set_movable(true);

}
void godot::City::_on_Game_Speed_changed()
{
	time_speed = this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->get_child(0)->get("value");
	std::cout << time_speed;
}
;


void City::add_shop(Shop* shop) {
	all_shops.push_back(shop);
	int x = shop->get_position()[0]/30;
	int y = shop->get_position()[1]/30;
	if (x < sizeOfCity && y < sizeOfCity) {
		positionOfBuildings[x][y] = 1;
		update_traffic(x, y, true);
	}
}

void City::update_traffic(int x, int y, bool newBuilding) {
	traffic[x][y][0][3] = 1;
	traffic[x][y][1][3] = 1;
	traffic[x][y][2][3] = 1;
	traffic[x][y][3][3] = 1;
	if (x + 1 < sizeOfCity && y + 1 > sizeOfCity && positionOfBuildings[x + 1][y + 1] == 1) {
		traffic[x][y][3][0] = 1;
	}
	if (y + 1 < sizeOfCity && positionOfBuildings[x][y + 1] == 1) {
		traffic[x][y][3][1] = 1;
	}
	if (x - 1 > 0 && y + 1 > sizeOfCity && positionOfBuildings[x - 1][y + 1] == 1) {
		traffic[x][y][2][0] = 1;
	}
	if (x - 1 > 0 && positionOfBuildings[x - 1][y] == 1) {
		traffic[x][y][2][1] = 1;
	}
	if (x - 1 > 0 && y - 1 > 0 && positionOfBuildings[x - 1][y - 1] == 1) {
		traffic[x][y][1][0] = 1;
	}
	if (y - 1 > 0 && positionOfBuildings[x][y - 1] == 1) {
		traffic[x][y][1][1] = 1;
	}
	if (x + 1 < sizeOfCity && y - 1 > 0 && positionOfBuildings[x + 1][y - 1] == 1) {
		traffic[x][y][0][0] = 1;
	}
	if (x + 1 < sizeOfCity && positionOfBuildings[x + 1][y] == 1) {
		traffic[x][y][0][1] = 1;
	}
	if (newBuilding == true) {
		if (x - 1 > 0) {
			if (y - 1 > 0) {
				update_traffic(x - 1, y - 1, false);
			}
			update_traffic(x - 1, y, false);
			if (y + 1 < sizeOfCity) {
				update_traffic(x - 1, y - 1, false);
			}
		}
		if (y - 1 > 0) {
			update_traffic(x, y - 1, false);
		}
		if (y + 1 < sizeOfCity) {
			update_traffic(x, y + 1, false);
		}
		if (x + 1 < sizeOfCity) {
			if (y - 1 > 0) {
				update_traffic(x + 1, y - 1, false);
			}
			update_traffic(x + 1, y, false);
			if (y + 1 < sizeOfCity) {
				update_traffic(x + 1, y - 1, false);
			}
		}
	}
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


void City::simulation() {
	day_tick++;
	/*

	//write the old values in a file
	income = 0;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
	healthcare = 0;
	needs = 0;
	*/


	std::cout << return_game_date() << std::endl;


	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
	{
		/*
		commented out until we know what variables to call in every structure

		income += (*it)->income;
		std::cout << "in LOOP income " << (*it)->income << std::endl;
		numberOfEmployees += (*it)->numberOfEmployees;
		carbonEmission += (*it)->carbonEmission;
		energyDemand += (*it)->energyDemand;
		energySupply += (*it)->energySupply;
		healthcare += (*it)->healthcare;
		needs += (*it)->needs;
		(*it)->simulate_step(); //function that updates the building

		*/
		if (day_tick % 15 == 0) {
			((Restaurant*)*it)->Restaurant::simulate_step(15.0);
		}
	}
	/*
	for (std::vector<Transport*>::iterator it = all_transports.begin(); it != all_transports.end(); ++it)
	{

		
			 count up all the vehicle stuff
		
	}
	*/


	if (day_tick % 140 == 0) {
		// randomly finds an open spot and splats down a restaurant


		std::cout << "DEBUG: daytick mod 140 entered in simulation " << std::endl;
		Vector3 temp = Vector3(0, 0, 0);
		bool location_covered = true;
		while (location_covered) {
			location_covered = false;
			int r = rand() % 2;
			int s = pow(-1,rand() % 2);
			std::cout << "DEBUG: about to add to temp " << std::endl;
			switch (r) {
			case 0: temp += Vector3(s * 30, 0, 0);
				break;
			case 1: temp += Vector3(0, 0, s * 30);
				break;
			}

			for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
			{
				// std::cout << "DEBUG: looking at a restaurant's position " << std::endl;
				if (temp == ((Structure*)*it)->Structure::get_position()) {
					location_covered = true;
					break;
				}

			}
			
		}
		add_restaurant_to_city(temp);
	}
	
}

void City::write_stat_history_to_file() {
	/*
	std::ofstream file;
 	file.open("file.txt", std::ofstream::out | std::ofstream::app);
 	if(!file) { 
        std::cout<<"Error in creating file!!!"<< std::endl;
    	}
	std::cout<<"File created successfully."<<std::endl; 
 	file.close();
	 */

 	//incomefile << timer << " " << income << " " << population << " " << numberOfEmployees << " ";
 	//incomefile << carbonEmission << " " << energyDemand << " " << energySupply << std::endl;
 	//add_data("incomefile",std::to_string((day_tick / 365) + 1),std::to_string(income));



 	/* old version to be deleted?
 	std::ofstream out_file;
 	out_file.open("stat_history.txt", std::ofstream::out | std::ofstream::app);

 	//out_file << timer << " " << income << " " << population << " " << numberOfEmployees << " ";
 	out_file << carbonEmission << " " << energyDemand << " " << energySupply << std::endl;
 	out_file.close();
 	*/
}


void City::add_restaurant_to_city(Vector3 position) {

	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");

	if (RestaurantScene.is_valid())
	{
		std::cout << "DEBUG: add_restaurant_to_city called " << std::endl;
		Node* node = RestaurantScene->instance();

		// REMOVE COMMENT ONCE INITIALIZE COMMAND IS CREATED
		((Restaurant*)node)->Restaurant::initialize();

		//Node* node = RestaurantScene->instance();
		//std::cout << "DEBUG: restaurant instanciating " << std::endl;
		node->set("scale", Vector3(10, 10, 10));
		node->set("translation", position);
		//int rot = rand() % 2;
		//node->set("rotation_degrees", Vector3(0, 180 * rot, 0));
		this->add_child(node);
		
		// REMOVE COMMENT ONCE INHERITANCE IS FIXED
		this->add_shop((Shop*)node);
		//std::cout << "DEBUG: restaurant added to city your mom fat " << std::endl;
	}
	else { std::cout << "DEBUG: Major error, restaurant scene not valid " << std::endl; }
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
	date += std::to_string((day_tick / 365) + 1) + ", ";
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
