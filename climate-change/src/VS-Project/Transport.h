#pragma once

#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <KinematicBody.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include "City.h"
#include "obj.h"

namespace godot {
    class Transport : public Infrastructure, public KinematicBody {
        GODOT_CLASS(Transport, KinematicBody)
    public:
        Transport(int type);
        Transport();

        // godot functions
        static void _register_methods();
        void _init();
        void _ready();
        void _process(float delta);		
        void _on_Area_mouse_entered();
        void _on_Area_mouse_exited();

        // simulation function
        void simulate_step(double days);

        // graphical functions
        void turn(int dir, float delta);
        void straight(float delta);
        int get_direction(Vector3 pos, double rot);

    private:
        // simulation variables
        double occupancyRate, capacity, kmPerDay;
        double passengers, co2PerKm, fuelPerKm; // CO2 in kg
        double fuelInput; // in litres
        /*
        * 0 - electic car
        * 1 - big american car
        * 2 - normal car 
        * 3 - old collection car 
        * 4 - bike 
        * 5 - motorcycle 
        * 6 - bus 
        * 7 - sports car
        */
        int transportType;

        // graphical variables
        Vector3 motion;
        double position;
        double rot;
        double SPEED_T;
        int Turn_R = 4;
        int dir;
        Vector3 prevPosition = Vector3(0, 0, 0);
        double Acc = 0.5;
        Vector3 center;
        City* myCity;
        int traffic[2][3][4][3] = { {{{0, 1, 0},{0, 0, 1},{0, 0, 1},{0, 0, 1}},		{{0, 0, 1},{0, 1, 0},{0, 0, 1},{0, 0, 1}},		{ {0, 1, 1},{0, 1, 1},{0, 0, 1},{0, 0, 1}}},
                                    { {{0, 0, 1},{0, 0, 1},{0, 0, 1},{0, 1, 0}}	,		{{0, 0, 1},{0, 0, 1},{1, 1, 0},{0, 1, 1}},		 { {0, 0, 1},{0, 0, 1},{0, 1, 1},{0, 0, 1}}} };
    };
}   