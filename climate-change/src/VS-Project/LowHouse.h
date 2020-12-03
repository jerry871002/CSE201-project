#include "obj.h"
#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

// #include <Math.hpp>
// #include <GodotGlobal.hpp>

namespace godot {
 
	//all these numbers are for a house of 100m^2
	class LowHouse : public Housing , public StaticBody {
        GODOT_CLASS(LowHouse, StaticBody)
	public:
		LowHouse();
		~LowHouse();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void simulate_step(double days); // updates every attribute directly as a function of time (days since beginning of the game)
		double energyUse;
		int window_cost = 200;
		int window_number;
		int solar_panel_cost = 1500;
		


		bool solar_panel(); // adds solar panels
		bool double_glazing(); // improve insulation
		
	};

}