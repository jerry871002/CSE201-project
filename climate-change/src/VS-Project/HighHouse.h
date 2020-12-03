#include "obj.h"
#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot {
 
class HighHouse : public Housing {
public:
	HighHouse();
	~HighHouse();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();

	void simulate_step(double days); // updates every attribute directly as a function of time (days since beginning of the game)
    double energyUse;
    double window_cost;
};

}