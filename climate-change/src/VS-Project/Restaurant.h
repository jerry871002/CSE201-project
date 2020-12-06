#pragma once
#include "obj.h"
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot {
	class Restaurant : public Production, public StaticBody {
		GODOT_CLASS(Restaurant, StaticBody)
	private:

		bool Clickable;

	public:

		Restaurant();
		//Restaurant(int);

		bool PanelsOn;
		
		Node* GetPanels();

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void _on_Area_mouse_entered();
		void _on_Area_mouse_exited();
		void _on_Checkbox_pressed();
		void _on_Checkbox_button_up();
		void _on_Checkbox_button_down();
		void _on_Checkbox_toggled();


		int income;

		void change_income_by_percent(float);
		


		int restaurantType;
		double diningSize;
		double averageWage;
	};
}