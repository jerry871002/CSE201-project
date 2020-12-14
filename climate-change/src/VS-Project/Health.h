#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
namespace godot {
class Health : public Production {
		GODOT_SUBCLASS(Health, Production)
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
	};
};