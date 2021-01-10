#include <iostream>
#include "Transport.h"


/*
If on Mac, use this to test in terminal:

g++ -std=c++17 TransportTest.cpp Transport.cpp obj.cpp -ILibraries/godot-cpp-bindings/godot_headers \
-ILibraries/godot-cpp-bindings/include -ILibraries/godot-cpp-bindings/include/core \
-ILibraries/godot-cpp-bindings/include/gen -LLibraries/godot-cpp-bindings/bin \
-lgodot-cpp.osx.debug.64 -o test_Transport

To then run the test if there are no compilation errors:
enter this in terminal:
./test_Transport

*/


using namespace godot;

//Declare All the test functions:
void test_constructor();
void test_simulate(double days);

int main() {
    test_constructor();
    std::cout<<std::endl;
    test_simulate(500);
}

void test_simulate(double days){
std::cout<<"Simulation of "<<days<<" days"<<std::endl;
Transport electricCar = Transport(0);
Transport bigCar = Transport(1);
Transport car = Transport(2);
Transport collectionCar = Transport(3);
Transport bike = Transport(4);
Transport motorcycle = Transport(5);
Transport bus = Transport(6);
Transport sportscar = Transport(7);
electricCar.simulate_step(days);
bigCar.simulate_step(days);
car.simulate_step(days);
collectionCar.simulate_step(days);
bus.simulate_step(days); 
bike.simulate_step(days);
motorcycle.simulate_step(days);
sportscar.simulate_step(days);
std::cout<<"Electric car: co2 emission "<<electricCar.CO2Emission<<", age "<<electricCar.age<<", energy use "<<electricCar.energyUse<< ", fuel input "<<electricCar.fuelInput<<", maintenance "<<electricCar.maintenance<<", passengers "<<electricCar.passengers<<std::endl;
std::cout<<"Big car: co2 emission "<<bigCar.CO2Emission<<", age "<<bigCar.age<< ", fuel input "<<bigCar.fuelInput<<", maintenance "<<bigCar.maintenance<<", passengers "<<bigCar.passengers<<std::endl;
std::cout<<"Car: co2 emission "<<car.CO2Emission<<", age "<<car.age<< ", fuel input "<<car.fuelInput<<", maintenance "<<car.maintenance<<", passengers "<<car.passengers<<std::endl;
std::cout<<"Bus: co2 emission "<<bus.CO2Emission<<", age "<<bus.age<< ", fuel input "<<bus.fuelInput<<", maintenance "<<bus.maintenance<<", passengers "<<bus.passengers<<std::endl;
std::cout<<"Bike: co2 emission "<<bike.CO2Emission<<", age "<<bike.age<< ", fuel input "<<bike.fuelInput<<", maintenance "<<bike.maintenance<<", passengers "<<bike.passengers<<std::endl;
std::cout<<"Collection car: co2 emission "<<collectionCar.CO2Emission<<", age "<<collectionCar.age<< ", fuel input "<<collectionCar.fuelInput<<", maintenance "<<collectionCar.maintenance<<", passengers "<<collectionCar.passengers<<std::endl;
std::cout<<"Motorcycle: co2 emission "<<motorcycle.CO2Emission<<", age "<<motorcycle.age<< ", fuel input "<<motorcycle.fuelInput<<", maintenance "<<motorcycle.maintenance<<", passengers "<<motorcycle.passengers<<std::endl;
std::cout<<"Sports car: co2 emission "<<sportscar.CO2Emission<<", age "<<sportscar.age<< ", fuel input "<<sportscar.fuelInput<<", maintenance "<<sportscar.maintenance<<", passengers "<<sportscar.passengers<<std::endl;
std::cout<<"Simulation works";
}

void test_constructor() {
Transport electricCar = Transport(0);
Transport bigCar = Transport(1);
Transport car = Transport(2);
Transport collectionCar = Transport(3);
Transport bike = Transport(4);
Transport motorcycle = Transport(5);
Transport bus = Transport(6);
Transport sportscar = Transport(7);
    std::cout<<"Electric car: capacity "<<electricCar.capacity<<" , occupancy rate "<<electricCar.occupancyRate<<", cost "<<electricCar.cost<<" ,building time "<<electricCar.buildingTime<<", satisfaction "<<electricCar.satisfaction<<std::endl;
    std::cout<<"Bus: capacity "<<bus.capacity<<" , occupancy rate "<<bus.occupancyRate<<" cost"<<bus.cost<<" ,building time "<<bus.buildingTime<<", employment "<<bus.employment<<", satisfaction "<<bus.satisfaction<<std::endl;
    std::cout<<"Car: capacity "<<car.capacity<<" , occupancy rate "<<car.occupancyRate<<" cost"<<car.cost<<" ,building time "<<car.buildingTime<<", satisfaction "<<car.satisfaction<<std::endl;
    std::cout<<"Big car: capacity "<<bigCar.capacity<<" , occupancy rate "<<bigCar.occupancyRate<<" cost"<<bigCar.cost<<" ,building time "<<bigCar.buildingTime<<", satisfaction "<<bigCar.satisfaction<<std::endl;
    std::cout<<"Bike: capacity "<<bike.capacity<<" , occupancy rate "<<bike.occupancyRate<<" cost"<<bike.cost<<" ,building time "<<bike.buildingTime<<", satisfaction "<<bike.satisfaction<<std::endl;
    std::cout<<"Collection car: capacity "<<collectionCar.capacity<<" , occupancy rate "<<collectionCar.occupancyRate<<" cost"<<collectionCar.cost<<" ,building time "<<collectionCar.buildingTime<<", satisfaction "<<collectionCar.satisfaction<<std::endl;
    std::cout<<"Motorcycle: capacity "<<motorcycle.capacity<<" , occupancy rate "<<motorcycle.occupancyRate<<" cost"<<motorcycle.cost<<" ,building time "<<motorcycle.buildingTime<<", satisfaction "<<motorcycle.satisfaction<<std::endl;
     std::cout<<"Sports car capacity "<<sportscar.capacity<<" , occupancy rate "<<sportscar.occupancyRate<<" cost"<<sportscar.cost<<" ,building time "<<sportscar.buildingTime<<", satisfaction "<<sportscar.satisfaction<<std::endl;
    std::cout << "Constructor Works " << std::endl;
}



