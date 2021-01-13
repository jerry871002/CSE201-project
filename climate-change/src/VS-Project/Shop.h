#pragma once

#include "obj.h"
#include <core/Godot.hpp>



namespace godot {

    class Shop : public Structure {
        GODOT_SUBCLASS(Shop, Structure)

    public:

        Shop();
        ~Shop();

        static void _register_methods();
        virtual void _ready();
        virtual void _process(float delta);
        
        virtual String get_object_type() { return "Shop"; };
        String get_main_type() { return "Shop"; };

        bool PanelsOn; // whether the building has solar panels or not. delfault at false, only possible to set to true for certain classes
        bool WindTurbineOn;  // wether the building has a tooftop wind turbine or not

        bool shopStatus = true;   //True if open, False if closed
        double employment = 0;
        int shopType = 0; //1 if restaurant, 2 if small shop, 3 if mall

        virtual double get_satisfaction();
        virtual double get_co2emissions();
        virtual double get_energyuse();
        virtual double get_environmentalcost();

        virtual void simulate_step(double days);
        virtual String get_object_info();

        void panels_get_added(); // function to add the panels, both in simulation and visually
        int panels_age{ 0 }; // int to track the panels_age once they are installed - ticks down every day, when it reaches 0 the panels dissapear
        double panel_probability{ 0.0 };  // double supposed to represent yearly probability of panel appearing - this will be changed by policies
        
        void windTurbine_get_added();
        int windTurbineAge{ 0 };
        double windTurbine_probability { 0.25 };

        virtual void panel_added_probability();  //Function to determine the probablility of adding a solar panel
        virtual void windTurbine_added_probability();  //Function to determine the probablility of adding a wind turbine 

    private:
        double turnSpeed = 0.5;
        double rot = 0;
    };



    class Restaurant : public Shop {
        GODOT_SUBCLASS(Restaurant, Shop)
    private:

    public:

        Restaurant();
        ~Restaurant();
        virtual String get_object_type() { return "Restaurant"; };


        int restaurantType = 1;
        double energyUsePerSize = 0;
        double diningSize = 0;
        double averageWage = 0;
        bool firstYearShock = false;

        double get_energyuse();

        virtual void simulate_step(double days);

        //void panel_added_probability(double panelCost, double unemployment, double setUpCost, double solarSatisfaction);
    };

    //####################################   Small Shop   ################################

    class SmallShop : public Shop {
        GODOT_SUBCLASS(SmallShop, Shop)

    public:

        SmallShop();
        ~SmallShop();
        virtual String get_object_type() { return "SmallShop"; };

        int smallShopType;
        double averageWage;

        virtual void simulate_step(double days);
    };

    //####################################   Mall   ################################

    class Mall : public Shop {
        GODOT_SUBCLASS(Mall, Shop)
    public:

        Mall();
        ~Mall();
        virtual String get_object_type() { return "Mall"; };

        virtual void simulate_step(double days);

    };

}