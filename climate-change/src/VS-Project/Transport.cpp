#include "Transport.h"
#include "random"
#include <Area.hpp>

using namespace godot;

void Transport::_register_methods(){
    register_method((char*)"_init", &Transport::_init);
	register_method((char*)"_process", &Transport::_process);
	register_method((char*)"_input", &Transport::_input);
	register_method((char*)"_ready", &Transport::_ready);
}

void Transport::_init(){

}

void Transport::_process(float delta){

}

void Transport::_input(Input* e){

}

void Transport::_ready(){

}
/*
    0 - electic car, 1 - big american car 2 - normal car 
    3 - old collection car 4 - bike 5 -  motorcycle 6 - bus 7 - sports car*/

Transport::Transport(int type){
transportType = type;
CO2Emission = 0; // co2 output for the whole duration of simulation
maintenance = 0; // maintenance cost for the whole duration of simulation
fuelInput = 0; // fuel needed for the whole duration of simulation
energyUse = 0; //energy needed for the whole duration of simulation
passengers = 0; //total number of passengers that used the car
age = 0; //age in days
employment = 0; // employees only for the bus
// random device class instance, source of 'true' randomness for initializing random seed
std::random_device rd; 
std::mt19937 gen(rd()); 
switch(transportType) {
    case 0:{ //electric car
        co2PerKm = 0; 
        capacity = 5;
        std::normal_distribution <double> occupancyt(0.4, 0.2);
        occupancyRate = occupancyt(gen); // average percentage occupancy of the car: number of people in the car / capacity
        fuelPerKm = 0;
        std::normal_distribution <double> kmt(70, 15);
        kmPerDay = kmt(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> costt(42000, 8500); //cost randomised using gaussian
        cost = costt(gen);
        std::normal_distribution <double> timet(4, 1);
        buildingTime = timet(gen); // building time of 1 electric car in days, taking tesla model 3
        std::normal_distribution <double> satisfactiont(9.7, 0.2); //very high satisfaction
        satisfaction = satisfactiont(gen);
        if (satisfaction > 10){
            satisfaction = 10;
        }
        break;
    }
    case 1: {  // big american car
        co2PerKm = 0.328; 
        fuelPerKm = 0.24; 
        std::normal_distribution <double> costg(85000, 12000);
        cost = costg(gen); // cost of 1 car in euros, randomised using gaussian
        capacity = 8;
        std::normal_distribution <double> kmg(85, 15);
        kmPerDay = kmg(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> occupancyg(0.375, 0.25);
        occupancyRate = occupancyg(gen); // average percentage occupancy of the car: number of people in the car / capacity
        std::normal_distribution <double> timeg(1, 0.5);
        buildingTime = timeg(gen); // building time of 1 car, very fast
        std::normal_distribution <double> satisfactiong(8.5, 0.4); // high satisfaction
        satisfaction = satisfactiong(gen);
        break;
    }
    case 2:{ //normal family car 
        co2PerKm = 0.115; 
        fuelPerKm = 0.078; 
        std::normal_distribution <double> costf(14000, 3000);
        cost = costf(gen); // cost of 1 car in euros, randomised using gaussian
        capacity = 4;
        std::normal_distribution <double> kmf(50, 10);
        kmPerDay = kmf(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> occupancyf(0.5, 0.25);
        occupancyRate = occupancyf(gen); // average percentage occupancy of the car: number of people in the car / capacity
        std::normal_distribution <double> timeg(1, 0.5);
        buildingTime = timeg(gen); // building time of 1 car, very fast
        std::normal_distribution <double> satisfactiong(6.8, 0.5); // medium satisfaction
        satisfaction = satisfactiong(gen);
        break;
    }
    case 3:{ //old collection car 
        co2PerKm = 0.4; 
        fuelPerKm = 0.22; 
        std::normal_distribution <double> costo(40000, 7000);
        cost = costo(gen); // cost of 1 car in euros, randomised using gaussian
        capacity = 2;
        std::normal_distribution <double> kmo(80, 15);
        kmPerDay = kmo(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> occupancyo(0.75, 0.25);
        occupancyRate = occupancyo(gen); // average percentage occupancy of the car: number of people in the car / capacity
        std::normal_distribution <double> timeo(7, 2);
        buildingTime = timeo(gen); // building time of 1 collection (replica i think) car car, not so fast
        std::normal_distribution <double> satisfactiono(9.3, 0.5); // very high satisfaction
        satisfaction = satisfactiono(gen);
        break;
    }
    case 4:{ //bike
    fuelPerKm = 0;
    co2PerKm = 0;
    std::normal_distribution <double> costbike(370, 30);
    cost = costbike(gen); // cost of 1 bike in euros, randomised using gaussian
    capacity = 1;
    occupancyRate = 1;
    buildingTime = 0.04; //really fast, in days (1 hour )
    std::normal_distribution <double> satisfactionbike(9, 1);
    satisfaction = satisfactionbike(gen); //high satisfaction
    std::normal_distribution <double> kmbike(18, 7);
    kmPerDay = kmbike(gen); // kilometres per day,  randomised for each bike
    break;
    }
    case 5:{ //motorcycle 
        fuelPerKm = 0.044; //in liters
        co2PerKm = 0.11; //in kg
        std::normal_distribution <double> costm(6200, 2000);
        cost = costm(gen); // cost of 1 motorcycle in euros, randomised using gaussian
        capacity = 1;
        occupancyRate = 1;
        buildingTime = 0.12; //really fast, in days (3 hours )
        std::normal_distribution <double> satisfactionm(7, 1);
        satisfaction = satisfactionm(gen); // not very high satisfaction
        std::normal_distribution <double> kmbike(30, 10);
        kmPerDay = kmbike(gen); // kilometres per day,  randomised for each motorcycle
        break;
    }
    case 6:{ // bus
        fuelPerKm = 0.26; //in liters
        co2PerKm = 1.25; //in kg
        std::normal_distribution <double> costb(262500, 52500);
        cost = costb(gen); // cost of 1 bus in euros, randomised using gaussian
        double alpha = (cost-262500)/262500;
        if (alpha<0) {
            alpha=0;
        }
        if (alpha > 1) {
            alpha =1;
        }
        capacity = 30+alpha*30;  
        capacity  = round(capacity); //maximum number of people in 1 bus, positive correlation with cost 
        std::normal_distribution <double> occupancyb(0.70, 0.30);
        occupancyRate = occupancyb(gen); // average percentage occupancy of the bus: number of people in the bus / capacity
        if (occupancyRate>1) {
         occupancyRate=1;
        }
        std::normal_distribution <double> buildingb(22, 3);
        buildingTime = buildingb(gen); // construction time for 1 bus, in days
        std::normal_distribution <double> satisfactionb(8, 1.5); // satisfaction level rather high, randomised
        satisfaction = satisfactionb(gen);
        if (satisfaction >10) {
         satisfaction = 10;
        }
        std::normal_distribution <double> kmb(225, 75);
        kmPerDay = kmb(gen); // kilometres per day,  randomised for each bus
        employment = 1+ round(alpha*2);
        break;
    }
}
}
Transport::Transport(){
    
}
void Transport::simulate_step(double days){
int years = floor((age+days)/365-age/365); 
co2PerKm *=pow(1.05,years); //increase in emissions with each year
age +=days; //total number of days 
fuelInput+=fuelPerKm*kmPerDay*days; //litres of fuel for car
CO2Emission+=co2PerKm*kmPerDay*days; // co2 emissions per car
passengers+=capacity*occupancyRate*days; //number of people that used the car in given period
switch (transportType){
    case 0:{ //electric car
        energyUse+=11.9/100*kmPerDay*days; //energy use of car for the whole duration in kWh
        if (age<=365){
            maintenance += 1.09*days; //maintenance service price per day if car is less than 1 yo
        }
        else if (age <=730){
            maintenance += 1.67*days; //maintenance service price per day if car is less than 2 yo
        }
        else if (age <= 1095){
            maintenance += 1.1*days; //maintenance service price per day if car is less than 3 yo
        }
        if (age<=1460) {
            maintenance+=1.94*days; // maintenance service price per day is car is less than 4 yo
        }
        else{
            maintenance +=2.2*days; // maintenance service price per day if car is more than 4 yo
        }
        break;
    }
    case 1:{ // big american car
     // no repairs price if car is <3yo
        if (age<=365){
            maintenance += 1.25*days; //maintenance price per day if car is less than 1 yo
        }
        else if (age <=730){
            maintenance += 1.92*days; //maintenance price per day if car is less than 2 yo
        }
        else if (age <= 1095){
            maintenance += 1.95*days; //maintenance price per day if car is less than 3 yo
        }
        if (age<=1460) {
            maintenance+=1.86*days; // repairs price per day is car is less than 4 yo
            maintenance +=5.7*days; //maintenance price per day if car is less than 4 yo
        }
        else{
            maintenance += 2.78*days; //repairs price per day if car is more than 4 yo
            maintenance +=3.21*days; //maintenance price per day if car is more than 4 yo
        }
        break;
    }
    case 2:{ // normal family car
     // no repairs price if car is <3yo
        if (age<=365){
            maintenance += 0.15*days; //maintenance price per day if car is less than 1 yo
        }
        else if (age <=730){
            maintenance += 0.5*days; //maintenance price per day if car is less than 2 yo
        }
        else if (age <= 1095){
            maintenance += 1.05*days; //maintenance price per day if car is less than 3 yo
        }
        if (age<=1460) {
            maintenance+=0.765*days; // repairs price per day is car is less than 4 yo
            maintenance +=2.70*days; //maintenance price per day if car is less than 4 yo
        }
        else{
            maintenance += 1.17*days; //repairs price per day if car is more than 4 yo
            maintenance +=2.26*days; //maintenance price per day if car is more than 4 yo
        }
        break;
    }
    case 3:{ // collection car 
     // no repairs price if car is <3yo
        maintenance += 25*days; //maintenance very expensive, does not depend on the age, the car is already old
        break;
    }
    case 4:{ //bike
        maintenance+=0.8*days;

    }
    case 5:{ //motorcycle
        maintenance+=2.6*days;
    }
    case 6:{ //bus
        double alpha = (cost-262500)/262500;
        if (alpha<0) {
         alpha=0;
        }
        maintenance += (0.67+alpha*0.17)*kmPerDay*days; // maintenance cost in euros, positive correlation with bus price
        break;
    }
}
}

