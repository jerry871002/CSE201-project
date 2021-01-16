#pragma once

#include "obj.h"

#include <core/Godot.hpp>
#include <KinematicBody.hpp>
#include <AnimationPlayer.hpp>
#include "City.h"

namespace godot {
    class Transport : public KinematicBody {
        GODOT_CLASS(Transport, KinematicBody)
    public:
        Transport();
        Transport(int type);
        void transport_type();

        // godot functions
        static void _register_methods();
        void _init();
        void _ready();
        void _physics_process(float delta);

        // simulation function
        void simulate_step(double days);

        // graphical functions
        void turn(int dir, float delta);
        void straight(float delta);
        int get_direction(Vector3 pos, double rot);

        double occupancyRate, capacity, kmPerDay;
        double passengers, co2PerKm, fuelPerKm; // CO2 in kg
        double test;
        double CO2Emission; // co2 output for the whole duration of simulation
        double maintenance; // maintenance cost for the whole duration of simulation
        double fuelInput; // fuel needed for the whole duration of simulation
        double energyUse; //energy needed for the whole duration of simulation
        double age; //age in days
        double employment;
        double cost;
        double buildingTime;
        double satisfaction;
        double get_satisfaction();
		double get_co2emissions();
		double get_energyuse();
		double get_environmentalcost();
        double lifetime; //in years
        double pricePerMonth; // in euros
        double weight; //in tonnes
        double workingDays;//number  of days when cars are not prohibited

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
        void set_transportType(int type);
        int get_transportType();

        /*
        * 0 - electic car
        * 1 - big american car
        * 2 - normal car
        * 3 - old collection car/Bugatti
        * 4 - bike
        * 5 - motorcycle
        * 6 - bus
        * 7 - sports car/Chiron
        */
        int transportType;

    private:
        // simulation variables

        

        // graphical variables
        Vector3 motion;
        double position;
        double rot;
        double SPEED_T;
        int Turn_R = 4;
        int dir;
        double prevPosition = 0;
        Vector3 prevPositionVec = Vector3();
        double Acc = 0.5;
        Vector3 center;

        City* myCity = new City();
        
    };

    class Pedestrian : public KinematicBody {
        GODOT_CLASS(Pedestrian, KinematicBody)
    public:
        Pedestrian();

        // godot functions
        static void _register_methods();
        void _init();
        void _ready();
        void _process(float delta);

    private:
        AnimationPlayer* player = new AnimationPlayer();

        Vector3 motion;
        double position;
        double rot;
        double SPEED_T;
        int Turn_R = 4;
        int dir;
        double prevPosition = 0;
        Vector3 prevPositionVec = Vector3();
        double Acc = 0.5;
        Vector3 center;
        City* myCity = new City();
        void turn(int dir, float delta);
        void straight(float delta);
    };
}
