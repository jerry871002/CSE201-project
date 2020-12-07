#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <sidebar.h>



namespace godot {
	class Menu {

	public:

		Menu();
		~Menu();
		bool Sidebar;
		

		static void _register_methods();
		void _init();
		void _input(Input* e);
		void _ready();
		void _on_MenuButton_mouse_entered();
		void _on_MenuButton_mouse_exited();

	};
