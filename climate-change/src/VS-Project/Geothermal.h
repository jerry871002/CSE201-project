#pragma once
#include "obj.h"
#include <core/Godot.hpp>
#include <StaticBody.hpp>
namespace godot {
class Geothermal : public Energy{
		GODOT_SUBCLASS(Geothermal, Energy)
	public:
		virtual String class_name();
		Geothermal();
		~Geothermal();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		 
		//Many different geothermal types of plants exist and mostly depend on their geological placement. Since we cannot 
		//assume that the city in our simulation is positioned in a favorable geothermal spot, we will consider it to be on an 
		//average geothermal possibilities' location. We thus model a geothermal power plant which uses EGS for profound geothermal 
		//power, which could possibly be done basically anywhere around the world. We base our model especially on 1,7MW the power plant
		//of Soultz-sous-forêts (Alsace), with a 5km deep well.

		//Important sismic risk to be taken into account 

		void simulate_step(double days); //updates attribute by adding to their previous values as a function of time (days since last step) 
		double H2SEmission;
		double CH4Emission;
		double NH3Emission;
	};
};
