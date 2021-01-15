#include "Transport.h"

#include <SceneTree.hpp>
#include <Viewport.hpp>
#include <KinematicCollision.hpp>
#include <Mesh.hpp>
#include <Timer.hpp>
#include <random>
#include <AnimationPlayer.hpp>

# define M_PI 3.14159265358979323846  /* pi */

using namespace godot;


extern int traffic_system[citysize][citysize][4][3];

// helper functions
void compute_speed(double& Speed, double &Acc, float delta, Vector3 prevPos, Vector3 pos) {
    //Speed = (pos - prevPos).dot((pos - prevPos).normalized()) / (10 * delta);
    if ((Speed <= 0.8 && Acc > 0) or (Acc < 0 and Speed + Acc * delta > 0.2)) {
        Speed += Acc * delta;
    }
    else if (Acc < 0 && Speed < 0.2) {
        Acc = 0.1;
        Speed += Acc * delta;
    }

}

void ComputeAcceleration(double& Acc, double Speed0, double Speed1, double d) {
    Acc = fmin(10 * (pow(Speed1, 2) - pow(Speed0, 2)) / (2 * d), 0);
}

template <typename T> void round_position(T obj, Vector3& Motion) {
    Vector3 motion = obj->get_global_transform().get_origin();
    motion.x = round(motion.x); motion.z = round(motion.z);
    obj->set_translation(motion);
}

template <typename T> void align_on_axis(T obj) {
    obj->set_rotation_degrees(Vector3(round(obj->get_rotation_degrees().x / 90) * 90, round(obj->get_rotation_degrees().y / 90) * 90, round(obj->get_rotation_degrees().z / 90) * 90));
}


Transport::Transport() {
    this->transportType = 4;
    set_transportType(4);
}

Transport::Transport(int type){
    this->transportType = type;
    set_transportType(type);
}



