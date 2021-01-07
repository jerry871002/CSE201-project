#include <iostream>
#include "Housing.h"
//This file is just to catch compiling errors for those working on Mac

/*
If on Mac, use this to test in terminal:

g++ -std=c++17 HousingTest.cpp Housing.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers \
-ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core \
-ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin \
-lgodot-cpp.osx.debug.64 -o test_Housing

To then run the test if there are no compilation errors:
enter this in terminal:
./test_Housing
*/

using namespace godot;

//Declare All the test functions:


//Test Functions For the House:
void test_house_constructor();
void test_house_simulate_step();
void test_house_get_attributes();

//Test Functions For the Building:
void test_building_constructor();
void test_building_simulate_step();
void test_house_get_attributes();



int main() {
    // Test functions:

    //Checking through the House:
    test_house_constructor();
    test_house_simulate_step();

    //Checking through the Building:
    test_building_constructor();
    test_building_simulate_step();


    std::cout << "EveryThing in Housing works fine ;)" << std::endl;
}






void test_house_constructor() {
    try {
        House A = House();
        House B = House();
        //Here can add code to check attributes of constructor
        int type1 = 0;
        int type2 = 0;
        for (int i = 0; i < 100; i++){
            House C = House();
            if (C.houseType == 1){type1 +=1;}
            if (C.houseType == 2){type2 +=1;}
            
        }
        //std::cout << "Number of type 1, type 2, type 3: " << type1 << ", " << type2 << ", " << type3 << std::endl;
    } catch (...){
        std::cout << "Error with the Houses's Constructor" << std::endl;
    }

    // std::cout << "Constructor Works " << std::endl;
}

void test_house_simulate_step(){
    House A = House();
    try{
        for (int i = 0; i<5; i++) {
            A.simulate_step(100);
            //std::cout << "House age: " << A.age << std::endl;
            
        }
    } catch (...) {
        std::cout << "Problem with house's simulate step" << std::endl;
    }

    

}

void test_house_get_attributes(){
    House A = House();
    try{
        double a = A.get_satisfaction();
        double b = A.get_co2emissions();
        double c = A.get_environmentalcost();
        double d = A.get_energyuse();


        //std::cout << "Restaurant type: " << A.restaurantType << ", Energy use is: " << A.get_energyuse() << std::endl;
        //std::cout << "Restaurant type: " << A.restaurantType << ", CO2emmisions is: " << A.get_co2emissions() << std::endl;
    } catch (...){
        std::cout << "Error with one of the house's get_.... functions" << std::endl;
    }
}

void test_building_constructor() {
    try {
        Building A = Building();
        Building B = Building();
        //Here can add code to check attributes of constructor
        int type1 = 0;
        int type2 = 0;
        for (int i = 0; i < 100; i++){
            Building C = Building();
            if (C.buildingType == 1){type1 +=1;}
            if (C.buildingType == 2){type2 +=1;}
            
        }
        //std::cout << "Number of type 1, type 2, type 3: " << type1 << ", " << type2 << ", " << type3 << std::endl;
    } catch (...){
        std::cout << "Error with the Houses's Constructor" << std::endl;
    }

    // std::cout << "Constructor Works " << std::endl;
}

void test_building_simulate_step(){
    Building A = Building();
    try{
        for (int i = 0; i<5; i++) {
            A.simulate_step(100);
            //std::cout << "Building age: " << A.age << std::endl;
            
        }
    } catch (...) {
        std::cout << "Problem with house's simulate step" << std::endl;
    }

    

}

void test_building_get_attributes(){
    Building A = Building();
    try{
        double a = A.get_satisfaction();
        double b = A.get_co2emissions();
        double c = A.get_environmentalcost();
        double d = A.get_energyuse();


        //std::cout << "Restaurant type: " << A.restaurantType << ", Energy use is: " << A.get_energyuse() << std::endl;
        //std::cout << "Restaurant type: " << A.restaurantType << ", CO2emmisions is: " << A.get_co2emissions() << std::endl;
    } catch (...){
        std::cout << "Error with one of the building's get_.... functions" << std::endl;
    }
}



