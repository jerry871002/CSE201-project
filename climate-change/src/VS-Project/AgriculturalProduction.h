#pragma once
#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include "obj.h"
#include <StaticBody.hpp>

namespace godot {
	class AgriculturalProduction : public Production {
		GODOT_SUBCLASS(AgriculturalProduction, Production)
	public:
		virtual String class_name();
		AgriculturalProduction();
		~AgriculturalProduction();
		//void test();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
}
