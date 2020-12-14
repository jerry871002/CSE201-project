#include "obj.h"
#include <core/Godot.hpp>
#pragma once

namespace godot {
class RecyclingFactory : public Production{
		GODOT_SUBCLASS(RecyclingFactory, Production)
	public:
		RecyclingFactory();
		~RecyclingFactory();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		virtual String class_name();
	};
};