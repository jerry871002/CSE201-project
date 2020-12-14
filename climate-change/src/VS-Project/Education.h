#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class Education : public Production {
		GODOT_SUBCLASS(Education, Production)
	public:
		Education();
		~Education();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		virtual String class_name();
	};
};