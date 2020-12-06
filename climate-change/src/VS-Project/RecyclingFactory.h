#include "obj.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
namespace godot {
class RecyclingFactory : public Production, public StaticBody {
		GODOT_CLASS(RecyclingFactory, StaticBody)
	public:
		RecyclingFactory();
		~RecyclingFactory();
		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};