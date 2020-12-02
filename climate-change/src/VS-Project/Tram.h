#include "obj.h"
#include "Transport.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
namespace godot {
class Tram : public Transport, public StaticBody {
		GODOT_CLASS(Tram, StaticBody)
	public:
		Tram();
		~Tram();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};