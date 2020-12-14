#pragma once
#include "obj.h"
#include <core/Godot.hpp>
#include <StaticBody.hpp>
namespace godot {
class Windmill : public Energy{
		GODOT_SUBCLASS(Windmill, Energy)
	public:
		virtual String class_name();
		Windmill();
		~Windmill();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		//This class gives the functionnalities for one single onshore windmill, 
		//the sim team can use it in order to simulate a wind farm of the size it chooses
		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double requiredLand; 
		double turnSpeed;
	private:
		double rot = 0;


	};
};
