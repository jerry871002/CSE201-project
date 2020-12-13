#pragma once
#include "obj.h"
#include <core/Godot.hpp>
#include <StaticBody.hpp>
namespace godot {
class Windmill : public Energy{
		GODOT_SUBCLASS(Windmill, Energy)
	public:
		Windmill();
		~Windmill();
		
		void _process(float delta);

		//This class gives the functionnalities for one single onshore windmill, 
		//the sim team can use it in order to simulate a wind farm of the size it chooses
		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step)
		double requiredLand; 
		double turnSpeed;
	private:
		double rot = 0;


	};
};
