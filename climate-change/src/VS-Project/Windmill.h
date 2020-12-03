#pragma once
#include "obj.h";
#include <core/Godot.hpp>
#include <StaticBody.hpp>
namespace godot {
class Windmill : public Energy, public StaticBody {
		GODOT_CLASS(Windmill, StaticBody)
	public:
		Windmill();
		~Windmill();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double environmentalCost;
	};
};
