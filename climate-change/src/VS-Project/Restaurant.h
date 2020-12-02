#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include "obj.h"


namespace godot {
	class Restaurant : public Struc, public StaticBody {
		GODOT_CLASS(Restaurant, StaticBody)

	private:

		bool Clickable;

	public:

		Restaurant();
		Restaurant(double income, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply, double healthcare, double needs);

		bool PanelsOn;
		
		Node* GetPanels();

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void _on_Area_mouse_entered();
		void _on_Area_mouse_exited();
	
	};
}