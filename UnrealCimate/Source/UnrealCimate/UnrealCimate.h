// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

//Contains a GameModeBase class which I called city and represents our "world", it will not exist on the interface but defines the rules and settings for the game.
//For now, we put all the structure subclasses without using the unreal editor; for each object which will appear in the 3d world, we will however need to use the unreal class wiard

#include "CoreMinimal.h"

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "City.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT2_API ACity : public AGameModeBase
{
	GENERATED_BODY()
	//time seetings
	//reset function
};

class Structure : public MYPROJECT2_API ACity {
public:
    double cost, location, age, type;
    //solar panels template function which can be applied to any building (ie shop)
    template <typename T> void Solar_panels(T){
	    //if "button clicked" : solar panels appear
    }
}

class Production : public Structure {
public:
    double input, output, efficiency;
    int employment;
}

class Housing : public Structure {
public:

}

class PublicServices : public Structure {
public:
    double input, output, efficiency;
    int employment;
}

class Factory : public Production {


}

class Shop : public Production {
    //constructor :
    Shop(double employment){
	    //if employment high enough : shope open
	    //else : shope closed
    };
}


class Energy : public Factory {
    double energy_output;
    double environmental_impact();
}

class Solar : public Energy {
    double energy_output;
    double environmental_impact() //redefined don't use same as the one in energy
}

class Nuclear : public Energy {
    double energy_output;
    double environmental_impact() //redefined don't use same as the one in energy
}

class Water : public Factory {
    double water_output;
    double environmental_impact();
}

class Data : public MYPROJECT2_API ACity {
public:
    // no general attributes yet
}

class Indices : public Data {
public:
    int max=100;
    int min=0;
    int current_value;
    int long_term;
}

/* not needed (just mind map)? :
class Environmental : public Indices {
public:
    
}

class Social : public Indices {
public:
    
}

class Economical : public Indices {
public:
    
}

*/

class Pollution : public Indices {
    //input: mainly:factory outputs, also: housing and public services level
}

class Happiness : public Indices {
    //input: factories nature, housing and public services level, unemployment, average income, restrictions
}

class Unemployment : public Indices {
    //input: mainly: capacity of factories, also: happiness, public services
}

class Income : public Indices {
    //input: mainly: factories nature/capacity
}

class Counters : public Data {
public:
    double current;
    int min=0;
    //optimal value: city usage
}

class GDP : public Counters {
    //input: average income, unemployment
}

class CarbonEmissions : public Counters {
    //input: factories nature/level/capacity, housing/public services level, restrictions
}

/* not needed?
class Resources : public Counters {
    //input: factories output - city usage
}
*/

class Energy : public Counters {
     //input: dedicated factories output - city usage
}

class Water : public Counters {
    //input: dedicated factories output - city usage
}