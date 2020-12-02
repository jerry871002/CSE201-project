#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

class LowBuilding : public Housing {
	class City : public Object {
		GODOT_CLASS(City, Object)
	public:
		LowBuilding();
		~LowBuilding();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};