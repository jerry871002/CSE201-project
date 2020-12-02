#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class CoalPowerPlant : public Energy, public StaticBody {
		GODOT_CLASS(CoalPowerPlant, StaticBody)
	public:
		CoalPowerPlant();
		~CoalPowerPlant();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double SO2Emission;
		double NOxEmission;
		double ashOutput;
		double mercuryEmission;
		double coal;
		double environmentalCost;
		double energyPerDay;
	};
};
