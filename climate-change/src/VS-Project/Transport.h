#include "obj.h"
#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>

namespace godot {
    class Transport : public Infrastructure, public StaticBody {
        GODOT_CLASS(Transport, StaticBody)
    private:
        bool Clickable;
    public:
        double numCars;
        double numElectricCars;
        double numBus;
        double numTram;
        double occupancyRate, capacity, kmPerDay,passengers, co2PerKm, fuelPerKm; // co2 in kg
        double fuelInput; // in litres
        int transportType; /*
        0 - electic car, 1 - big american car 2 - normal car 
        3 - old collection car 4 - bike 5 -  motorcycle 6 - bus*/
        Transport(int type);
        Transport();
	    static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(Input* e);
		void _ready();
		void _on_Area_mouse_entered();
		void _on_Area_mouse_exited();
	    void simulate_step(double days);
        int age; // age of this particular transport (in days)
    };
}   