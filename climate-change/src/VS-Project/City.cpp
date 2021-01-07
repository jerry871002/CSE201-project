#include "City.h"
#include "Transport.h"
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
#include <Color.hpp>

#include <PoolArrays.hpp>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif





using namespace godot;

int traffic_system[10][10][4][3] = { 0}; //sets everything to non-existing for the traffic array : the first to things are coordinates of the building where  the car is
				 // the third coornidate indicates the side of the building and the forth one which way the car can turn


City::City() {

	income = 0;
	population = 50000;
	numberOfEmployees = 0;
	carbonEmission = 0;
	energyDemand = 0;
	energySupply = 0;
	environmentalCost = 0;
	totalSatisfaction = 100;
	totalCo2Emissions = 100;

	time_speed = 1;


	//timer = 0;
	day_tick = 0;
	days_since_last_simulation = 0;

	// in order to find date
	daycount = 0;

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
	register_method((char*)"_physics_process", &City::_physics_process);
	register_method((char*)"_input", &City::_input);
	register_method((char*)"_ready", &City::_ready);
	register_method((char*)"_on_MenuShop_pressed", &City::_on_MenuShop_pressed);
	register_method((char*)"_on_Validate_pressed", &City::_on_Validate_pressed);
	register_method((char*)"_on_Game_Speed_changed", &City::_on_Game_Speed_changed);
	register_method((char*)"_on_ResetButton_pressed", &City::_on_ResetButton_pressed);
	register_method((char*)"_on_Reset_confirmed", &City::_on_Reset_confirmed);
	register_method((char*)"_on_Reset_cancelled", &City::_on_Reset_cancelled);
	register_method((char*)"_on_ExitButton_pressed", &City::_on_ExitButton_pressed);
	register_method((char*)"_on_Exit_confirmed", &City::_on_Exit_confirmed);
	register_method((char*)"_on_Exit_cancelled", &City::_on_Exit_cancelled);
	register_method((char*)"change_pie_chart", &City::change_pie_chart);


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

	if (bool(this->time_speed))
	{
		this->simulation_counter += (double)delta;
		this->date_counter += double(delta) * this->time_speed;
	}

	if (simulation_counter > 5)
	{
		(this->rolling_simulation_counter)++;

		if (this->rolling_simulation_counter == 0) {
			this->simulation_shops();
			change_pie_chart(carbonEmission, "PieSatisfaction");
			change_pie_chart(carbonEmission, "PieCO2");
		}
		else if (this->rolling_simulation_counter == 1) {
			this->simulation_housing();
		}
		else if (this->rolling_simulation_counter == 2) {
			this->simulation_energy();
		}
		else if (this->rolling_simulation_counter == 3) {
			this->simulation_production();
		}
		else if (this->rolling_simulation_counter == 4) {
			this->simulation_transport();
			(this->rolling_simulation_counter) -= 5;
		}

		(this->simulation_counter)--;
	}

	if (this->date_counter > 1)
	{
		(this->days_since_last_simulation)++;
		this->update_date();
		(this->date_counter)--;
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
		this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);

		this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);

		this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);
		this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);

		this->_on_Reset_cancelled();
		this->_on_Exit_cancelled();

		this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
		this->notification_active = false;
		this->notification_counter = 0;
	}

	if (i->is_action_pressed("ui_accept") && this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->get("visible")) {
		this->get_tree()->get_root()->get_node("Main/2Dworld/InvalidInputNotification")->set("visible", false);
		this->notification_active = false;
		this->notification_counter = 0;
	}

	if (i->is_action_pressed("ui_accept") && this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->get("visible"))
	{
		this->_on_Validate_pressed();
	}
};


