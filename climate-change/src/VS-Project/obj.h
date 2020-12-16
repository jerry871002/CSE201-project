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
#include <Mesh.hpp>
#include <cstdlib>
#include <ctime>
#include <random>

# define M_PI 3.14159265358979323846  /* pi */

namespace godot {
    class Structure : public StaticBody {
        GODOT_CLASS(Structure, StaticBody)
    private:
        bool Clickable;
    public:
        bool MenuVisible;

        static void _register_methods();
        virtual void _init();
        virtual void _process(float delta);
        virtual void _input(InputEvent* e);
        virtual void _ready();
        void _on_Area_mouse_entered();
        void _on_Area_mouse_exited();
        //void _on_CheckBox_pressed();
        //void _on_CheckBox_button_up();
        //void _on_CheckBox_button_down();
        //void _on_CheckBox_toggled();
        virtual String class_name();
        virtual void show_menu();

        virtual String output_information();

        Structure();
        ~Structure();

        Vector3 get_position();

        bool is_other_structure_within_distance(Vector3, double);

        double employment = 0; // approximate number of employees per building
        double cost = 0; // cost in euros to build a new building
        double energyUse = 0; //amount of energy used by the building in kWh
        double maintenance = 0; //maintenace and working cost in euros
        double CO2Emission = 0; // kg of CO2 emitted
        double buildingTime = 0; // years needed to build a new building
        double satisfaction = 0; // on scale of 10
        double environmentalCost = 0; // environmental and health costs in euros 
        double age = 0; //age of each particular object in days

        // The following will be city-wide counters that will be updated every day : 
        // income, population, numberOfEmployees, carbonEmission, energyDemand, energySupply

        // income is the total wage (GDP), population is the population of the whole city, numberOfEmployees the total employed people,  
        // carbonEmission the total CO2, energyDemand the sum of all energy needed and energySupply the maximum production capacity

        // It would be great if someone could write here what variable we have to call to update each
        // income : we didn't take it into account for the moment
        // population : we don't have that, thought you would decide about it
        // numberOfEmployees: employment
        // carbonEmission : CO2Emission
        // energyDemand : that corresponds to energyUse for each class
        // energySupply : that corresponds to energyOutput for each object producing it in the energy class 

        // sim team will program these counters to grab the correct attributes within all structures
        // if needed within any object they will be passed on by the city object
        // don't use these specific variable names inside the structures pls or we will all get confused    

        double totalDays; //total number of days that have passed in the simulation, will be passed on by the City object

        // All of our policies have to go in the City class !! Look at City.h 

        // virtual void simulate_step();
        // Coal power plant (constructor creates subcritical plant of 38% efficiency) :
        bool efficiency_supercritical(); // improve efficiency to supercritical type of plant (42% energy converted to electricity)
        bool efficiency_cogeneration(); // improve efficiency to cogeneration type of plant (47% energy converted to electricity)
        // need to add a cost for their implementation in the maintenance variable once

        virtual double get_maintenance();
        virtual double get_building_time();
        virtual double get_cost();
        virtual double get_employment();
        virtual double get_energyuse();
        virtual double get_co2emissions();
        virtual double get_satisfaction();
        virtual double get_environmentalcost();

    };
}
  