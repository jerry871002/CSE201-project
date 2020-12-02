#pragma once
#include <stdio.h>
#include <core/Godot.hpp>
#include <Node.hpp>


namespace godot {
	class City : public Node {

		GODOT_CLASS(City, Node);

	private:
		

	public:

		// STRUCTURES
		
		// array of all pointers to all structures in city

		// POLICIES
		
		// all policies listed here, as booleans or values, with descriptive names

		// GODOT FUNCTIONS

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _physics_process(float delta);
		void _input(InputEvent* e);
		void _ready();

		City();
		~City();

	};
}