void Transport::set_transportType(int type)
{
    this->transportType = type;
    std::cout << "setter is used for transport type value : " << this->transportType << std::endl;
    // initialize graphical variables
    motion = Vector3(0, 0, 0);
    rot = (M_PI / 2);
    center = Vector3(0, 0, 0);
    dir = 0;
    position = 0;
    SPEED_T = 0;
    kmPerDay = 0;
    maintenance = 0; // maintenance cost for the whole duration of simulation
    energyUse = 0; //energy per day
    passengers = 0; //total number of passengers that used the car
    age = 0; //age in days
    employment = 0; // employees only for the bus
    // random device class instance, source of 'true' randomness for initializing random seed
    std::random_device rd;
    std::mt19937 gen(rd());
    switch ((int)type) {
    case 0: { //electric car
        co2PerKm = 0;
        capacity = 5;
        std::normal_distribution <double> occupancyt(0.4, 0.2);
        occupancyRate = occupancyt(gen); // average percentage occupancy of the car: number of people in the car / capacity
        fuelPerKm = 0;
        std::normal_distribution <double> kmt(70, 15);
        kmPerDay = kmt(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> costt(60000, 8500); //cost randomised using gaussian
        cost = costt(gen);
        std::normal_distribution <double> timet(4, 1);
        buildingTime = timet(gen); // building time of 1 electric car in days, taking tesla model 3
        satisfaction = 7.45;
        energyUse = 0.119 * kmPerDay*365; //in 1 year
        lifetime = 15;
        pricePerMonth = 1.09 * 30 ;
        weight = 1.7;
        break;
    }
    case 1: {  // big american car
        co2PerKm = 0.328; //in kg
        fuelPerKm = 0.24; //in litres
        std::normal_distribution <double> costg(85000, 12000);
        cost = costg(gen); // cost of 1 car in euros, randomised using gaussian
        capacity = 8;
        std::normal_distribution <double> kmg(85, 15);
        kmPerDay = kmg(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> occupancyg(0.375, 0.25);
        occupancyRate = occupancyg(gen); // average percentage occupancy of the car: number of people in the car / capacity
        std::normal_distribution <double> timeg(1, 0.5);
        buildingTime = timeg(gen); // building time of 1 car, very fast
        satisfaction = 9.3;
        lifetime = 12;
        pricePerMonth = 1.25 * 30;
        weight = 2.5;
        break;
    }
    case 2: { //normal family car 
        co2PerKm = 0.115; //in kg
        fuelPerKm = 0.078; //in litres
        std::normal_distribution <double> costf(14000, 3000);
        cost = costf(gen); // cost of 1 car in euros, randomised using gaussian
        capacity = 4;
        std::normal_distribution <double> kmf(50, 10);
        kmPerDay = kmf(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> occupancyf(0.5, 0.25);
        occupancyRate = occupancyf(gen); // average percentage occupancy of the car: number of people in the car / capacity
        std::normal_distribution <double> timeg(1, 0.5);
        buildingTime = timeg(gen); // building time of 1 car, very fast
        satisfaction = 7.3;
        lifetime = 15;
        pricePerMonth = 0.15 * 30;
        weight = 0.98;
        break;
    }
    case 3: { //old collection car 
        co2PerKm = 0.4; //in kg
        fuelPerKm = 0.22; //in litres
        std::normal_distribution <double> costo(40000, 7000);
        cost = costo(gen); // cost of 1 car in euros, randomised using gaussian
        capacity = 2;
        std::normal_distribution <double> kmo(80, 15);
        kmPerDay = kmo(gen); // average km per day for this car using gaussian
        std::normal_distribution <double> occupancyo(0.75, 0.25);
        occupancyRate = occupancyo(gen); // average percentage occupancy of the car: number of people in the car / capacity
        std::normal_distribution <double> timeo(7, 2);
        buildingTime = timeo(gen); // building time of 1 collection (replica i think) car car, not so fast
        satisfaction = 7.6;
        lifetime = 20;
        pricePerMonth = 25 * 30;
        weight = 2;
        break;
    }
    case 4: { //bike
        fuelPerKm = 0; //in litres
        co2PerKm = 0; //in kg
        std::normal_distribution <double> costbike(370, 30);
        cost = costbike(gen); // cost of 1 bike in euros, randomised using gaussian
        capacity = 1;
        occupancyRate = 1;
        buildingTime = 0.04; //really fast, in days (1 hour )
        satisfaction = 3; //meduim satisfaction
        std::normal_distribution <double> kmbike(18, 7);
        kmPerDay = kmbike(gen); // kilometres per day,  randomised for each bike
        lifetime = 5;
        pricePerMonth = 0.8 * 30;
        weight = 0.15; //not true but in order for the tax to not be applied
        break;
    }
    case 5: { //motorcycle 
        fuelPerKm = 0.044; //in literss
        co2PerKm = 0.11; //in kg
        std::normal_distribution <double> costm(12000, 2000);
        cost = costm(gen); // cost of 1 motorcycle in euros, randomised using gaussian
        capacity = 1;
        occupancyRate = 1;
        buildingTime = 0.12; //really fast, in days (3 hours )
        satisfaction = 7;
        std::normal_distribution <double> kmbike(30, 10);
        kmPerDay = kmbike(gen); // kilometres per day,  randomised for each motorcycle
        lifetime = 12;
        pricePerMonth = 2.6 * 30;
        weight = 0.01; //not true but in order for the tax to not be applied
        break;
    }
    case 6: { // bus
        fuelPerKm = 0.26; //in liters
        co2PerKm = 1.25; //in kg
        std::normal_distribution <double> costb(230500, 20000);
        cost = costb(gen); // cost of 1 bus in euros, randomised using gaussian
        double alpha = (cost - 262500) / 262500;
        if (alpha < 0) {
            alpha = 0;
        }
        if (alpha > 1) {
            alpha = 1;
        }
        capacity = 30 + alpha * 30;
        capacity = round(capacity); //maximum number of people in 1 bus, positive correlation with cost 
        std::normal_distribution <double> occupancyb(0.70, 0.30);
        occupancyRate = occupancyb(gen); // average percentage occupancy of the bus: number of people in the bus / capacity
        if (occupancyRate > 1) {
            occupancyRate = 1;
        }
        std::normal_distribution <double> buildingb(22, 3);
        buildingTime = buildingb(gen); // construction time for 1 bus, in days
        satisfaction = 3;
        if (satisfaction > 10) {
            satisfaction = 10;
        }
        std::normal_distribution <double> kmb(225, 75);
        kmPerDay = kmb(gen); // kilometres per day,  randomised for each bus
        employment = 1 + round(alpha * 2);
        lifetime = 16;
        pricePerMonth = (0.67 + 0.18 * alpha) * kmPerDay * 30;
        weight = 11; //not true but in order for the tax to not be applied
        break;
    }
    default : { //sports car
        fuelPerKm = 0.22;  //in litres
        co2PerKm = 0.5; //in kg
        std::normal_distribution <double> costsp(52000, 7000);
        cost = costsp(gen); // cost of 1 car in euros, randomised using gaussian
        capacity = 2;
        std::normal_distribution <double> occupancysp(0.75, 0.25);
        occupancyRate = occupancysp(gen); // average percentage occupancy of the car: number of people in the bus / capacity
        std::normal_distribution <double> timeg(1.5, 0.75);
        buildingTime = timeg(gen); // building time of 1 car, very fast
        satisfaction = 9.2;
        if (satisfaction > 10) {
            satisfaction = 10;
        }
        std::normal_distribution <double> kmsp(80, 20);
        kmPerDay = kmsp(gen); // kilometres per day,  randomised for each car
        lifetime = 10;
        pricePerMonth = 2.5 * 30;
        weight = 1.5;
        break;
    }
    }

    pricePerMonth += cost / (12 * lifetime);
    fuelInput = fuelPerKm * kmPerDay * 365; // litres of fuel in 1 year
    CO2Emission =365 * co2PerKm * kmPerDay / 1000; // tonnes of co2 in 1 year
    

}

int Transport::get_transportType()
{
    return this->transportType;
}

void Transport::transport_type() {
    
}

void Transport::_register_methods() {
    register_method((char*)"_init", &Transport::_init);
    register_method((char*)"_physics_process", &Transport::_physics_process);
    register_method((char*)"_ready", &Transport::_ready);
    register_method((char*)"transport_type", &Transport::transport_type);

    register_property<Transport, int>("transportType", &Transport::set_transportType, &Transport::get_transportType, 0);
}


void Transport::_init() {
    
}

void Transport::_ready() {
    prevPosition = this->get_global_transform().get_origin().dot(get_global_transform().get_basis().get_axis(0).normalized());
    prevPositionVec = this->get_global_transform().get_origin();
    myCity = (City*)((this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")));
    transport_type();
}


void Transport::_physics_process(float delta) 
{
    (this->physics_counter)++;

    if (this->physics_counter == 2) {
        this->physics_counter == 0;
        //delta = delta / 2;
    }

    prevPositionVec = this->get_global_transform().get_origin();
   
   
    if (rot >= (M_PI / 2)) {
        
        int gameSpeed = myCity->get("time_speed");
        if (gameSpeed != 0) { straight(fmin(0.04, delta) / delta); }
       
        Vector3 p = this->get_global_transform().get_origin();
        switch ((int)(((this->get_rotation_degrees().y) / 90) + 4) % 4) {                                   //Put the car on the road if problems
        case 0: this->set("translation", Vector3(p.x, 0, p.z + 28 - fmod(p.z + 28, 30) - 13)); break;
        case 2: this->set("translation", Vector3(p.x, 0, p.z + 2 - fmod(p.z + 2, 30) + 13)); break;
        case 3: this->set("translation", Vector3(p.x + 2 - fmod(p.x + 2, 30) + 13, 0, p.z)); break;
        case 1: this->set("translation", Vector3(p.x + 28 - fmod(p.x + 28, 30) - 13, 0, p.z)); break;
        default: break; }

        int real_rot = round(this->get_rotation_degrees().y / 90);

        if (position >= 13 && Acc > 0 && real_rot % 2 == 0) {
            ComputeAcceleration(Acc, SPEED_T, 0.2, 5);
        }
        else if (position >= 18 && Acc > 0 && real_rot % 2 == 1 && dir == 1) {
            // Decrease also the speed before turning right (small radius)
            ComputeAcceleration(Acc, SPEED_T, 0.7, 4);
        }

        

        if (position >= 22 ) {
            Acc = 0.5;	rot = 0;
            round_position(this, motion);
            center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z) * Turn_R * dir;
        }
    }
    else if (position >= 22) {


        Vector3 globalSpeed = Vector3((SPEED_T * 10 * fmin(0.04, delta)), 0, 0);
        globalSpeed.rotate(Vector3(0, 1, 0), (this->get_rotation_degrees().y) * (M_PI / 180));
        

        if (this->move_and_collide(globalSpeed, true, true, true) == NULL) { //No collision
            turn(dir, fmin(0.04, delta));
        }
        else {
            
            Vector3 colliderVelocity = this->move_and_collide(globalSpeed, true, true, true)->get_collider_velocity();
            if (fmod(((Transport*)(this->move_and_collide(globalSpeed, true, true, true)->get_collider()))->get_rotation_degrees().y + 360, 90) != 0) { //Car not on a straight line
                if (colliderVelocity.dot(colliderVelocity) < SPEED_T) {
                    turn(dir, fmin(0.04, delta));
                }
            }
            else if (colliderVelocity.dot(colliderVelocity) == 0) {
                turn(dir, fmin(0.04, delta));
            }

        }

        if ((dir != 0 && rot >= (M_PI / 2)) or dir == 0) {
            rot = M_PI / 2;
            align_on_axis(this);
            round_position(this, motion);
            
            switch (dir) {
            case 0: position = -8; break;
            default: position = 4; break;
            }

            prevPosition = this->get_global_transform().get_origin().dot(get_global_transform().get_basis().get_axis(0).normalized()) - position;
            dir = get_direction(this->get_global_transform().get_origin() + Vector3(12, 0, 0).rotated(Vector3(0, 1, 0), this->get_rotation_degrees().y * (M_PI / 180)), this->get_rotation_degrees().y);

            switch (dir) {
            case -1: Turn_R = 8; break;
            default: Turn_R = 4; break;
            }
        }
    }

    compute_speed(SPEED_T, Acc, fmin(0.04, delta), prevPositionVec, this->get_global_transform().get_origin());
}

void Transport::simulate_step(double days) {
    int years = floor((age + days) / 365 - age / 365);
    co2PerKm *= pow(1.05, years); //increase in emissions with each year
    age += days; //total number of days 
    /*fuelInput += fuelPerKm * kmPerDay * days; //litres of fuel for car
    CO2Emission += co2PerKm * kmPerDay * days;*/ // co2 emissions per car
    fuelInput = fuelPerKm * kmPerDay * 365; // litres of fuel in 1 year
    CO2Emission =365 * co2PerKm * kmPerDay / 1000; // tonnes of co2 in 1 year
    passengers = capacity * occupancyRate * 365; //number of people that used the car in 1 year period
    switch (transportType) {
        //maintenance in euros per year
    case 0: { //electric car
        if (age <= 365) {
            maintenance = 1.09 * 365; //maintenance service price per day if car is less than 1 yo
        }
        else if (age <= 730) {
            maintenance = 1.67 * 365; //maintenance service price per day if car is less than 2 yo
        }
        else if (age <= 1095) {
            maintenance = 1.1 * 365; //maintenance service price per day if car is less than 3 yo
        }
        if (age <= 1460) {
            maintenance = 1.94 * 365; // maintenance service price per day is car is less than 4 yo
        }
        else {
            maintenance = 2.2 * 365; // maintenance service price per day if car is more than 4 yo
        }
        break;
    }
    case 1: { // big american car
    // no repairs price if car is <3yo
        if (age <= 365) {
            maintenance = 1.25 * 365; //maintenance price per day if car is less than 1 yo
        }
        else if (age <= 730) {
            maintenance = 1.92 * 365; //maintenance price per day if car is less than 2 yo
        }
        else if (age <= 1095) {
            maintenance = 1.95 * 365; //maintenance price per day if car is less than 3 yo
        }
        if (age <= 1460) {
            maintenance = 1.86 * 365; // repairs price per day is car is less than 4 yo
            maintenance = 5.7 * 365; //maintenance price per day if car is less than 4 yo
        }
        else {
            maintenance = 2.78 * 365; //repairs price per day if car is more than 4 yo
            maintenance = 3.21 * 365; //maintenance price per day if car is more than 4 yo
        }
        break;
    }
    case 2: { // normal family car
    // no repairs price if car is <3yo
        if (age <= 365) {
            maintenance = 0.15 * 365; //maintenance price per day if car is less than 1 yo
        }
        else if (age <= 730) {
            maintenance = 0.5 * 365; //maintenance price per day if car is less than 2 yo
        }
        else if (age <= 1095) {
            maintenance = 1.05 * 365; //maintenance price per day if car is less than 3 yo
        }
        if (age <= 1460) {
            maintenance = 0.765 * 365; // repairs price per day is car is less than 4 yo
            maintenance += 2.70 * 365; //maintenance price per day if car is less than 4 yo
        }
        else {
            maintenance = 1.17 * 365; //repairs price per day if car is more than 4 yo
            maintenance = 2.26 * 365; //maintenance price per day if car is more than 4 yo
        }
        break;
    }
    case 3: { // collection car 
    // no repairs price if car is <3yo
        maintenance = 25 * 365; //maintenance very expensive, does not depend on the age, the car is already old
        break;
    }
    case 4: { //bike
        maintenance = 0.8 * 365;
    }
    case 5: { //motorcycle
        maintenance = 2.6 * 365;
    }
    case 6: { //bus
        double alpha = (cost - 262500) / 262500;
        if (alpha < 0) {
            alpha = 0;
        }
        maintenance = (0.67 + alpha * 0.17) * kmPerDay *365; // maintenance cost in euros, positive correlation with bus price
        break;
    }
    case 7: { //sports car
        maintenance = 2.5 * 365;
        break;
    }
    }
    //car prohibition
    /*
    if ((transportType!=6)&&(transportType!=5)&&(transportType!=4)) {
        {if (workingDays!= (7-(int)(myCity->get("carProhibition")))) {
        satisfaction /= workingDays/7;
     }
     
    maintenance*=workingDays/7;
    fuelInput*=workingDays/7;
    CO2Emission*=workingDays/7;
    energyUse*=workingDays/7;
    passengers*=workingDays/7;
    }
    }
    */
    //car consumption tax
    maintenance+=fuelInput*(int)(myCity->get("fuelTax"));
}

void Transport::turn(int dir, float delta) {
    double drot = (SPEED_T * delta * int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed"))) * 5 ;
    if (dir == 1) { drot /= 4; }
    else { drot /= 12; }
    rot += drot;

    this->global_translate(-center);			//define the center of rotation
    this->set_transform(this->get_transform().rotated(Vector3(0, 1, 0), -drot * dir));
    this->global_translate(center);				//reset the center of rotation

    if (dir == 1) {
        
        if (transportType == 5) {
            ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3((90 / M_PI) * sin(rot * 2), 0, -(180 / M_PI) * rot * 4));
            ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3((90 / M_PI) * sin(rot * 2), 0, -(180 / M_PI) * rot * 4));
            ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3((90 / M_PI) * sin(rot * 2), 0, 0));
        }
        else if ((transportType != 4)) {
            ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, -(90 / M_PI) * sin(rot * 2), -(180 / M_PI) * rot * 4));
            ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, -(90 / M_PI) * sin(rot * 2), -(180 / M_PI) * rot * 4));
            ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * rot * 4));
        }

    }
    else {
        if (transportType == 5) {
            ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(-(90 / M_PI) * sin(rot * 2), 0, -(180 / M_PI) * rot * 4));
            ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(-(90 / M_PI) * sin(rot * 2), 0, -(180 / M_PI) * rot * 4));
            ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(-(90 / M_PI) * sin(rot * 2), 0, 0));
        }
        else if ((transportType != 4)) {
            ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, (90 / M_PI) * sin(rot * 2), -(180 / M_PI) * rot * 12));
            ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, (90 / M_PI) * sin(rot * 2), -(180 / M_PI) * rot * 12));
            ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * rot * 12));
        }
       
    }
}

