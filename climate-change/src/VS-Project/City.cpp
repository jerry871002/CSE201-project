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
	totalSatisfaction = 100;

	time_speed = 1;


	//timer = 0;
	day_tick = 0;
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

	counter += (double)delta * time_speed;
	if (counter > 1)
	{
		simulation();
		counter -= 1;
	}
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

	if (RestaurantScene.is_valid() && ShopScene.is_valid())
	{
		for (int x = 0; x < 1; x++)
		{
			for (int z = 0; z < 2; z++)
			{
				int type = rand() % 2;
				Node* node;
				if (type == 0) { node = RestaurantScene->instance(); }
				else { node = ShopScene->instance(); }
				node->set("scale", Vector3(10, 10, 10));
				node->set("translation", Vector3(30 * x, 0, 30 * z));
				this->add_child(node);
				all_shops.push_back((Shop*)node);
			}
		}
	}
	if (MallScene.is_valid())
	{
		Node* node = MallScene->instance();
		node->set("translation", Vector3(75, 0, 45));
		this->add_child(node);
	}
}

void City::set_initial_visible_components()
{
	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("position", Vector2(20, 20));
	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("visible", true);
}


void City::_ready()
{
	this->generate_initial_city_graphics();
	this->set_initial_visible_components();
	int children_count = this->get_child_count();
}

void godot::City::_on_MenuShop_pressed(String name)
{
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/InfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->set("text", String(""));
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

	this->implement_policies((double)mytext.to_float());

}

void City::implement_policies(double value) {

	Godot::print(active_button);

	if (active_button == String("ChangePanelProbability")) {

		Godot::print(value);

		if (value >= 0 && value < 1) {
			for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
			{
				(*it)->set("panel_probability", value);
			}
		}
		
	}
}

void godot::City::_on_Game_Speed_changed()
{
	time_speed = round(pow(2, (int)(this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->get_child(0)->get("value")) - 1) - 0.1);
}
;

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

void City::simulation()
{
	// std::cout << "Simulation" << std::endl;
	/*
	day_tick++;
	//write the old values in a file 
	income = 0;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
    healthcare = 0;
	*/

	this->carbonEmission = 0;
	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
	{
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



