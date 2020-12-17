#pragma once

#include "City.h"
#include "obj.h"

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot {
class Health : public KinematicBody  {
		GODOT_CLASS(Health, KinematicBody)
	public:
		Health();
		~Health();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		double patientsAdmitted, successRate;
 		int hospitalType;
		virtual String class_name();
		double get_satisfaction();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
	};
};