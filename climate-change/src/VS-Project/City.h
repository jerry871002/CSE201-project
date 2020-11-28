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

namespace godot {

	class City : public Object {
		GODOT_CLASS(City,Object)
	public:
		static void _register_methods();

		void _init();

		void _process(float);

		void _physics_process(float);

		void _input(InputEvent*);

		void _ready();

		City();
		~City();

		std::set<Struc*> buildings;
		double income, population, numberOfEmplyees, carbonEmission, energyDemand, energySupply;

		void add_building(Struc*);
		void simulation();
		void write_stat_history_to_file();
		float return_income();
		std::string return_game_date();

		/* we can keep these vairables as floats as long as each object only computes the ADDITIONAL AMOUNT of energy, income etc. 
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
}

