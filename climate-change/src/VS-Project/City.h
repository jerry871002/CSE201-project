#pragma once
#include <core/Godot.hpp>
#include <iostream>
#include <set>


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



		float income ;
		float population ;
		float employed;
		float carbon;
		float energyDemand;
		float energySupply;

		void update_statistics();


	};
}