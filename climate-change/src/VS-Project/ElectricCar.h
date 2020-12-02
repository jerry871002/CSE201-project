#include "obj.h"
#include "Transport.h"
#include <core/Godot.hpp>
#pragma once
#include <StaticBody.hpp>
namespace godot {
class ElectricCar : public Transport, public StaticBody {
		GODOT_CLASS(ElectricCar, StaticBody)
	public:
		ElectricCar();
		~ElectricCar();
		void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
	};
};