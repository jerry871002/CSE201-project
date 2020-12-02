#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

class AllPhysicalGoodsFactories : public Production {
	class City : public Object {
		GODOT_CLASS(City, Object)
	public:
		AllPhysicalGoodsFactories();
		~AllPhysicalGoodsFactories();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};