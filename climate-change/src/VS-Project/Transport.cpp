#include "Transport.h"

#include <SceneTree.hpp>
#include <Viewport.hpp>
#include <KinematicCollision.hpp>
#include <Mesh.hpp>
#include <Timer.hpp>

#include <random>

# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;
using namespace std;

void Transport::_register_methods(){
    register_method((char*)"_init", &Transport::_init);
	register_method((char*)"_process", &Transport::_process);
	register_method((char*)"_input", &Transport::_input);
	register_method((char*)"_ready", &Transport::_ready);
}

void ComputeAcceleration(double& Acc, double Speed0, double Speed1, double d) {
    Acc = fmin(10 * (pow(Speed1, 2) - pow(Speed0, 2)) / (2 * d), 0);
}

template <typename T> void round_position(T obj, Vector3 &Motion) {
    Vector3 motion = obj->get_global_transform().get_origin();
    motion.x = round(motion.x); motion.z = round(motion.z);
    obj->set_translation(motion);
}

template <typename T> void align_on_axis(T obj) {
    obj->set_rotation_degrees(Vector3(round(obj->get_rotation_degrees().x / 90) * 90, round(obj->get_rotation_degrees().y / 90) * 90, round(obj->get_rotation_degrees().z / 90) * 90));
}

// Constructors
Transport::Transport() {
    transport_type(0);
}

void Transport::transport_type(int type) {
    // initialize graphical variables
    motion = Vector3(0, 0, 0);
    rot = (M_PI / 2);
    center = Vector3(0, 0, 0);
    dir = 0;
    position = 0;
    SPEED_T = 0;
    kmPerDay = 0;
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
            if (satisfaction > 10) {
                satisfaction = 10;
            }
            break;
        }
        case 1: {  // big american car
            co2PerKm = 0.328; 
            fuelPerKm = 0.24; 
            std::normal_distribution <double> costg(85000, 12000);
            cost = costg(gen); // cost of 1 car in euros, randomised using gaussian
            cost = 25;
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
        case 7:{ //sports car
            fuelPerKm = 0.22;
            co2PerKm = 0.5;
            std::normal_distribution <double> costsp(52000, 7000);
            cost = costsp(gen); // cost of 1 car in euros, randomised using gaussian
            capacity = 2;
            std::normal_distribution <double> occupancysp(0.75, 0.25);
            occupancyRate = occupancysp(gen); // average percentage occupancy of the car: number of people in the bus / capacity
            std::normal_distribution <double> timeg(1.5, 0.75);
            buildingTime = timeg(gen); // building time of 1 car, very fast
            std::normal_distribution <double> satisfactionsp(9.5, 0.5); // satisfaction level is very high, randomised
            satisfaction = satisfactionsp(gen);
            if (satisfaction >10) {
                satisfaction = 10;      
            }
            std::normal_distribution <double> kmsp(80, 20);
            kmPerDay = kmsp(gen); // kilometres per day,  randomised for each car
            break;
        }
    }
}

void Transport::_register_methods() {
    register_method((char*)"_init", &Transport::_init);
    register_method((char*)"_process", &Transport::_process);
    register_method((char*)"_ready", &Transport::_ready);
}

void Transport::_init(){

}

