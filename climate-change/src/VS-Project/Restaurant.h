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
#include <String.hpp>


namespace godot {
	class Restaurant : public Shop{
		GODOT_SUBCLASS(Restaurant, Shop)
	private:

		bool Clickable;

	public:

		Restaurant();
		//Restaurant(double income, double numberOfEmployees, double carbonEmission, double energyDemand, double energySupply, double healthcare, double needs);

		bool PanelsOn;
		bool MenuVisible;
		
		Node* GetPanels();

		static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(InputEvent* e);
		void _ready();
		void _on_Area_mouse_entered();
		void _on_Area_mouse_exited();
		void _on_CheckBox_pressed();
		void _on_CheckBox_button_up();
		void _on_CheckBox_button_down();
		void _on_CheckBox_toggled();
		String class_name();


		int income;

		void change_income_by_percent(float);
		

		bool restaurantStatus;    //True if open, False if closed
		int restaurantType;
		double energyUsePerSize;
		double diningSize;
		double averageWage;
		bool firstYearShock;
		void simulate_step(double days);

		double get_satisfaction();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
	};
}