void City::generate_initial_city_graphics()
{

	Vector3 center = Vector3(15 * citysize, 0, 15 * citysize);
	float maxdist = pow(pow(center.x, 2) + pow(center.z, 2), 1 / 2);


	// city dynamically generated based on city size


	for (int x = 0; x < citysize; x++)

		// SIMPLER CITY FOR TESTING PURPOSES ON SAD COMPUTERS


		for (int x = 0; x < 1; x++) {
			for (int z = 0; z < citysize; z++) {
				Vector3 pos = Vector3(60 * x, 0, 60 * z);

				Vector3 bigbuildingpos = pos + Vector3(15, 0, 15);
				double dist = pow(pow(center.x - bigbuildingpos.x, 2) + pow(center.z - bigbuildingpos.z, 2), (1 / 2));

				//  probability functions for buildings

				float mallprob = float(min(0, maxdist - 2 * dist));
				float nuclearprob = float(min(double(0), double(((0.01920) * (-1) * pow((20 * dist / citysize), 2) + (-1) * ((8.640) * (20 * dist / citysize) * 672.0)))));
				float fieldprob = float(min(0, -4 * maxdist + 6 * dist));
				float pastureprob = float(min(0, -4 * maxdist + 6 * dist));
				float factoryprob = float(min(0, -0.1020 * pow((20 * dist / citysize), 2) + 63.27 * (20 * dist / citysize) - 9306));


				float restaurantprob = float(2000 / (20 * dist / citysize));
				float shopprob = float(2500 / (20 * dist / citysize));
				float buildingprob = float((min(0, (-0.02000) * pow((20 * dist / citysize), 2) + 7 * (20 * dist / citysize) - 300.0)));
				float windmillprob = float(min(0, (-0.08333) * pow((20 * dist / citysize), 2) + 60.00 * (20 * dist / citysize) - 10500));

				float lowhouseprob = float(min(0, -0.2000 * pow((20 * dist / citysize), 2) + 160.0 * (20 * dist / citysize) - 3.150e+4));
				float highhouseprob = float(min(0, 0.1250 * pow((20 * dist / citysize), 2) - 17.5 * (20 * dist / citysize)));

				float smallerprob = restaurantprob + shopprob + buildingprob + windmillprob + lowhouseprob + highhouseprob;
				//std::cout << "DEBUG: About to create a random shop" << std::endl;

				int bigbuildingmaybe = (rand() % int((mallprob + nuclearprob + fieldprob + factoryprob + smallerprob)));

				if (bigbuildingmaybe < mallprob) { add_shop(bigbuildingpos, MallScene); }
				else if (bigbuildingmaybe < mallprob + nuclearprob) { add_energy(bigbuildingpos, NuclearPowerPlantScene); }
				else if (bigbuildingmaybe < mallprob + nuclearprob + fieldprob) { add_production(bigbuildingpos, FieldScene); }
				else if (bigbuildingmaybe < mallprob + nuclearprob + fieldprob + pastureprob / 2) { add_production(bigbuildingpos, SheepPastureScene); }
				else if (bigbuildingmaybe < mallprob + nuclearprob + fieldprob + pastureprob) { add_production(bigbuildingpos, PigsPastureScene); }
				else if (bigbuildingmaybe < mallprob + nuclearprob + fieldprob + pastureprob + factoryprob) { add_production(bigbuildingpos, FactoryScene); }
				else {
					for (int x1 = 0; x1 < 2; x1++)
					{
						for (int z1 = 0; z1 < 2; z1++) {

							Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);

							int type = rand() % int(smallerprob);

							if (type < restaurantprob) { add_shop(pos + pos1, RestaurantScene); }
							else if (type < restaurantprob + shopprob) { add_shop(pos + pos1, ShopScene); }
							else if (type < restaurantprob + shopprob + lowhouseprob) { add_house(pos + pos1, LowHouseScene); }
							else if (type < restaurantprob + shopprob + lowhouseprob + buildingprob) { add_house(pos + pos1, BuildingScene); }
							else if (type < restaurantprob + shopprob + lowhouseprob + buildingprob + windmillprob) { add_energy(pos + pos1, WindmillScene); }
							else { add_house(pos + pos1, HighHouseScene); }



						}
					}
				}
				// SIMPLER CITY FOR TESTING PURPOSES ON SAD COMPUTERS


				/*
				for (int x = 0; x < 1; x++)
				{
					for (int z = 0; z < 1; z++)
					{
						Vector3 pos = Vector3(60 * x, 0, 60 * z);

						//std::cout << "DEBUG: About to create a random shop" << std::endl;

						int bigbuildingmaybe = rand() % 30;
						if (bigbuildingmaybe < 5) { add_shop(pos + Vector3(15, 0, 15), MallScene); }
						else if (bigbuildingmaybe < 7) { add_energy(pos + Vector3(15, 0, 15), NuclearPowerPlantScene); }   // Make it something other than a shop !!
						else {
							for (int x1 = 0; x1 < 2; x1++)
							{
								for (int z1 = 0; z1 < 2; z1++) {
									int type = rand() % 25;
									Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);
									if (type < 3) { add_shop(pos + pos1, RestaurantScene); }
									else if (type < 8) { add_shop(pos + pos1, ShopScene); }
									else if (type < 14) { add_house(pos + pos1, LowHouseScene); }
									else if (type < 20) { add_house(pos + pos1, BuildingScene); }
									else if (type == 20) { add_energy(pos + pos1, WindmillScene); }
									else { add_house(pos + pos1, HighHouseScene); }

									for (int i = 0; i < 2; i++) {
										for (int j = 0; j < 2; j++) {
											for (int k = 0; k < 4; k++) {
												std::cout << "TRAFFIC SYSTEM : " << i << " " << j << "  " << traffic_system[i][j][k][0] << "   " << traffic_system[i][j][k][1] << "   " << traffic_system[i][j][k][2] << "  " << std::endl;
											}
										}
									}

								}
							}
						}
					}
				}

				*/


				// ACTUAL CITY

				/*

				for (int x = 0; x < 4; x++)
				{
					for (int z = 0; z < 4; z++)
					{
						Vector3 pos = Vector3(60 * x, 0, 60 * z);
						//std::cout << "DEBUG: About to create a random shop" << std::endl;

						int bigbuildingmaybe = rand() % 30;
						if (bigbuildingmaybe < 5) { add_shop(pos + Vector3(15, 0, 15), MallScene); }
						else if (bigbuildingmaybe < 7) { add_energy(pos + Vector3(15, 0, 15), NuclearPowerPlantScene); }   // Make it something other than a shop !!
						else {
							for (int x1 = 0; x1 < 2; x1++)
							{
								for (int z1 = 0; z1 < 2; z1++) {
									int type = rand() % 25;
									Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);
									if (type < 3) { add_shop(pos + pos1, RestaurantScene); }
									else if (type < 8) { add_shop(pos + pos1, ShopScene); }
									else if (type < 14) { add_house(pos + pos1, LowHouseScene); }
									else if (type < 20) { add_house(pos + pos1, BuildingScene); }
									else if (type == 20) { add_energy(pos + pos1, WindmillScene); }
									else { add_house(pos + pos1, HighHouseScene); }
								}
							}
						}



					}
				}


				for (int x = 4; x < 8; x++)
				{
					for (int z = 0; z < 8; z++)
					{
						Vector3 pos = Vector3(60 * x, 0, 60 * z);
						int fieldmaybe = rand() % 30;
						if (fieldmaybe < 7) { add_production(pos + Vector3(15, 0, 15), FieldScene); }
						else if (fieldmaybe < 14) { add_production(pos + Vector3(15, 0, 15), SheepPastureScene); }
						else if (fieldmaybe < 21) { add_production(pos + Vector3(15, 0, 15), PigsPastureScene); }
						else {
							for (int x1 = 0; x1 < 2; x1++)
							{
								for (int z1 = 0; z1 < 2; z1++) {
									int type = rand() % 25;
									Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);
									if (type < 1) { add_shop(pos + pos1, RestaurantScene); }
									else if (type < 2) { add_shop(pos + pos1, ShopScene); }
									else if (type < 17) { add_house(pos + pos1, LowHouseScene); }
									else if (type < 23) { add_energy(pos + pos1, WindmillScene); }
									else { add_house(pos + pos1, HighHouseScene); }
								}
							}
						}
					}
				}
				for (int x = 0; x < 4; x++)
				{
					for (int z = 4; z < 8; z++)
					{
						Vector3 pos = Vector3(60 * x, 0, 60 * z);
						int fieldmaybe = rand() % 30;
						if (fieldmaybe < 7) { add_production(pos + Vector3(15, 0, 15), FieldScene); }
						else if (fieldmaybe < 14) { add_production(pos + Vector3(15, 0, 15), SheepPastureScene); }
						else if (fieldmaybe < 21) { add_production(pos + Vector3(15, 0, 15), PigsPastureScene); }
						else {
							for (int x1 = 0; x1 < 2; x1++)
							{
								for (int z1 = 0; z1 < 2; z1++) {
									int type = rand() % 25;
									Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);
									if (type < 1) { add_shop(pos + pos1, RestaurantScene); }
									else if (type < 2) { add_shop(pos + pos1, ShopScene); }
									else if (type < 17) { add_house(pos + pos1, LowHouseScene); }
									else if (type < 23) { add_energy(pos + pos1, WindmillScene); }
									else { add_house(pos + pos1, HighHouseScene); }
								}
							}
						}
					}
				}
				for (int x = 8; x < 12; x++)
				{
					for (int z = 0; z < 8; z++)
					{
						Vector3 pos = Vector3(60 * x, 0, 60 * z);
						int fieldmaybe = rand() % 30;
						if (fieldmaybe < 5) { add_production(pos + Vector3(15, 0, 15), NuclearPowerPlantScene); }
						else if (fieldmaybe < 8) { add_production(pos + Vector3(15, 0, 15), MallScene); }
						else {
							for (int x1 = 0; x1 < 2; x1++)
							{
								for (int z1 = 0; z1 < 2; z1++) {
									int type = rand() % 25;
									Vector3 pos1 = Vector3(30 * x1, 0, 30 * z1);
									if (type < 20) { add_shop(pos + pos1, ShopScene); }
									else { add_house(pos + pos1, BuildingScene); }
								}
							}
						}
					}
				}
				*/

			}
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

	this->get_tree()->get_root()->get_node("Main/2Dworld/Slider")->set("visible", true);

	this->get_tree()->get_root()->get_node("Main/2Dworld/ResetButton")->set("visible", true);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/ExitButton")->set("visible", true);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ExitConfirmationBox")->set("visible", false);

}


