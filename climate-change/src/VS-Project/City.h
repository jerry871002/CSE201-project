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

		void _ready();

		City();
		~City();

		std::set<Structure*> buildings;
		double income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply;

		void add_building(Structure*);
		void simulation();                    //updates all the stats abd the building
		void write_stat_history_to_file();    //writes all the stats to a file so that the inteface team can make graphs 
		double return_income();               //returns the income of the city
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
		float time_speed;
		float delta_counter;
		int64_t timer;


		// this variable keeps track of the in-game days, one day added every time city.sim() is called
		int day_tick;

	};
};

