#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

class Mall : public Shop {
	class City : public Object {
		GODOT_CLASS(City, Object)
	public:
		Mall();
		~Mall();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};