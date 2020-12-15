#pragma once

#include "City.h"
#include "obj.h"

#include <core/Godot.hpp>
#include <KinematicBody.hpp>

// use forward declarations to get around the circular dependencies problem
namespace godot {
	class City;
};

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
        double test ;
        double CO2Emission ; // co2 output for the whole duration of simulation
        double maintenance ; // maintenance cost for the whole duration of simulation
        double fuelInput ; // fuel needed for the whole duration of simulation
        double energyUse ; //energy needed for the whole duration of simulation
        double age ; //age in days
        double employment ;
        double cost ;
        double buildingTime ;
        double satisfaction ;

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