#pragma once
#include <core/Godot.hpp>
#include <iostream>
#include <cstdlib>
#include <set>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include "obj.h"

namespace godot {

	class City : public Object {
		GODOT_CLASS(City,Object)
	public:
		void _register_methods();

		void _init();

		void _process(float);

		void _physics_process(float);

		void _input(InputEvent*);

		void _ready();

		City();
		~City();

		std::set<Struc*> buildings;

		void add_building(Struc*);

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

