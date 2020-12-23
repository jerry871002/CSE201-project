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
    needs = 0;
	timer = 0;
	totalSatisfaction = 100;

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
	register_method((char*)"add_shop", &City::add_shop);
	
	register_property<City, float>("time_speed", &City::time_speed, 1.0);

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
				std::cout << "DEBUG: shop before instanciating " << std::endl;
				Shop* node = (Shop*)ShopScene->instance();
				

				// REMOVE COMMENT ONCE INITIALIZE COMMAND IS CREATED
				// ((Restaurant*)node)->Restaurant::initialize();

				//Node* node = RestaurantScene->instance();
				std::cout << "DEBUG: shop instanciating " << std::endl;
				((Node*)node)->set("scale", Vector3(10, 10, 10));
				((Node*)node)->set("translation", Vector3(0, 0, 0));
				//int rot = rand() % 2;

				this->add_child((Node*)node);

				// REMOVE COMMENT ONCE INHERITANCE IS FIXED
				this->add_shop((Shop*)node);
				std::cout << "DEBUG: restaurant added to city " << std::endl;

	}
	

	if (BugattiScene.is_valid() && ChironScene.is_valid())
	{
		// TODO: This loop is only going to run once, maybe remove the loop?
		for (int z = 0; z < 0; z++) // Car removed to test
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

	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider/HSlider")->set("value", 2);
	time_speed = this->get_tree()->get_root()->get_node("Main/2Dworld/Slider/HSlider")->get("value");
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

	((Player*)(this->get_tree()->get_root()->get_node("Main/3Dworld/Player")))->set("movable", true);
	this->_on_Game_Speed_changed();

}
void godot::City::_on_Game_Speed_changed()
{
	time_speed = this->get_tree()->get_root()->get_node("Main/2Dworld/Slider/HSlider")->get("value");
	std::cout << time_speed <<std::endl;
}
;


