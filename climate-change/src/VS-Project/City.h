#pragma once

#include "obj.h"
#include "Shop.h"

#include <core/Godot.hpp>
#include <Spatial.hpp>

#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

#include <StaticBody.hpp>

#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <String.hpp>



/* current test fct using main_loop.cpp on mac:
g++ -std=c++17 main_loop.cpp obj.cpp edit_text_files.cpp City.cpp -ILibraries/godot-cpp-bindings/godot_headers -ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core -ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin -lgodot-cpp.osx.debug.64
then run:
./a.out
*/

namespace godot {

	class Transport;

	class City : public Spatial {

		GODOT_CLASS(City, Spatial)

	public:

		// CONSTRUCTOR, DESTRUCTOR

		City();
		~City();

		// GODOT FUNCTIONAL METHODS

		static void _register_methods();
		void _init();
		void _process(float);
		void _physics_process(float);
		void _input(InputEvent*);
		void _ready();

		// SIGNALS

		void _on_MenuShop_pressed(String Name);
		void _on_Validate_pressed();
		void _on_Game_Speed_changed();

		// INITIAL GRAPHICAL SETUP

		void generate_initial_city_graphics();
		void set_initial_visible_components();

		// TIME AND OVERALL SIMULATION

		float time_speed = 1; // 1 for regular speed (1 in-game day per second)
		double counter{ 0 }; // counter used to cast per-frame processing to a time_speed related frequency
		int day_tick; // keeps track of the in-game days, incremented when simulation() is called
		std::string return_game_date(); // returns the date :day/month/year as a string
		void simulation(); // updates all the stats and calls the simulation for the buildings

		// ARRAYS CONTAINING ALL ACTIVE ELEMENTS

		std::vector<Shop*> all_shops;

		String active_button;
		void implement_shop_policies(double);
		bool notification_active{ false };
		int notification_timeout{ 0 };
		void trigger_notification(String);
		String get_button_info_text();

		void add_car();
		                
		void write_stat_history_to_file();    //writes all the stats to a file so that the interface team can make graphs 
		
											  
		// getter functions for city indices
		double return_income();
		double return_numberOfEmployees();
		double return_carbonEmission();
		double return_energyDemand();
		double return_energySupply();
		double return_healthcare();
		double return_needs();
		double return_totalSatisfaction();

		
		

		/* we can keep these vairables as floats as long as each StaticBody only computes the ADDITIONAL AMOUNT of energy, income etc.
		and we cannot have different consequences for diff sectors (e.g. housing, production and industry) and thus implement different policies for each*/

		/* other idea: implement arrays based on sector (housing, production, infrastructure), compute additional amounts but differences between sector
		(other more radical idea: array with all buildings, not necessarily needed?)
		float income_array[3];
		float population_array[3];
		float employed_array[3];
		float carbon_array[3];
		float energyDemand_array[3];
		float energySupply_array[3];
		*/

		

	private:
		// city indices
		double income;
		double population;
		double numberOfEmployees;
		double carbonEmission;
		double energyDemand;
		double energySupply;
		double healthcare;
		double totalSatisfaction;



		ResourceLoader* ResLo = ResourceLoader::get_singleton();
		Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
		Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
		Ref<PackedScene> MallScene = ResLo->load("res://Resources/Mall.tscn", "PackedScene");
		Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
		Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");

	};
};
