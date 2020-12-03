#pragma once
#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

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

		std::vector<Struc*> buildings;

		void add_building(Struc*);
		void simulation();                   // updates all the stats abd the building
		void write_stat_history_to_file();   // writes all the stats to a file so that the inteface team can make graphs 
		std::string return_game_date();      // returns the date :day/month/year as a string

		// getter functions for city indices
		double return_income();              
		double return_numberOfEmployees();
		double return_carbonEmission();
		double return_energyDemand();
		double return_energySupply();
		double return_healthcare();
		double return_needs();

	private:
		// city indices
		double income;
		double population;
		double numberOfEmployees;
		double carbonEmission;
		double energyDemand;
		double energySupply; 
		double healthcare;
		double needs;
		
		// used for caculating in-game time
		float time_speed;    // 1 for regular speed (1 in-game day per second)
		float delta_counter; // accumulate delta from `_physics_process` function
		int64_t timer;       // helper data to see if `delta_counter` have carry on units digit
		int day_tick; 		 // keeps track of the in-game days, 
							 // one day added every time city.simulate() is called

	};
};

