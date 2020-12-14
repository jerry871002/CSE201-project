#pragma once
#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <KinematicBody.hpp>

namespace godot {
    class Transport : public KinematicBody {
        GODOT_CLASS(Transport, KinematicBody)
    private:
        bool Clickable;
    public:
        /*double numCars;
        double numElectricCars;
        double numBus;
        double numTradddm;*/ //not using it for now
        double occupancyRate, capacity, kmPerDay,passengers, co2PerKm, fuelPerKm; // co2 in kg
        double fuelInput; // in litres
        int transportType; /*
        0 - electic car, 1 - big american car 2 - normal car 
        3 - old collection car 4 - bike 5 -  motorcycle 6 - bus 7 - sports car*/
        Transport(int type);
        Transport();
	    static void _register_methods();
		void _init();
		void _process(float delta);
		void _input(Input* e);
		void _ready();
		void _on_Area_mouse_entered();
		void _on_Area_mouse_exited();
        virtual String class_name();
	    void simulate_step(double days);

        double employment, cost, energyUse, maintenance, CO2Emission, buildingTime, satisfaction, environmentalCost;
        double age;
    };
}   