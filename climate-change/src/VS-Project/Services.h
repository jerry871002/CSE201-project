#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
namespace godot {
class Services : public Production, public StaticBody {
		GODOT_CLASS(Services, StaticBody)
	public:
		virtual String class_name();
		Services();
		~Services();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};