#pragma once
#include <core/Godot.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <SceneTree.hpp>


namespace godot {
	class Restau : public SceneTree {
		GODOT_CLASS(Restau, SceneTree)
	private:
		
	public:

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();

		Restau();
		~Restau();

		void object_selected();
	};
}