void Transport::_ready(){
    prevPosition = this->get_global_transform().get_origin();
    myCity = (City*)(this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld"));
}

void Transport::_process(float delta){
    if (rot >= (M_PI / 2)) {	
        straight(delta);
        prevPosition = this->get_global_transform().get_origin();

        int real_rot = round(this->get_rotation_degrees().y /90);

        if (position >= 13 && Acc > 0 && real_rot % 2 == 0) { 
            ComputeAcceleration(Acc, SPEED_T, 0.2, 5);
        } else if (position >= 18 && Acc > 0 && real_rot % 2 == 1 && dir == 1) { 
            // Decrease also the speed before turning right (small radius)
            ComputeAcceleration(Acc, SPEED_T, 0.7, 4);
        } 

        compute_speed(SPEED_T, Acc, delta);

        if (position >= 22 && dir == 1 or position >= 18 && dir == -1 or position >= 22 && dir == 0) {
            Acc = 0.5;	rot = 0;
            round_position(this, motion);
            center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z)*Turn_R*dir;
        }
    } else if (position >= 22 && dir == 1 or position >= 18 && dir == -1 or position >= 22 && dir == 0) {

        compute_speed(SPEED_T, Acc, delta);
        Vector3 globalSpeed = Vector3((SPEED_T * 10), 0, 0);
        globalSpeed.rotate(Vector3(0, 1, 0), (this->get_rotation_degrees().y) * (M_PI / 180));

        if (this->move_and_collide(globalSpeed, true, true, true) == NULL) {
            turn(dir, delta);
        }
        else {
            Vector3 colliderVelocity = this->move_and_collide(Vector3(0, 0, 0), true, true, true)->get_collider_velocity();
            if (colliderVelocity.dot(colliderVelocity) < SPEED_T) {
               // turn(dir, delta);
            }
        }
        prevPosition = this->get_global_transform().get_origin();

        if ((dir != 0 && rot >= (M_PI / 2)) or dir == 0) {
            rot = M_PI / 2;
            align_on_axis(this);
            round_position(this, motion);

            switch (dir) {
            case -1 : position = 8; break;
            case 0: position = -8; break;
            default: position = 4; break;
            }

            dir = get_direction(this->get_global_transform().get_origin() + Vector3(12,0,0).rotated(Vector3(0, 1, 0), this->get_rotation_degrees().y * (M_PI/180)), this->get_rotation_degrees().y );

            switch (dir) {
            case -1: Turn_R = 12; break;
            default: Turn_R = 4; break;

                switch ( (int)(round(this->get_rotation_degrees().y / 90) + 4)%4 ) {
                case 0: this->set_axis_lock(0, true);  break;
                case 1: this->set_axis_lock(2, true);  break;
                case 2: this->set_axis_lock(0, true);  break;
                case 3: this->set_axis_lock(2, true);  break;
                default:  this->set_axis_lock(0, true); break;
                }
            }
        }
    }
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
        case 7:{ //sports car
            maintenance+=2.5*days;
        }

    }
}

void Transport::turn(int dir, float delta) {
    double drot = (SPEED_T * delta) * 10;
    if (dir == 1) { drot /= 4; }
    else { drot /=  12 ; }
    rot += drot;
    
    this->global_translate(-center);			//define the center of rotation
    this->set_transform(this->get_transform().rotated(Vector3(0, 1, 0), -drot*dir));
    this->global_translate(center);				//reset the center of rotation

    if (dir == 1) {
        ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, -(90 / M_PI) * sin(rot* 2) , -(180 / M_PI) * rot * 4));
        ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, -(90 / M_PI)  * sin(rot * 2), -(180 / M_PI) * rot * 4));
        ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(90 / M_PI)  * sin(rot * 2) * 4));
    }
    else {
        ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, (90 / M_PI)  * sin(rot * 2) , -(180 / M_PI) * rot * 12));
        ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, (90 / M_PI)  * sin(rot * 2) , -(180 / M_PI) * rot * 12));
        ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * rot * 12));
    }
}

void Transport::straight(float delta) {
    //Vector3 globalSpeed = Vector3((SPEED_T * delta*10), 0, 0);
    Vector3 globalSpeed = Vector3((SPEED_T  * 10), 0, 0);
    globalSpeed.rotate(Vector3(0, 1, 0), (this->get_rotation_degrees().y) * (M_PI / 180));
    //this->move_and_collide(globalSpeed, true, true, false);
    if ((int)((rot / 90) + 4) % 2 == 0) {
        this->move_and_slide_with_snap(globalSpeed, Vector3(), Vector3(1, 0, 0), true);
    }
    else {
        this->move_and_slide_with_snap(globalSpeed, Vector3(), Vector3(0, 1, 0), true);
    }
    
    //position += SPEED_T * delta * 10;
    Vector3 pos = this->get_global_transform().get_origin() - prevPosition;
    position += pos.normalized().dot(pos);				//Get the norm....
    //position += SPEED_T * delta * 10;
    prevPosition = this->get_global_transform().get_origin();	
    
    ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position ));
    ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));
    ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));
}

int Transport::get_direction(Vector3 pos, double rot) {
    int rotInt = (int)((rot / 90)+4) % 4 ;
    vector<int> out;
    
    if ((int)round(pos.x / 30) >= sizeof(traffic) or (int)round(pos.z / 30) >= sizeof(traffic[0])) {
        myCity->add_car();
        this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->remove_child(this);
        myCity->add_car();
        return(0);
    }	
    
    int i = -1;
    for (const int& n : traffic[(int)round(pos.x / 30)][(int)round(pos.z / 30)][(int)rotInt]) { 
        if (n == 1) {
            out.push_back(i);
        }
        i++;
    }

    if (out.size() == 0) {
        
        myCity->add_car();
        this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->remove_child( this);
        myCity->add_car();
        return(0);
    }

    return(out[rand() % out.size()]);
}