void City::_ready()
{

	std::cout << "DEBUG: Ready started" << std::endl;
	this->generate_initial_city_graphics();
	this->set_initial_visible_components();

}

void City::_on_ExitButton_pressed()
{
	
	this->_on_Reset_cancelled();
	this->time_speed = 0;

	this->get_tree()->get_root()->get_node("Main/2Dworld/ExitConfirmationBox")->set("visible", true);
	this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);
	
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
	
}

void City::_on_Exit_cancelled()
{
	this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ExitConfirmationBox")->set("visible", false);
	this->_on_Game_Speed_changed();
}

void City::_on_Exit_confirmed()
{
	this->get_tree()->quit();
}


void City::_on_ResetButton_pressed() 
{
	
	this->_on_Exit_cancelled();
	this->time_speed = 0;

	this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", true);
	this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", true);
	
	this->get_tree()->get_root()->get_node("Main/2Dworld/Menus/MenuShop")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld")->get_node("InfoBox")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ButtonInfoBox")->set("visible", false);

	this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput")->set("visible", false);
	
}

void City::_on_Reset_cancelled() 
{
	this->get_tree()->get_root()->get_node("Main/3Dworld/Player")->set("movable", true);
	this->get_tree()->get_root()->get_node("Main/2Dworld/Blur")->set("visible", false);
	this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);
	this->_on_Game_Speed_changed();
}

void City::_on_Reset_confirmed() 
{
	
	/*
	
	this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);


	int city_child_count = this->get_child_count();
	for (int i = city_child_count - 1; i >= 0; --i) {
		if (this->get_child(i)->get("name") != String("WorldEnvironment") && this->get_child(i)->get("name") != String("Player")) {
			this->remove_child(this->get_child(i));
		}
	}

	Node* PlayerNode = this->get_node("Player");
	this->remove_child(PlayerNode);
	this->add_child(PlayerNode);

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
	daycount = 0;

	// in order to write stats to csv files
	stat = 0;

	srand((int)time(0));

	this->_init();
	this->_ready();
	*/

	this->get_tree()->reload_current_scene();
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

	this->get_tree()->get_root()->get_node("Main/2Dworld/ResetConfirmationBox")->set("visible", false);

}

