#pragma once
#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include <string>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include "obj.h"
#include <StaticBody.hpp>
#include <Spatial.hpp>

namespace godot {

	class City : public Spatial {
		GODOT_CLASS(City, Spatial)
	public:
		static void _register_methods();

		void _init();

		void _process(float);

		void _physics_process(float);

		void _input(InputEvent*);
<<<<<<< Updated upstream
=======
		void _ready();
		void _on_MenuShop_pressed(String Name);
		void _on_Validate_pressed();
		void _on_Game_Speed_changed();
		
>>>>>>> Stashed changes

		void _ready();

		City();
		~City();

		std::set<Structure*> buildings;
		double income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply;

		void add_building(Structure*);
		void simulation();                    //updates all the stats abd the building
		void write_stat_history_to_file();    //writes all the stats to a file so that the inteface team can make graphs 
		double return_income();               //returns the income of the city
<<<<<<< Updated upstream
=======
		// getter functions for city indices
		double return_numberOfEmployees();
		double return_carbonEmission();
		double return_energyDemand();
		double return_energySupply();
		double return_healthcare();
		double return_needs();
		double return_totalSatisfaction();
		double return_totalCo2Emissions();

>>>>>>> Stashed changes
		std::string return_game_date();       //returns the date :day/month/year as a string

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
<<<<<<< Updated upstream
		float time_speed;
		float delta_counter;
		int64_t timer;


		// this variable keeps track of the in-game days, one day added every time city.sim() is called
		int day_tick;

=======
		// city indices
		double income;
		double population;
		double numberOfEmployees;
		double carbonEmission;
		double energyDemand;
		double energySupply;
		double healthcare;
		double needs;
		double totalSatisfaction;
		double totalCo2Emissions;
		// used for caculating in-game time
		
		float delta_counter; // accumulate delta from `_physics_process` function
		int64_t timer;       // helper data to see if `delta_counter` have carry on units digit
		int day_tick; // this variable keeps track of the in-game days, 
		// one day added every time simulation() is called

		/*
		Ref<PackedScene> RestaurantScene;
		Ref<PackedScene> ShopScene;
		Ref<PackedScene> BugattiScene;
		Ref<PackedScene> ChironScene;
		*/
		ResourceLoader* ResLo = ResourceLoader::get_singleton();
		Ref<PackedScene> RestaurantScene = ResLo->load("res://Resources/Restaurant.tscn", "PackedScene");
		Ref<PackedScene> ShopScene = ResLo->load("res://Resources/Shop.tscn", "PackedScene");
		Ref<PackedScene> BugattiScene = ResLo->load("res://Resources/Bugatti.tscn", "PackedScene");
		Ref<PackedScene> ChironScene = ResLo->load("res://Resources/Chiron.tscn", "PackedScene");
>>>>>>> Stashed changes
	};
};

