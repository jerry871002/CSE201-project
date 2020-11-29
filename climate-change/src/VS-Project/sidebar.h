#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

namespace godot {
	class Sidebar : public TextureRect {
		GODOT_CLASS(Menu, TextureRect)
	public:
		Sidebar();
		~Sidebar();
		static void _register_methods();
		void _init();
		void _input(Input* e);
		void _ready();
	};
}
