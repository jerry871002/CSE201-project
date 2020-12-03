#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
namespace godot {
class SmallShop : public Shop, public StaticBody {
		GODOT_CLASS(SmallShop, StaticBody)
	public:
		SmallShop();
		~SmallShop();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};