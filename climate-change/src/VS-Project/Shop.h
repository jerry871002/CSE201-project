#pragma once

#include "obj.h"

#include <core/Godot.hpp>
#include <StaticBody.hpp>
#include <MeshInstance.hpp>
#include <Input.hpp>
#include <InputEventMouse.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>


namespace godot{
    class Shop : public Structure {
        GODOT_SUBCLASS(Shop, Structure)

    public:
        Shop();
        ~Shop();

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

            bool Clickable;

        public:

            Restaurant();
            ~Restaurant();
            
            int restaurantType = 0;
            double energyUsePerSize = 0;
            double diningSize = 0;
            double averageWage = 0;
            bool firstYearShock = false;

            double get_satisfaction();
            double get_co2emissions();
            double get_energyuse();
            double get_environmentalcost();

            void simulate_step(double days);
    };

    class SmallShop : public Shop {
		GODOT_SUBCLASS(SmallShop, Shop)
	public:
        SmallShop();
        ~SmallShop();

    };

    class Mall : public Shop {
		GODOT_SUBCLASS(Mall, Shop)
	public:
        Mall();
        ~Mall();
    };

}