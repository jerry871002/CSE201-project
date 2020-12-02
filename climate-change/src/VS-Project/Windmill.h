#include "obj.h";
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
namespace godot {
class Windmill : public Energy, public StaticBody {
		GODOT_CLASS(Windmill, StaticBody)
	public:
		Windmill();
		~Windmill();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};
