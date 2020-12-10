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
<<<<<<< Updated upstream
=======
void test_simulate_step();
void test_get_satisfaction();
>>>>>>> Stashed changes


int main() {
    test_constructor();
<<<<<<< Updated upstream
=======
    // test_simulate_step();
    test_get_satisfaction();
>>>>>>> Stashed changes
}


void test_constructor() {
    Restaurant A = Restaurant();
    Restaurant B = Restaurant();
    
    //Here can add code to check attributes of constructor

    std::cout << "Constructor Works " << std::endl;
}

<<<<<<< Updated upstream
=======
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
>>>>>>> Stashed changes