void Transport::straight(float ratioDelta) {
    

    Vector3 globalSpeed = Vector3((ratioDelta * SPEED_T * 5 * int(((City*)(this->get_tree()->get_root()->get_node("Main/3Dworld")))->get("time_speed"))), 0, 0);
    globalSpeed.rotate(Vector3(0, 1, 0), (this->get_rotation_degrees().y) * (M_PI / 180));

    switch ((int)(((this->get_rotation_degrees().y) / 90) + 4) % 4) {
    case 0: this->move_and_slide_with_snap(globalSpeed, Vector3(), Vector3(-1, 0, 0), true).z; break;
    case 2: this->move_and_slide_with_snap(globalSpeed, Vector3(), Vector3(1, 0, 0), true).z; break;
    case 1: this->move_and_slide_with_snap(globalSpeed, Vector3(), Vector3(0, 0, 1), true).x; break;
    case 3: this->move_and_slide_with_snap(globalSpeed, Vector3(), Vector3(0, 0, -1), true).x; break;
    default:     break;
    }

    position = this->get_global_transform().get_origin().dot(get_global_transform().get_basis().get_axis(0).normalized()) - prevPosition;
    if (transportType != 4) {
        ((Mesh*)this->get_child(0))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));
        ((Mesh*)this->get_child(1))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));
        if (transportType != 5) { // not a two wheels
            ((Mesh*)this->get_child(2))->set("rotation_degrees", Vector3(0, 0, -(180 / M_PI) * position));
        }
    }
}