String City::get_button_info_text() {

	// This method returns text giving information about the various policies 
	// The placeholder text for the input box changed to correpsond to the old value of the "policy"

	// I DON'T KNOW HOW TO DO THE SECOND PART YET 
	// I GUESS THESE VALUES SHOULD BE STORED IN CITY AND NOT FETCHED FROM RANDOM OBJECTS

	if (this->active_button == String("PanelSubsidyForShops"))
	{
		//this->get_tree()->get_root()->get_node("Main/2Dworld/PoliciesInput/TextEdit")->set("placeholder_text", String(""));
		return String("Please input a value between 0 and 450. This will be a solar panel subsidy for shops.");
	}
	else if (this->active_button == String("WindTurbineSubsidyForShops"))  // Changed the name here as well
	{
		return String("Please input a value between 0 and 800. This will be a wind Turbine subsidy for shops.");
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

	(this->get_tree()->get_root()->get_node("Main/3Dworld/Player"))->set("movable", true);
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

	if (this->active_button == String("PanelSubsidyForShops")) {
		if (value >= 0 && value <= 450) {
			Godot::print("PANEL SUBSIDY WILL BE CHANGED FOR ALL SHOPS");
			for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
			{
				(*it)->set("solar_panel_subsidies", value);
			}
		}
		else {
			this->trigger_notification(String("The value you provided was not in the specified range."));
		}
	}
	else if (this->active_button == String("WindTurbineSubsidyForShops")) {    ///I Changed the name here
		if (value >= 0 && value < 800) {
			Godot::print("PANEL PROBABILITY WILL BE CHANGED ONLY FOR SHOPS");
			for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
			{
				//if ((String)(*it)->get("object_type") == (String)("Restaurant")) {}
				(*it)->set("wind_turbine_subsidies", value);
				
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
	(this->get_tree()->get_root()->get_node("Main/3Dworld/Player"))->set("movable", true);
}


void City::add_car() {


	const Ref<PackedScene> BugattiScene = ResourceLoader::get_singleton()->load("res://Resources/Bugatti.tscn", "PackedScene");
	const Ref<PackedScene> ChironScene = ResourceLoader::get_singleton()->load("res://Resources/Chiron.tscn", "PackedScene");
	const Ref<PackedScene> MotoScene = ResourceLoader::get_singleton()->load("res://Resources/Moto.tscn", "PackedScene");

	if (BugattiScene.is_valid() && ChironScene.is_valid() && MotoScene.is_valid())
	{

		// randomly choose between bugatti and chiron

		int type = rand() % 3;

		Node* node;

		switch (type) {
		case 0: node = BugattiScene->instance(); break;
		case 1: node = ChironScene->instance(); break;
		default: node = MotoScene->instance(); break;
		}
		
		node->set("scale", Vector3(10, 10, 10));
		node->set("translation", Vector3(-13, 0, -13 + 30 * (0 + 1)));
		
		
		this->add_child((Node*)node);
		((Transport*)node)->set("transportType", type);

		//((Transport*)node)->transport_type();


		//income -= node->cost;

		//all_transports.push_back((Transport*)node);
	}
}

void City::add_shop(Vector3 pos, Ref<PackedScene> scene) {

	//std::cout << "DEBUG: add shop called" << std::endl;

	//std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
	if (scene.is_valid()) {
		//std::cout << "DEBUG: creating node" << std::endl;
		Node* node;
		//std::cout << "DEBUG: instanciating" << std::endl;
		node = scene->instance();
		//std::cout << "DEBUG: setting scale and translation" << std::endl;
		node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
		node->set("translation", pos);
		node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0));
		//std::cout << "DEBUG: add child" << std::endl;
		this->add_child(node);
		//std::cout << "DEBUG: add shop to vector" << std::endl;
		all_shops.push_back((Shop*)node);

		//std::cout << "DEBUG: traffic stuff called" << std::endl;
		
		// traffic stuff
		std::cout << "DEBUG: traffic stuff called" << std::endl;
		//double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
		//double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
		double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
		double y = pos.z / 30; // can be int only for small building

		std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
		std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
		std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
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
			std::cout << "DEBUG: call the function update traffic" << std::endl;
			update_traffic(int(x), int(y), true, positionOfBuildings[int(x)][int(y)]);

			//for (int k = 0; k < 4; k++) {
			//	std::cout << "start :" << x << " " << y << " " << traffic[int(x)][int(y)][0][0] << " " << traffic[int(x)][int(y)][k][1] << " " << traffic[int(x)][int(y)][k][2] << " " << std::endl;
			//}

			//for(int i = 0; i < 10; i++){
			//	for (int j = 0; j < 10; j++) {
			//		for (int k = 0; k < 4; k++) {
			//				std::cout << "start :" << x << " " << y << traffic[i][j][k][0] << '   ' << traffic[i][j][k][1] << '   ' << traffic[i][j][k][2] << '  ' << std::endl;
			//		}
			//	}
			//}

			std::cout << std::endl;
		}
	}
}


void City::add_house(Vector3 pos, Ref<PackedScene> scene) {

	//std::cout << "DEBUG: add house called" << std::endl;

	//std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
	if (scene.is_valid()) {
		//std::cout << "DEBUG: creating node" << std::endl;
		Node* node;
		//std::cout << "DEBUG: instanciating" << std::endl;
		node = scene->instance();
		//std::cout << "DEBUG: setting scale and translation" << std::endl;
		node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
		node->set("translation", pos);
		node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0));
		//std::cout << "DEBUG: add child" << std::endl;
		this->add_child(node);
		//std::cout << "DEBUG: add house to vector" << std::endl;
		all_houses.push_back((Housing*)node);



		// traffic stuff
		std::cout << "DEBUG: traffic stuff called" << std::endl;
		//double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
		//double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
		double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
		double y = pos.z / 30; // can be int only for small building

		std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
		std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
		std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
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
			std::cout << "DEBUG: call the function update traffic" << std::endl;
			update_traffic(int(x), int(y), true, positionOfBuildings[int(x)][int(y)]);

			//for (int k = 0; k < 4; k++) {
			//	std::cout << "start :" << x << " " << y << " " << traffic[int(x)][int(y)][0][0] << " " << traffic[int(x)][int(y)][k][1] << " " << traffic[int(x)][int(y)][k][2] << " " << std::endl;
			//}

			//for(int i = 0; i < 10; i++){
			//	for (int j = 0; j < 10; j++) {
			//		for (int k = 0; k < 4; k++) {
			//				std::cout << "start :" << x << " " << y << traffic[i][j][k][0] << '   ' << traffic[i][j][k][1] << '   ' << traffic[i][j][k][2] << '  ' << std::endl;
			//		}
			//	}
			//}

			std::cout << std::endl;
		}
		std::cout << "DEBUG: add house done" << std::endl;
	}
}


