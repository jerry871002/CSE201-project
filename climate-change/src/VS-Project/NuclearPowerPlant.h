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
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double environmental_cost;

		//inputs:
		double natural_uranium;
		double fissile_material;
		//outputs:
		double energy_per_day;
		double nuclear_waste;
		double radiation;
	};
};