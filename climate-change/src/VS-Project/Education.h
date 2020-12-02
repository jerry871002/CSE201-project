#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

namespace godot {
class Education : public Infrastructure, public StaticBody {
		GODOT_CLASS(Education, StaticBody)
	public:
		Education();
		~Education();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};