#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class HighBuilding : public Housing, public StaticBody {
		GODOT_CLASS(HighBuilding, StaticBody)
	public:
		HighBuilding();
		~HighBuilding();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void simulate_step(double days);
		double coOwnershipBudget;

	};
};