#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class Mall : public Shop, public StaticBody {
		GODOT_CLASS(Mall, StaticBody)
	public:
		Mall();
		~Mall();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};