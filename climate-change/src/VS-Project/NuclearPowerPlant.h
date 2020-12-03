#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class NuclearPowerPlant : public Energy, public StaticBody {
		GODOT_CLASS(NuclearPowerPlant, StaticBody)
	public:
		NuclearPowerPlant();
		~NuclearPowerPlant();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double environmentalCost;

		//inputs:
		double naturalUranium;
		double fissileMaterial;
		//outputs:
		double energyPerDay;
		double nuclearWaste;
		double radiation;
	};
};