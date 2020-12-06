#pragma once
#include "Structure.h"
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot {
	class Restaurant : public Structure {
		GODOT_SUBCLASS(Restaurant, Structure)
	private:

	public:

		Restaurant();
		~Restaurant();

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(Input* e);
		void _ready();

		int income;

		void change_income_by_percent(float);
		
	};
}