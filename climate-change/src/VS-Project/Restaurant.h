#pragma once
#include "obj.h"
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot {
	class Restaurant : public Struc, public StaticBody {
		GODOT_CLASS(Restaurant, StaticBody)

	private:

		bool Clickable;

	public:

		Restaurant();

		bool PanelsOn;
		
		Node* GetPanels();

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(Input* e);
		void _ready();
		void _on_Area_mouse_entered();
		void _on_Area_mouse_exited();


		int income;

		void change_income_by_percent(float);
	};
}