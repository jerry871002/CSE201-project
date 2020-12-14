#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class NuclearPowerPlant : public Energy {
		GODOT_SUBCLASS(NuclearPowerPlant, Energy)
	public:
		virtual String class_name();
		NuclearPowerPlant();
		~NuclearPowerPlant();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		
		//inputs:
		double naturalUranium;
		double fissileMaterial;
		//outputs:
		double nuclearWaste;
		double radiation;
	};
};