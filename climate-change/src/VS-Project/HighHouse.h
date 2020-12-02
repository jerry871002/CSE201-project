#include "obj.h";
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

class HighHouse : public Housing {
	class City : public Object {
		GODOT_CLASS(City, Object)
	public:
		HighHouse();
		~HighHouse();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};