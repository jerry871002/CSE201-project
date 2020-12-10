#include "obj.h"
#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>

namespace godot {
//all these numbers are for a house of 100m^2
class LowHouse : public Housing, public StaticBody  {
		GODOT_CLASS(LowHouse, StaticBody)
	public:
		LowHouse();
		~LowHouse();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		void simulate_step(double days); // updates every attribute directly as a function of time (days since beginning of the game)
		int windowCost = 200;
		int windowNumber;
		int solarPanelCost = 1500;

		double houseIncome;
		int numberOfInhabitants;
	
		// policies 
		bool solar_panel(); 
		bool double_glazing(); // improve insulation

		//To test rand() function
		double get_houseIncome();
		double get_numberofInhabitants();

	};

};
