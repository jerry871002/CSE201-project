#include "obj.h"
#include "Transport.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>

class Tram : public Transport {
	class City : public Object {
		GODOT_CLASS(City, Object)
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