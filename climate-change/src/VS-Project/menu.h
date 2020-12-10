#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <TextureButton.hpp>


namespace godot {
	class Menu : public TextureButton{
			GODOT_CLASS(Menu, TextureButton)
	public:

		Menu();
		~Menu();
		bool Sidebar;
		bool CloseButton;

		static void _register_methods();
		void _init();
		void _input(Input* e);
		void _ready();
		void _on_MenuButton_mouse_entered();
		void _on_MenuButton_mouse_exited();
		void _on_MenuButton_pressed();
		void _on_CloseButton_pressed();
	};
}
