#pragma once
#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include "obj.h"

namespace Godot {

	class City {
	public:
		void _register_methods();

		void _init();

		void _process(float);

		void _physics_process(float);

		void _input(InputEvent*);

		void _ready();

		City();
		~City();

		std::set<Structure*> buildings;

		void add_building(Structure*);

		float income ;
		float population ;
		float employed;
		float carbon;
		float energyDemand;
		float energySupply;

		void update_statistics();

		float return_income();
	};
}

