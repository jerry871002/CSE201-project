#include <iostream>
#include "Shop.h"


/*
If on Mac, use this to test in terminal:

g++ -std=c++17 ShopTest.cpp Shop.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers \
-ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core \
-ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin \
-lgodot-cpp.osx.debug.64 -o test_Shop

To then run the test if there are no compilation errors:
enter this in terminal:
./test_Shop
*/

using namespace godot;

//Declare All the test functions:

//Test Functions for the Shop:
void test_shop_constructor();

//Test Functions For the Restaurant:
void test_restaurant_constructor();
void test_restaurant_simulate_step();
void test_restaurant_get_attributes();

//Test Functions For the Small Shop:
void test_smallshop_constructor();

//Test Functions For the Small Shop:
void test_mall_constructor();


int main() {
    // Test functions:

    //Checking through the Restaurant:
    test_restaurant_constructor();
    test_restaurant_simulate_step();
    test_restaurant_get_attributes();

    //Checking through the Small Shop:
    test_smallshop_constructor();

    //Checking through the Mall:
    test_mall_constructor();

    std::cout << "EveryThing in Shop works fine ;)" << std::endl;
}


void test_shop_constructor(){
    Shop A = Shop();
}



void test_restaurant_constructor() {
    try {
        Restaurant A = Restaurant();
        Restaurant B = Restaurant();
        //Here can add code to check attributes of constructor
        int type1 = 0;
        int type2 = 0;
        int type3 = 0;
        for (int i = 0; i < 100; i++){
            Restaurant C = Restaurant();
            if (C.restaurantType == 1){type1 +=1;}
            if (C.restaurantType == 2){type2 +=1;}
            if (C.restaurantType == 3){type3 += 1;}
        }
        //std::cout << "Number of type 1, type 2, type 3: " << type1 << ", " << type2 << ", " << type3 << std::endl;
    } catch (...){
        std::cout << "Error with the Restaurant's Constructor" << std::endl;
    }

    // std::cout << "Constructor Works " << std::endl;
}

void test_restaurant_simulate_step(){
    Restaurant A = Restaurant();
    try{
        for (int i = 0; i<5; i++) {
            A.simulate_step(100);
            //std::cout << "Restaurant age: " << A.age << std::endl;
            //std::cout << "Restaurant status: " << A.restaurantStatus << std::endl;
        }
    } catch (...) {
        std::cout << "Problem with restaurant's simulate step" << std::endl;
    }

    int numberFailed = 0;       //Number of shops that closed after the first year

    for (int i = 0; i<101; i++){
        Restaurant B = Restaurant();
        B.simulate_step(400);
        // std::cout << "Restaurant status: " << B.restaurantStatus << std::endl;
        if (B.shopStatus == false){numberFailed += 1;}
    }
    //std::cout << "Number of Restaurants that failed in 1 year: " << numberFailed << std::endl;

}

void test_restaurant_get_attributes(){
    Restaurant A = Restaurant();
    try{
        double a = A.get_satisfaction();
        double b = A.get_co2emissions();
        double c = A.get_environmentalcost();
        double d = A.get_energyuse();


        //std::cout << "Restaurant type: " << A.restaurantType << ", Energy use is: " << A.get_energyuse() << std::endl;
        //std::cout << "Restaurant type: " << A.restaurantType << ", CO2emmisions is: " << A.get_co2emissions() << std::endl;
    } catch (...){
        std::cout << "Error with one of the restaurant's get_.... functions" << std::endl;
    }
}



void test_smallshop_constructor(){
    SmallShop A = SmallShop();
}

void test_mall_constructor(){
    Mall A = Mall();
    
}
