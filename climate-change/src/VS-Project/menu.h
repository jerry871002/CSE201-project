
#pragma once

#include <core/Godot.hpp>
#include <TextureRect.hpp>
#include "Root.h"

namespace godot {
	class Menu : public TextureRect {
		GODOT_CLASS(Menu, TextureRect)
	public:
		Menu();
		~Menu();

		static void _register_methods();
		void _init();

		void _on_TextureButton_pressed(Variant body);
	};
}
