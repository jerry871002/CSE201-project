#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <menu.h>

namespace godot {
	class Sidebar {

	public:
		Sidebar();
		~Sidebar();
		static void _register_methods();
		void _init();
		void _input(Input* e);
		void _ready();
		void set_item_text ( int nb, String string);
	};
}
