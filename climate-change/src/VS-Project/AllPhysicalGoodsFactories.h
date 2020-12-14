#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class AllPhysicalGoodsFactories : public Production {
		GODOT_SUBCLASS(AllPhysicalGoodsFactories, Production)
	public:
		AllPhysicalGoodsFactories();
		~AllPhysicalGoodsFactories();
		static void _register_methods();
		void _init();
		void _process(float);
		void _input(InputEvent*);
		void _ready();
		virtual String class_name();
	};
};