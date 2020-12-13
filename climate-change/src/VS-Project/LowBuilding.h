#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>


namespace godot {
class LowBuilding : public Housing, public StaticBody {
		GODOT_CLASS(LowBuilding, StaticBody)
	public:
		LowBuilding();
		~LowBuilding();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void simulate_step(double days);
		int windowCost = 200;
		int windowNumber;
		int solarPanelCost = 1500;
		double coOwnershipBudget;
	
		// policies 
		bool solar_panel(); 
		bool double_glazing(); // improve insulation
	};
};