void City::add_shop(Shop* shop) {
	all_shops.push_back(shop);
	int x = shop->get_position()[0]/30;
	int y = shop->get_position()[1]/30;
	if (x < sizeOfCity && y < sizeOfCity) {
		if (x == int(x)) {
			positionOfBuildings[x][y] = 1;
		}	
		else {
			positionOfBuildings[int(x)][int(y)] = 2;
			positionOfBuildings[int(x) + 1][int(y)] = 3;
			positionOfBuildings[int(x) + 1][int(y) + 1] = 4;
			positionOfBuildings[int(x)][int(y) + 1] = 5;
		}
		update_traffic(x, y, true);
	}
}

  
void City::update_traffic(int x, int y, bool newBuilding) {
	if (x == int(x)) {
		traffic[x][y][0][2] = 1;
		traffic[x][y][1][2] = 1;
		traffic[x][y][2][2] = 1;
		traffic[x][y][3][2] = 1;
		if (x + 1 < sizeOfCity && y + 1 > sizeOfCity && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 3)) {
			traffic[x][y][3][0] = 1;
		}
		if (y + 1 < sizeOfCity && (positionOfBuildings[x + 1][y + 1] == 1 || positionOfBuildings[x + 1][y + 1] == 2 || positionOfBuildings[x + 1][y] == 5)) {
			traffic[x][y][3][1] = 1;
		}
		if (x - 1 > 0 && y + 1 > sizeOfCity && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 3 || positionOfBuildings[x - 1][y + 1] == 4)) {
			traffic[x][y][2][0] = 1;
		}
		if (x - 1 > 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
			traffic[x][y][2][1] = 1;
		}
		if (x - 1 > 0 && y - 1 > 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
			traffic[x][y][1][0] = 1;
		}
		if (y - 1 > 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
			traffic[x][y][1][1] = 1;
		}
		if (x + 1 < sizeOfCity && y - 1 > 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
			traffic[x][y][0][0] = 1;
		}
		if (x + 1 < sizeOfCity && (positionOfBuildings[x + 1][y] == 1 || positionOfBuildings[x + 1][y] == 2 || positionOfBuildings[x + 1][y] == 3)) {
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
	else {
		x = int(x);
		y = int(y);
		traffic[x][y][0][1] = 1;
		if (x + 1 < sizeOfCity && (positionOfBuildings[x + 1][y] == 1 || positionOfBuildings[x + 1][y] == 4 || positionOfBuildings[x + 1][y] == 5)) {
			traffic[x][y][0][0] = 1;
		}
		traffic[x][y][1][2] = 1;
		if (x + 1 < sizeOfCity && y - 1 > 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
			traffic[x][y][0][0] = 1;
		}
		if (x + 1 < sizeOfCity && (positionOfBuildings[x + 1][y] == 1 || positionOfBuildings[x + 1][y] == 2 || positionOfBuildings[x + 1][y] == 3)) {
			traffic[x][y][0][1] = 1;
		}
		traffic[x+1][y][0][2] = 1;
		if (x + 2 < sizeOfCity && y + 1 > sizeOfCity && (positionOfBuildings[x + 2][y + 1] == 1 || positionOfBuildings[x + 2][y + 1] == 2 || positionOfBuildings[x + 2][y] == 3)) {
			traffic[x][y][3][0] = 1;
		}
		if (y + 1 < sizeOfCity && (positionOfBuildings[x + 2][y + 1] == 1 || positionOfBuildings[x + 2][y + 1] == 2 || positionOfBuildings[x + 2][y] == 5)) {
			traffic[x][y][3][1] = 1;
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


template<typename T> String to_godot_string(T s)
{
	std::string standardString = std::to_string(s);
	godot::String godotString = godot::String(standardString.c_str());
	return godotString;
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

	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); advance(it, 1))
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
			
			((Shop*)*it)->Shop::simulate_step(15.0);
			Godot::print((*it)->___get_godot_base_class_name());
			Godot::print((*it)->___get_class_name());
			Godot::print((*it)->get_class());
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
		int spawn = rand() % 2;
		switch (spawn) {
		case 0:add_restaurant_to_city(temp);
			break;
		case 1: add_shop_to_city(temp);

		}
		
	}
	
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


void City::add_restaurant_to_city(Vector3 position) {

	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");

	if (RestaurantScene.is_valid())
	{
		std::cout << "DEBUG: add_restaurant_to_city called " << std::endl;
		Restaurant* node = (Restaurant*)RestaurantScene->instance();

		// REMOVE COMMENT ONCE INITIALIZE COMMAND IS CREATED
		((Restaurant*)node)->Restaurant::initialize();

		//Node* node = RestaurantScene->instance();
		//std::cout << "DEBUG: restaurant instanciating " << std::endl;
		((Node*)node)->set("scale", Vector3(10, 10, 10));
		((Node*)node)->set("translation", position);
		int rot = rand() % 2;
		((Node*)node)->set("rotation_degrees", Vector3(0, 180 * rot, 0));
		this->add_child((Node*)node);
		
		// REMOVE COMMENT ONCE INHERITANCE IS FIXED
		this->add_shop((Shop*)node);
		//std::cout << "DEBUG: restaurant added to city your mom fat " << std::endl;
	}
	else { std::cout << "DEBUG: Major error, restaurant scene not valid " << std::endl; }
}


void City::add_shop_to_city(Vector3 position) {

	ResourceLoader* ResLo = ResourceLoader::get_singleton();
	Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
	Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
	Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
	Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");

	if (RestaurantScene.is_valid())
	{
		std::cout << "DEBUG: add_shop_to_city called " << std::endl;
		Shop* node = (Shop*)ShopScene->instance();

		// REMOVE COMMENT ONCE INITIALIZE COMMAND IS CREATED
		//((Shop*)node)->Shop::initialize();

		//Node* node = RestaurantScene->instance();
		//std::cout << "DEBUG: restaurant instanciating " << std::endl;
		((Node*)node)->set("scale", Vector3(10, 10, 10));
		((Node*)node)->set("translation", position);
		int rot = rand() % 2;
		((Node*)node)->set("rotation_degrees", Vector3(0, 180 * rot, 0));
		this->add_child((Node*)node);

		// REMOVE COMMENT ONCE INHERITANCE IS FIXED
		this->add_shop((Shop*)node);
		//std::cout << "DEBUG: restaurant added to city your mom fat " << std::endl;
	}
	else { std::cout << "DEBUG: Major error, restaurant scene not valid " << std::endl; }
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


