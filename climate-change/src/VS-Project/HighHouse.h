#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class HighHouse : public Housing, public StaticBody {
		GODOT_CLASS(HighHouse, StaticBody)
	public:
		HighHouse();
		~HighHouse();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void simulate_step(double days); // updates every attribute directly as a function of time (days since beginning of the game)
		double energyUse;
		int numberOfInhabitants;
		double houseIncome;
	};
};