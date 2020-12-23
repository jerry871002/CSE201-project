#pragma once

#include "obj.h"
#include <core/Godot.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>


namespace godot{
    class Shop : public Structure {
        GODOT_SUBCLASS(Shop, Structure)

    private:
        bool Clickable = false;


    public:
        static void _register_methods();

        virtual String class_name();

        String get_class() { return "Shop"; }

        Shop();
        ~Shop();

        
        Node* GetPanels();
        bool PanelsOn{false}; // whether the building has solar panels or not. delfault at false, only possible to set to true for certain classes
        void _ready();

        bool shopStatus = true;   //True if open, False if closed
        double employment = 0;
        
        virtual double get_satisfaction();
        virtual double get_co2emissions();
        virtual double get_energyuse();
        virtual double get_environmentalcost();

        void simulate_step(double days);
        void show_menu();
        virtual String output_information();

        static void _register_method();

        
        // double supposed to represent yearly probability of panel appearing - this will be changed by policies
         
        void panels_get_added(); // function to add the panels, both in simulation and visually
        //unsigned int *panels_age; // int to track the panels_age once they are installed - ticks down every day, when it reaches 0 the panels dissapear
        int panels_age;
        bool ArePanelsDisplayed(); // method just to check whether the panels are on visually (mostly for debug)
        double panel_probability;  // will likely be changed to a more complex function soon
    };



    class Restaurant : public Shop{
            GODOT_SUBCLASS(Restaurant, Shop)
        private:

        public:
            virtual String class_name();

            

            Restaurant();
            ~Restaurant();
            
            int restaurantType = 1;
            double energyUsePerSize = 0;
            double diningSize = 0;
            double averageWage = 0;
            bool firstYearShock = false;

            double get_energyuse();
            void initialize();
            void simulate_step(double days);
    };

    //####################################   Small Shop   ################################

    class SmallShop : public Shop {
		GODOT_SUBCLASS(SmallShop, Shop)
	public:
        virtual String class_name();
        SmallShop();
        ~SmallShop();

    };

    //####################################   Mall   ################################

    class Mall : public Shop {
		GODOT_SUBCLASS(Mall, Shop)
	public:
        virtual String class_name();
        Mall();
        ~Mall();
    };

}