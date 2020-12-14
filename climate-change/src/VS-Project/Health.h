#pragma once

#include "City.h"
#include "obj.h"

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot {
class Health : public Infrastructure, public StaticBody {
		GODOT_CLASS(Health, StaticBody)
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

	};
};