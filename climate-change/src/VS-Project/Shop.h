#pragma once

#include "obj.h"
#include <core/Godot.hpp>



namespace godot{
    class Shop : public Structure {
        GODOT_SUBCLASS(Shop, Structure)

    public:
        virtual String class_name();
        Shop();
        ~Shop();

        Node* GetPanels();
        bool PanelsOn; // whether the building has solar panels or not. delfault at false, only possible to set to true for certain classes


        bool shopStatus = true;   //True if open, False if closed
        double employment = 0;
        
        virtual double get_satisfaction();
        virtual double get_co2emissions();
        virtual double get_energyuse();
        virtual double get_environmentalcost();

        void simulate_step(double days);

    };



    class Restaurant : public Shop{
            GODOT_SUBCLASS(Restaurant, Shop)
        private:

            bool Clickable = false;

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