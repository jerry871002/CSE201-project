#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot {

	class Structure : public StaticBody {
		GODOT_CLASS(Structure, StaticBody)
	
	private:

		

	public:

		bool Clickable;

		Structure();
		~Structure();

		bool PanelsOn;
		Node* GetPanels();

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(Input* e);
		void _ready();

		virtual void _on_Area_mouse_entered();
		virtual void _on_Area_mouse_exited();

	};
}
