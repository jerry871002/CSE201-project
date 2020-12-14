#pragma once

#include "City.h"
#include "obj.h"

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

namespace godot {
    class Transport : public KinematicBody {
        GODOT_CLASS(Transport, KinematicBody)
    public:

        Transport();

        void transport_type(int type);

        // godot functions
        static void _register_methods();
        void _init();
        void _ready();
        void _process(float delta);

        // simulation function
        void simulate_step(double days);

        // graphical functions
        void turn(int dir, float delta);
        void straight(float delta);
        int get_direction(Vector3 pos, double rot);

        double occupancyRate, capacity, kmPerDay;
        double passengers, co2PerKm, fuelPerKm; // CO2 in kg
        double fuelInput; // in litres
        double test = 6;
        double CO2Emission = 0; // co2 output for the whole duration of simulation
        double maintenance = 0; // maintenance cost for the whole duration of simulation
        double fuelInput = 0; // fuel needed for the whole duration of simulation
        double energyUse = 0; //energy needed for the whole duration of simulation
        double passengers = 0; //total number of passengers that used the car
        double age = 0; //age in days
        double employment = 0;
        double cost = 0;
        double buildingTime = 0;
        double satisfaction = 0;

    private:
        // simulation variables

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