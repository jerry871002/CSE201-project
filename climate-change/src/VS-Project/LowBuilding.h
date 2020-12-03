#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class LowBuilding : public Housing, public StaticBody {
		GODOT_CLASS(LowBuilding, StaticBody)
	public:
		LowBuilding();
		~LowBuilding();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};