void City::add_energy(Vector3 pos, Ref<PackedScene> scene) {

	//std::cout << "DEBUG: add shop called" << std::endl;

	//std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
	if (scene.is_valid()) {
		//std::cout << "DEBUG: creating node" << std::endl;
		Node* node;
		//std::cout << "DEBUG: instanciating" << std::endl;
		node = scene->instance();
		//std::cout << "DEBUG: setting scale and translation" << std::endl;
		node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
		node->set("translation", pos);
		node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0));
		//std::cout << "DEBUG: add child" << std::endl;
		this->add_child(node);
		//std::cout << "DEBUG: add shop to vector" << std::endl;
		all_energies.push_back((Energy*)node);


		//std::cout << "DEBUG: traffic stuff called" << std::endl;
		// traffic stuff
		std::cout << "DEBUG: traffic stuff called" << std::endl;
		//double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
		//double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
		double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
		double y = pos.z / 30; // can be int only for small building

		std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
		std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
		std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
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
			std::cout << "DEBUG: call the function update traffic" << std::endl;
			update_traffic(int(x), int(y), true, positionOfBuildings[int(x)][int(y)]);

			//for (int k = 0; k < 4; k++) {
			//	std::cout << "start :" << x << " " << y << " " << traffic[int(x)][int(y)][0][0] << " " << traffic[int(x)][int(y)][k][1] << " " << traffic[int(x)][int(y)][k][2] << " " << std::endl;
			//}

			//for(int i = 0; i < 10; i++){
			//	for (int j = 0; j < 10; j++) {
			//		for (int k = 0; k < 4; k++) {
			//				std::cout << "start :" << x << " " << y << traffic[i][j][k][0] << '   ' << traffic[i][j][k][1] << '   ' << traffic[i][j][k][2] << '  ' << std::endl;
			//		}
			//	}
			//}

			std::cout << std::endl;
		}
		std::cout << "DEBUG: add energy done" << std::endl;
	}
}


