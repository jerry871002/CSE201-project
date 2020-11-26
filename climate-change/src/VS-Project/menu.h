#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot {
	class Menu : public StaticBody {
		GODOT_CLASS(Menu, StaticBody)

	private:

		bool Clickable;

	public:

		Menu();
 
		bool Sidebar;
		
		Node* getsidebar();

		static void _register_methods();
		void _init();
		void _input(Input* e);
		void _ready();
		void _on_MenuButton_mouse_entered();
		void _on_MenuButton_mouse_exited();

	};
