#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

class SmallShop : public Shop {
	class City : public Object {
		GODOT_CLASS(City, Object)
	public:
		SmallShop();
		~SmallShop();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};