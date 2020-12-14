#include <iostream>
#include "Restaurant.h"


/*
If on Mac, use this to test in terminal:

g++ -std=c++17 RestaurantTest.cpp Restaurant.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers \
-ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core \
-ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin \
-lgodot-cpp.osx.debug.64 -o test_Restaurant

To then run the test if there are no compilation errors:
enter this in terminal:
./test_Restaurant

*/


using namespace godot;

//Declare All the test functions:
void test_constructor();
void test_simulate_step();
void test_get_satisfaction();
void test_get_co2emissions();
void test_get_energyuse();


int main() {
    test_constructor();
    // test_simulate_step();
    test_get_satisfaction();
    test_get_co2emissions();
    test_get_energyuse();
}


void test_constructor() {
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
    std::cout << "Number of type 1, type 2, type 3: " << type1 << ", " << type2 << ", " << type3 << std::endl;
    std::cout << "Constructor Works " << std::endl;
}

void test_simulate_step(){
    Restaurant A = Restaurant();

    for (int i = 0; i<5; i++) {
        A.simulate_step(100);
        //std::cout << "Restaurant age: " << A.age << std::endl;
        //std::cout << "Restaurant status: " << A.restaurantStatus << std::endl;
    }

    int numberFailed = 0;

    for (int i = 0; i<101; i++){
        Restaurant B = Restaurant();
        B.simulate_step(400);
        // std::cout << "Restaurant status: " << B.restaurantStatus << std::endl;
        if (B.restaurantStatus == false){numberFailed += 1;}
    }
    std::cout << "Number of Restaurants that failed in 1 year: " << numberFailed << std::endl;

}

void test_get_satisfaction(){
    Restaurant A = Restaurant();
    std::cout << "Restaurant type: " << A.restaurantType << ", Satisfaction is: " << A.get_satisfaction() << std::endl;
}

void test_get_co2emissions(){
    Restaurant A = Restaurant();
    std::cout << "Restaurant type: " << A.restaurantType << ", CO2emmisions is: " << A.get_co2emissions() << std::endl;
}

void test_get_energyuse(){
    Restaurant A = Restaurant();
    std::cout << "Restaurant type: " << A.restaurantType << ", Energy use is: " << A.get_energyuse() << std::endl;
}