int Transport::get_direction(Vector3 pos, double rot) {
    int rotInt = (int)((rot / 90) + 4) % 4;
    std::vector<int> out;

    if ((int)round(pos.x / 30) >= sizeof(traffic_system) or (int)round(pos.z / 30) >= sizeof((traffic_system[0]))) {
        myCity->remove_type_car(get_transportType());
        this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->remove_child(this);
        return(0);

    }
    int i = -1;
    for (const int& n : (traffic_system)[(int)round(pos.x / 30)][(int)round(pos.z / 30)][(int)rotInt]) {
        if (n == 1) {
            out.push_back(i);
        }
        i++;
    }
    

    if (out.size() == 0) {
        myCity->remove_type_car(get_transportType());
        this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")->remove_child(this);
        return(0);
    }



    return(out[rand() % out.size()]);
}

double Transport::get_satisfaction(){
	return this->satisfaction;
}

double Transport::get_co2emissions(){
	return this->CO2Emission;
}

double Transport::get_energyuse(){
	return this->energyUse;
}

double Transport::get_environmentalcost(){
    return 0; //at least for now
}

Pedestrian::Pedestrian() {
    motion = Vector3(0, 0, 0);
    rot = (M_PI / 2);
    center = Vector3(0, 0, 0);
    dir = 1;
    position = 6;
    SPEED_T = 2;
    Turn_R = 2;
}



