#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

class CoalPowerPlant : public Energy {
	class City : public Object {
		GODOT_CLASS(City, Object)
	public:
		CoalPowerPlant();
		~CoalPowerPlant();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double SO2_output;
		double NOx_output;
		double ash_output;
		double mercury_output;
		double coal;
		double environmental_cost;
		double energy_per_day;
	};
};
