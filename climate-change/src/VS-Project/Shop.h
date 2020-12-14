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

        int shopType;       //Tells us what type of the object is
        bool shopStatus;   //True if open, False if closed
        double employment;

        // Will have type: 0 - Empty. 1 - Restaurant. 2 - Small Shop. 3 - Mall
        void initialize(int shopTypeInput);
        
        double get_satisfaction();
        double get_co2emissions();
        double get_energyuse();
        double get_environmentalcost();




        void simulate_step(double days);
    };

    class Restaurant : public Shop {
        GODOT_SUBCLASS(Restaurant, Shop)
    public:
        Restaurant();
        ~Restaurant();

        //Attributes used for restaurant:
        int restaurantType;
        double energyUsePerSize;
        double diningSize;
        double averageWage;
        bool firstYearShock = false;
    };
}