void Pedestrian::_register_methods() {
    register_method((char*)"_init", &Pedestrian::_init);
    register_method((char*)"_process", &Pedestrian::_process);
    register_method((char*)"_ready", &Pedestrian::_ready);
}

void Pedestrian::_init() {
    
    
}

void Pedestrian::_ready() {
    player = (AnimationPlayer*)(this->get_child(0));
    player->play("Walk");

    prevPosition = this->get_global_transform().get_origin().dot(get_global_transform().get_basis().get_axis(0).normalized());
    prevPositionVec = this->get_global_transform().get_origin();
    myCity = (City*)((this->get_tree()->get_root()->get_node("Main")->get_node("3Dworld")));
}

void Pedestrian::_process(float delta) {
    //std::cout << position << endl;
    player->set_speed_scale(int(myCity->get("time_speed"))); //TO BE CHANGED

    if (rot >= (M_PI / 2)) {

        if ( ((int)(myCity->get("time_speed"))) != 0) { straight(delta); }

        Vector3 p = this->get_global_transform().get_origin();
        switch ((int)(((this->get_rotation_degrees().y) / 90) + 4) % 4) {                                   //Put the pedestrian on the road if problems
        case 0: this->set("translation", Vector3(p.x, 0, p.z + 24 - fmod(p.z + 24, 30) - 9)); break;
        case 2: this->set("translation", Vector3(p.x, 0, p.z + 6 - fmod(p.z + 6, 30) + 9)); break;
        case 3: this->set("translation", Vector3(p.x + 6 - fmod(p.x + 6, 30) + 9, 0, p.z)); break;
        case 1: this->set("translation", Vector3(p.x + 24 - fmod(p.x + 24, 30) - 9, 0, p.z)); break;
        default: break;
        }


        if (position >= 2*9 - 2 * Turn_R) {
            rot = 0;
            round_position(this, motion);
            center = this->get_global_transform().get_origin() + (this->get_global_transform().get_basis().orthonormalized().z) * Turn_R * dir;
        }
    }
    else if (position >= 2*9 - 2 * Turn_R) {
        turn(dir, delta);

        if (rot >= (M_PI / 2)) {
            rot = M_PI / 2;
            align_on_axis(this);
            round_position(this, motion);

            prevPosition = this->get_global_transform().get_origin().dot(get_global_transform().get_basis().get_axis(0).normalized());
        }
    }   
}

void Pedestrian::turn(int dir, float delta) {
    double drot = (SPEED_T * delta * int(myCity->get("time_speed"))) / Turn_R;
    rot += drot;

    this->global_translate(-center);			//define the center of rotation
    this->set_transform(this->get_transform().rotated(Vector3(0, 1, 0), -drot * dir));
    this->global_translate(center);				//reset the center of rotation


}

void Pedestrian::straight(float delta) {
    Vector3 globalSpeed = Vector3(SPEED_T * delta * int(myCity->get("time_speed")), 0, 0);
    globalSpeed.rotate(Vector3(0, 1, 0), (this->get_rotation_degrees().y) * (M_PI / 180));

    this->move_and_collide(globalSpeed);
    position = this->get_global_transform().get_origin().dot(get_global_transform().get_basis().get_axis(0).normalized()) - prevPosition;

}
