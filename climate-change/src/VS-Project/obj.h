#pragma once

#include <core/Godot.hpp>
#include <Spatial.hpp>

#include <MeshInstance.hpp>

#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot {

	class Structure :public Spatial {

		GODOT_CLASS(Structure, Spatial)

	private:

		bool Clickable;

	public:

		Structure();
		~Structure();

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

		virtual String get_class_name();


		double cost, energyUse, maintenance, CO2Emission, buildingTime, satisfaction, environmentalCost;
		double age = 0; //age of each particular object in days, initialize to 0 in constructor  

		double totalDays; //total number of days that have passed in the simulation, will be passed on by the City object

		bool PanelsOn; // whether the building has solar panels or not. delfault at false, only possible to set to true for certain classes

		bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
		bool efficiency_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)

		virtual void simulate_step(double days);
	};




	class Production : public Structure {
		GODOT_SUBCLASS(Production, Structure)
	public:
		Production();
		~Production();
		double efficiency;
		double employment;
	};

	class Energy : public Production {
		GODOT_SUBCLASS(Energy, Production)
	public:
		Energy();
		~Energy();
		double energyOutput;
		double energyPerDay;
	};

	class Housing : public Structure {
		GODOT_SUBCLASS(Housing, Structure)
	public:
		Housing();
		~Housing();
	};


	class Infrastructure : public Structure {
		GODOT_SUBCLASS(Infrastructure, Structure)
	public:
		Infrastructure();
		~Infrastructure();
		double employment;
	};

	class Shop : public Production {
		GODOT_SUBCLASS(Shop, Production)
	public:
		Shop();
		~Shop();
		bool open;
	};


	class Solar : public Energy {
		GODOT_SUBCLASS(Solar, Energy)
	protected:
		double energyOutput;
	public:
		Solar();
		Solar(double energyOutput);
		~Solar();
		double environmental_impact(); //redefined don't use same as the one in energy

	};

}