void City::add_production(Vector3 pos, Ref<PackedScene> scene) {

	//std::cout << "DEBUG: add shop called" << std::endl;

	//std::cout << "DEBUG: scene is valid  " << scene.is_valid() << std::endl;
	if (scene.is_valid()) {
		//std::cout << "DEBUG: creating node" << std::endl;
		Node* node;
		//std::cout << "DEBUG: instanciating" << std::endl;
		node = scene->instance();
		//std::cout << "DEBUG: setting scale and translation" << std::endl;
		node->set("scale", Vector3(10, 10, 10));  //9 + ((double(rand()) / RAND_MAX) * 2)
		node->set("translation", pos);
		node->set("rotation_degrees", Vector3(0, 180 * (rand() % 2), 0));
		//std::cout << "DEBUG: add child" << std::endl;
		this->add_child(node);
		//std::cout << "DEBUG: add shop to vector" << std::endl;
		all_production.push_back((Production*)node);


		

		// traffic stuff
		std::cout << "DEBUG: traffic stuff called" << std::endl;
		//double x = ((Structure*)node)->get_position().x / 30; // needs to be double for identifying a 2 by 2 building
		//double y = ((Structure*)node)->get_position().z / 30; // can be int only for small building
		double x = pos.x / 30; // needs to be double for identifying a 2 by 2 building
		double y = pos.z / 30; // can be int only for small building

		std::cout << "DEBUG: coordinates " << x << " . " << y << std::endl;
		std::cout << "DEBUG: size city " << sizeOfCity << std::endl;
		std::cout << "DEBUG: position  " << pos.x << " . " << pos.z << std::endl;
		if (x < sizeOfCity && y < sizeOfCity) {
			if (x > int(x) - 0.1 && x < int(x) + 0.1) { // check that it's a small building
				positionOfBuildings[int(x)][int(y)] = 1;
				std::cout << " SMALL BUILDING CREATED" << std::endl;
			}
			else {
				positionOfBuildings[int(x)][int(y)] = 2; // assign numbers to the four squares of the 2 by 2 buidling to know it's position by knowing just the coordinates and the number of one square
				positionOfBuildings[int(x) + 1][int(y)] = 3;
				positionOfBuildings[int(x) + 1][int(y) + 1] = 4;
				positionOfBuildings[int(x)][int(y) + 1] = 5;
			}
			std::cout << "DEBUG: call the function update traffic" << std::endl;
			update_traffic(int(x), int(y), true, positionOfBuildings[int(x)][int(y)]);

			//for (int k = 0; k < 4; k++) {
			//	std::cout << "start :" << x << " " << y << " " << traffic[int(x)][int(y)][0][0] << " " << traffic[int(x)][int(y)][k][1] << " " << traffic[int(x)][int(y)][k][2] << " " << std::endl;
			//}

			//for(int i = 0; i < 10; i++){
			//	for (int j = 0; j < 10; j++) {
			//		for (int k = 0; k < 4; k++) {
			//				std::cout << "start :" << x << " " << y << traffic[i][j][k][0] << '   ' << traffic[i][j][k][1] << '   ' << traffic[i][j][k][2] << '  ' << std::endl;
			//		}
			//	}
			//}

			std::cout << std::endl;
		}
		std::cout << "DEBUG: add production done" << std::endl;
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
	std::cout << "DEBUG: UPDATE TRAFFIC STARTED for coordinates" << x << " " << y << " " << positionOfBuildings[x][y] << std::endl;
	if (positionOfBuildings[x][y] != 0) { // nothing happens if the building isn't there
		std::cout << "DEBUG: updating traffic for coordinates : " << x << " " << y << " " << positionOfBuildings[x][y] << std::endl;
		if (number == 1) {  // the case when it's a 1 by 1 buidling
			traffic_system[x][y][0][2] = 1;
			traffic_system[x][y][1][2] = 1;
			traffic_system[x][y][2][2] = 1;
			traffic_system[x][y][3][2] = 1;
			if (x + 1 < sizeOfCity && y + 1 < sizeOfCity && (positionOfBuildings[x + 1][y + 1] == 1 || positionOfBuildings[x + 1][y + 1] == 2 || positionOfBuildings[x + 1][y + 1] == 3)) {
				traffic_system[x][y][3][0] = 1;
			}
			if (y + 1 < sizeOfCity && (positionOfBuildings[x][y + 1] == 1 || positionOfBuildings[x][y + 1] == 2 || positionOfBuildings[x][y + 1] == 5)) {
				traffic_system[x][y][3][1] = 1;
			}
			if (x - 1 >= 0 && y + 1 < sizeOfCity && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 3 || positionOfBuildings[x - 1][y + 1] == 4)) {
				traffic_system[x][y][2][0] = 1;
			}
			if (x - 1 >= 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
				traffic_system[x][y][2][1] = 1;
			}
			if (x - 1 >= 0 && y - 1 >= 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
				traffic_system[x][y][1][0] = 1;
			}
			if (y - 1 >= 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
				traffic_system[x][y][1][1] = 1;
			}
			if (x + 1 < sizeOfCity && y - 1 >= 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
				traffic_system[x][y][0][0] = 1;
			}
			if (x + 1 < sizeOfCity && (positionOfBuildings[x + 1][y] == 1 || positionOfBuildings[x + 1][y] == 2 || positionOfBuildings[x + 1][y] == 3)) {
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
				x = x - 1;
			}
			if (number == 4) {
				x = x - 1;
				y = y - 1;
			}
			if (number == 5) {
				y = y - 1;
			}
			traffic_system[x][y][0][1] = 1;
			if (y - 1 >= 0 && (positionOfBuildings[x + 1][y - 1] == 1 || positionOfBuildings[x + 1][y - 1] == 2 || positionOfBuildings[x + 1][y - 1] == 5)) {
				traffic_system[x][y][0][0] = 1;
			}

			traffic_system[x + 1][y][0][2] = 1;
			if (x + 2 < sizeOfCity && y - 1 >= 0 && (positionOfBuildings[x + 2][y - 1] == 1 || positionOfBuildings[x + 2][y - 1] == 2 || positionOfBuildings[x + 2][y - 1] == 5)) {
				traffic_system[x + 1][y][0][0] = 1;
			}
			if (x + 2 < sizeOfCity && (positionOfBuildings[x + 2][y] == 1 || positionOfBuildings[x + 2][y] == 2 || positionOfBuildings[x + 2][y] == 3)) {
				traffic_system[x + 1][y][0][1] = 1;
			}

			traffic_system[x + 1][y][3][1] = 1;
			if (x + 2 < sizeOfCity && (positionOfBuildings[x + 2][y + 1] == 1 || positionOfBuildings[x + 2][y + 1] == 2 || positionOfBuildings[x + 2][y + 1] == 3)) {
				traffic_system[x + 1][y][3][0] = 1;
			}

			traffic_system[x + 1][y + 1][3][2] = 1;
			if (x + 2 < sizeOfCity && y + 2 < sizeOfCity && (positionOfBuildings[x + 2][y + 2] == 1 || positionOfBuildings[x + 2][y + 2] == 2 || positionOfBuildings[x + 2][y + 2] == 3)) {
				traffic_system[x + 1][y + 1][3][0] = 1;
			}
			if (y + 2 < sizeOfCity && (positionOfBuildings[x + 2][y] == 1 || positionOfBuildings[x + 2][y] == 3 || positionOfBuildings[x + 2][y] == 4)) {
				traffic_system[x + 1][y + 1][3][1] = 1;
			}

			traffic_system[x + 1][y][2][1] = 1;
			if (y + 2 < sizeOfCity && (positionOfBuildings[x][y + 2] == 1 || positionOfBuildings[x][y + 2] == 3 || positionOfBuildings[x][y + 2] == 4)) {
				traffic_system[x + 1][y][2][0] = 1;
			}

			traffic_system[x][y + 1][2][2] = 1;
			if (x - 1 >= 0 && y + 2 < sizeOfCity && (positionOfBuildings[x - 1][y + 2] == 1 || positionOfBuildings[x - 1][y + 2] == 3 || positionOfBuildings[x - 1][y + 2] == 4)) {
				traffic_system[x][y + 1][2][0] = 1;
			}
			if (x - 1 >= 0 && (positionOfBuildings[x - 1][y + 1] == 1 || positionOfBuildings[x - 1][y + 1] == 4 || positionOfBuildings[x - 1][y + 1] == 5)) {
				traffic_system[x][y + 1][2][1] = 1;
			}

			traffic_system[x + 1][y][1][1] = 1;
			if (x - 1 >= 0 && (positionOfBuildings[x - 1][y] == 1 || positionOfBuildings[x - 1][y] == 4 || positionOfBuildings[x - 1][y] == 5)) {
				traffic_system[x + 1][y][1][0] = 1;
			}

			traffic_system[x][y][1][2] = 1;
			if (x - 1 >= 0 && y - 1 >= 0 && (positionOfBuildings[x - 1][y - 1] == 1 || positionOfBuildings[x - 1][y - 1] == 4 || positionOfBuildings[x - 1][y - 1] == 5)) {
				traffic_system[x][y][1][0] = 1;
			}
			if (y - 1 >= 0 && (positionOfBuildings[x][y - 1] == 1 || positionOfBuildings[x][y - 1] == 2 || positionOfBuildings[x][y - 1] == 5)) {
				traffic_system[x][y][1][1] = 1;
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




void City::simulation_shops()
{
	std::cout << "Simulation SHOPS" << std::endl;

	day_tick += this->time_speed * 5;
	this->days_since_last_simulation = 0;
	Godot::print(return_word_date());
	this->get_tree()->get_root()->get_node("Main/GUI/GUIComponents/TimeControls/Date")->set("text", return_word_date());
	

	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
	{
		(*it)->set("updatable", true);
	}

}

void City::simulation_production()
{
	std::cout << "Simulation PRODUCTION" << std::endl;

	for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
	{
		(*it)->set("updatable", true);
	}

}

void City::simulation_energy()
{
	std::cout << "Simulation ENERGY" << std::endl;


	for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
	{
		(*it)->set("updatable", true);
	}

}

void City::simulation_housing() 
{

	std::cout << "Simulation HOUSING" << std::endl;

	for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
	{
		(*it)->set("updatable", true);
	}

	/*

	//   function which looks at a single random house at gives it the correct satisfaction then updates the total satisfaction
	Housing* h = all_houses.at(rand() % all_houses.size());
	double initialval = (double)(h->get("satisfaction"));
	Vector3 pos = ((Structure*)h)->get_position();
	double dist = 300;   // the distance where houses take into account the satisfaction
	int obj_count = 0;
	double tothouseSat = 0.0;

	this->totalSatisfaction = (totalSatisfaction * all_houses.size() - initialval);

	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
	{
		if (((Structure*)(*it))->is_other_structure_within_distance(pos, dist)) {
			tothouseSat += (double)((*it)->get("satisfaction")) * 10;
			obj_count++;
		}
	}
	for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
	{
		if (((Structure*)(*it))->is_other_structure_within_distance(pos, dist)) {
			tothouseSat += (double)((*it)->get("satisfaction")) * 10;
			obj_count++;
		}
	}
	for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
	{
		if (((Structure*)(*it))->is_other_structure_within_distance(pos, dist)) {
			tothouseSat += (double)((*it)->get("satisfaction")) * 10;
			obj_count++;
		}
	}
	tothouseSat /= obj_count;
	this->totalSatisfaction = (totalSatisfaction + tothouseSat) / all_houses.size();

	*/

}

void City::simulation_transport() 
{

	std::cout << "Simulation TRANSPORT" << std::endl;

	this->income = 0;
	this->population = 50000;
	this->carbonEmission = 0;
	this->numberOfEmployees = 0;
	this->energyDemand = 0;
	this->energySupply = 0;
	this->totalSatisfaction = 50;

	for (std::vector<Transport*>::iterator it = all_transports.begin(); it != all_transports.end(); ++it)
	{
		// run the vechicle simulation
		// count up all the vehicle stuff
	}

	// COUNT UP ALL INDICES FOR EVERYTHING

	for (std::vector<Shop*>::iterator it = all_shops.begin(); it != all_shops.end(); ++it)
	{
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		this->numberOfEmployees += (double)((*it)->get("employment"));
		this->income += (double)((*it)->get("employment")) * (double)((*it)->get("averageWage"));

		this->energyDemand += (double)((*it)->get("energyUse"));
		this->environmentalCost += (double)((*it)->get("environmentalCost"));
	}

	for (std::vector<Housing*>::iterator it = all_houses.begin(); it != all_houses.end(); ++it)
	{
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		
		// this->totalSatisfaction += (double)((*it)->get("satisfaction")) * 10;        satisfaction should be changed in the function below, with the day tick %4
	}

	for (std::vector<Energy*>::iterator it = all_energies.begin(); it != all_energies.end(); ++it)
	{
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		//this->totalSatisfaction += (double)((*it)->get("satisfaction")) * 10;
		this->energySupply += (double)((*it)->get("energyperDay"));
		this->numberOfEmployees += (double)((*it)->get("employment"));
		this->income += (double)((*it)->get("employment")) * (double)((*it)->get("averageWage"));
	}

	for (std::vector<Production*>::iterator it = all_production.begin(); it != all_production.end(); ++it)
	{
		this->carbonEmission += (double)((*it)->get("CO2Emission"));
		//this->totalSatisfaction += (double)((*it)->get("satisfaction")) * 10;
		this->energyDemand += (double)((*it)->get("energyUse"));
		this->environmentalCost += (double)((*it)->get("environmentalCost"));
		this->numberOfEmployees += (double)((*it)->get("employment"));
		this->income += (double)((*it)->get("employment")) * (double)((*it)->get("averageWage"));
	}

}



template<typename T> String to_godot_string(T s)
{
	std::string standardString = std::to_string(s);
	godot::String godotString = godot::String(standardString.c_str());
	return godotString;
}

int* return_date(int day_tick) {
	int static date[3];
	int Y = 1, M = 1, D = 1;
	int julian = (1461 * (Y + 4800 + (M - 14) / 12)) / 4 + (367 * (M - 2 - 12 / ((M - 14) / 12))) / 12 - (3 * ((Y + 4900 + (M - 14) / 12) / 100)) / 4 + D - 32075 + day_tick + 35;
	int gregorian = julian + 1401 + (int)((((int)(4 * day_tick + 274277) / 146097) * 3) / 4) - 38;
	int e = 4 * gregorian + 3;
	int h = 5 * ((int)(e % 1461) / 4) + 2;
	int day = ((int)(h % 153) / 5) + 1;
	int month = (((int)h / 153) + 2) % 12 + 1;
	int year = (int)(e / 1461) - 4716 + (int)((14 - month) / 12);
	date[0] = day;
	date[1] = month;
	date[2] = year;
	return date;
}


String return_number_date(int day, int month, int year) {
	return to_godot_string(day) + String(", ") + to_godot_string(month) + String(", ") + to_godot_string(year);
}


double find_avg(double array[], int leap) {
	int size;
	double sum = 0;
	if (leap == 0) {
		size = 366;
	}
	else {
		size = 365;
	}
	for (int i = 0; i < size; i++) {
		sum += array[i];
	}
	return sum / size;
}


String City::return_word_date() {

	int* datenumber = return_date(int(this->day_tick));
	int year = datenumber[2];
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

void City::transport_probabilities(){
	        /*
        * 0 - electic car
        * 1 - big american car
        * 2 - normal car
        * 3 - old collection car
        * 4 - bike
        * 5 - motorcycle
        * 6 - bus
        * 7 - sports car
        */
double satisfactions[8] = {9.7, 8.5,6.8, 9.3, 9, 7, 8, 9.5};
		double satisfactionsSum = 67.8;
		double alpha[8];
		for(int i=0;i<8;i++){
			alpha[i] = satisfactions[i]/satisfactionsSum*incomesLen;
			if ((i==4)||(i==5)){
				alpha[i]*=sqrt(airQuality);
			}
		}
		double lifetimes[8] = {15, 10, 15, 20, 20, 12, 10, 10};
		double capacities[8]={5, 8, 4, 2, 1, 1, 45, 2};
		double costs[8] = {42000, 85000, 14000,40000, 370, 6200, 262500, 52000};
		double pricesPerMonth[8];
		double probabilities[8];
		double quantities[8] = {0,0,0,0,0,0,0,0};
		double alphaSum = 0;
		for (int i=0;i<8;i++){
			alphaSum+=alpha[i];
		}
		for (int i = 0;i<8; i++){
			pricesPerMonth[i] = costs[i]/(12*lifetimes[i]);
			alpha[i] =0.01*alpha[i]/alphaSum;
		}
		alphaSum = 0;
		for (int i=0;i<8;i++){
			alphaSum+=alpha[i];
		}
		for (int n=0;n<incomesLen;n++){
			double choice[8]={0,0,0,0,0,0,0,0};
			for (int i=0;i<8;i++){
				probabilities[i]=alpha[i]*(incomes[n]/pricesPerMonth[i])/alphaSum;
				if (probabilities[i]>1){
				    choice[i] = alpha[i];
				}
				else {
				    choice[i]=0;
				}			}
			int maxIndex = 0;
			double maxChoice = choice[0];
			for (int i=1;i<8;i++){
				if (choice[i]>maxChoice){
					maxChoice = choice[i];
					maxIndex = i;
				}
			}
			quantities[maxIndex]+=1;
		}
		for (int i=0;i<8;i++){
		    quantities[i]/=capacities[i];
		}
		double quantitiesSum=0;
		for (int i=0;i<8;i++ ){
			quantitiesSum+=quantities[i];
		}
		probabilityElectricCar = quantities[0]/quantitiesSum;
		probabilityBigCar = quantities[1]/quantitiesSum;
		probabilityCar = quantities[2]/quantitiesSum;
		probabilityCollectionCar = quantities[3]/quantitiesSum;
		probabilityBike = quantities[4]/quantitiesSum;
		probabilityMotorcycle = quantities[5]/quantitiesSum;
		probabilityBus = quantities[6]/quantitiesSum;
		probabilitySportsCar = quantities[7]/quantitiesSum;

}

// All inputs are strings for the functions below.
// The documentName for the file pollution.csv that is located in the "data" folder is "pollution".
// Ideally, all csv files should be stored in the "data" folder.

string get_path(string documentName) {
    return "../../addons/easy_charts/file.samples/" + documentName + ".csv";
}

// Function to add a line of the form "2015;76" to the csv file named documentName.
// To do so, call add_data("pollution", "2015", "76");
void add_data(string documentName, string year, string value) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::app);
    file << year << ";" << value << '\n';
    file.close();
}

// Suppresses all data stored in the file documentName
void clear_completely(string documentName) {
    fstream file;
    string path = get_path(documentName);
    file.open(path, ios::out | ios::trunc);
    file.close();
}

// Suppresses all data stored in the file documentName except the first line.
void clear(string documentName) {
    fstream file;
    string path = get_path(documentName);

    file.open(path);
    string line;
    getline(file, line);
    file.close();

    file.open(path, ios::out | ios::trunc);
    file.close();

    file.open(path);
    file << line << '\n';
    file.close();
}

// Copies the data stored in documentNameFrom to an empty file documentNameTo.
void copy(string documentNameFrom, string documentNameTo) {
    fstream fileFrom;
    fstream fileTo;
    string path1 = get_path(documentNameFrom);
    string path2 = get_path(documentNameTo);
    fileFrom.open(path1);
    fileTo.open(path2);
    while (fileFrom.good()) {
        string line;
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
void change_data(string documentName, string dataToChange, string newValue) {
    fstream file;
    fstream temp;
    string path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        string line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            string sub = line.substr(0, pos);
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
void delete_line(string documentName, string dataToDelete) {
    fstream file;
    fstream temp;
    string path = get_path(documentName);
    file.open(path);
    temp.open("../../addons/easy_charts/file.samples/datas_on_rows.csv");

    while (file.good()) {
        string line;
        getline(file, line, '\n');
        if (line.length() > 0) {
            int pos = line.find(";");
            string sub = line.substr(0, pos);
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


/*
//in order to check for errors on mac
int main() {
	City c = City();
	c.simulation();
	std::cout << "DEBUG: TOTAL CARBON EMISSION = " << c.return_carbonEmission() << std::endl;
	return 0;
}
*/

void City::change_pie_chart(int totalSatisfaction, NodePath name)
{
	Color green = Color(0, 1, 0, 1);
	Color orange = Color(1, 0.65, 0, 1);
	Color red = Color(1, 0, 0, 1);
	TextureProgress* node = ((TextureProgress*)this->get_parent()->get_child(1)->get_node("Infographics")->get_node(name));
	node->set_tint_progress(green);
	if (totalSatisfaction < 70) {
		node->set_tint_progress(green);
	}
	if (totalSatisfaction < 33) {
		node->set_tint_progress(green);
	}
	node->set("value", totalSatisfaction);
}