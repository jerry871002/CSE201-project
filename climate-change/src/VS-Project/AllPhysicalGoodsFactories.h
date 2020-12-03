#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class AllPhysicalGoodsFactories : public Production, public StaticBody {
		GODOT_CLASS(AllPhysicalGoodsFactories, StaticBody)
	public:
		AllPhysicalGoodsFactories();
		~AllPhysicalGoodsFactories();
		void _register_methods();
		void _init();
		void _process(float);
		void _input(InputEvent*);
		void _ready();
	};
};