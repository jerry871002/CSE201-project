#include "Transport.h"
// #include <GodotGlobal.hpp>
#include "random"
// using namespace godot;

/*void Transport::_register_methods(){
	register_method((char*)"_process", &Car::_process);
	register_method((char*)"_input", &Car::_input);
	register_method((char*)"_ready", &Car::_ready);
}

void Transport::_init(){

}

void Transport:_process(float delta){

}

void Transport::_input(InputEvent* e){

}

void Transport::_ready(){

}*/
/*
    0 - electic car, 1 - big american car 2 - normal car 
    3 - old collection car 4 - bike 5 -  motorcycle 6 - bus*/

Transport::Transport(int transportType){
type = transportType;
CO2_output = 0; // co2 output for the whole duration of simulation
maintenance = 0; // maintenance cost for the whole duration of simulation
fuelInput = 0; // fuel needed for the whole duration of simulation
energyuse = 0; //energy needed for the whole duration of simulation
passengers = 0; //total number of passengers that used the car
// random device class instance, source of 'true' randomness for initializing random seed
std::random_device rd; 
std::mt19937 gen(rd()); 
if (type == 1 ){ // big american car
    co2PerKm = 328/1000; 
    fuelPerKm = 14/100; 
    std::normal_distribution <double> costg(85000, 12000);
    cost = costg(gen); // cost of 1 car in euros, randomised using gaussian
    capacity = 8;
    std::normal_distribution <double> kmg(80, 15);
    kmPerDay = kmg(gen); // average km per day for this car using gaussian
    std::normal_distribution <double> occupancyg(0.5, 0.30);
    occupancyRate = occupancyg(gen); // average percentage occupancy of the car: number of people in the car / capacity
    if (occupancyRate >1) {
	    occupancyRate=1;
}
std::normal_distribution <double> timeg(1, 0.5);
building_time = timeg(gen); // building time of 1 car, very fast
std::normal_distribution <double> satisfactiong(1, 0.5);
satisfaction = satisfactiong(gen);
}
}

void Transport::simulate_step(double days){
fuelInput+=fuelPerKm*kmPerDay*days; //litres of fuel for car
CO2_output+=co2PerKm*kmPerDay*days; // co2 emissions per car
passengers+=capacity*occupancyRate*days; //number of people that used the car in given period
if (type==1){
     // no repairs price if car is <3yo
    if (days<=365){
        maintenance += 1.25*days; //maintenance price per day if car is less than 1 yo
    }
    else if (days <=730){
        maintenance += 1.92*days; //maintenance price per day if car is less than 2 yo
    }
    else if (days <= 1095){
        maintenance += 1.95*days; //maintenance price per day if car is less than 3 yo
    }
    if (days<=1460) {
         maintenance+=1.86*days; // repairs price per day is car is less than 4 yo
        maintenance +=5.7*days; //maintenance price per day if car is less than 4 yo
    }
    else{
        maintenance += 2.78*days; //repairs price per day if car is more than 4 yo
        maintenance +=3.21*days; //maintenance price per day if car is more than 4 yo
    }
}
}

Transport::~Transport(){

}