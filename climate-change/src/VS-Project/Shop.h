#pragma once

#include "obj.h"
#include <core/Godot.hpp>



namespace godot{
    class Shop : public Structure {
        GODOT_SUBCLASS(Shop, Structure)

    public:
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

        void panels_get_added();
        int panels_age{ 0 };
        bool ArePanelsDisplayed();
        double panel_probability{ 75 };
        
    };



    class Restaurant : public Shop{
            GODOT_SUBCLASS(Restaurant, Shop)
        private:

            bool Clickable = false;

        public:

            Restaurant();
            ~Restaurant();

            String class_name();
            
            int restaurantType = 1;
            double energyUsePerSize = 0;
            double diningSize = 0;
            double averageWage = 0;
            bool firstYearShock = false;

            double get_energyuse();

            void simulate_step(double days);
    };

    //####################################   Small Shop   ################################

    class SmallShop : public Shop {
		GODOT_SUBCLASS(SmallShop, Shop)
	public:
        SmallShop();
        ~SmallShop();

    };

    //####################################   Mall   ################################

    class Mall : public Shop {
		GODOT_SUBCLASS(Mall, Shop)
	public:
        Mall();
        ~Mall();
    };

}