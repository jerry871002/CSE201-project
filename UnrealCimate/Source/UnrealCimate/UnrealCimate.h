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