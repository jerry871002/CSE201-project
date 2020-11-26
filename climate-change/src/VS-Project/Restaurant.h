#include "obj.h"


class Restaurant: public Shop{
public:
	Restaurant();
	~Restaurant();
	void _register_methods();
	void _init();
	void _process(float delta);
	void _input(InputEvent* e);
	void _ready();


	// variable inherited from above: Age/maintenance, Energy use, Satisfaction, Environmental_Impact 
	//Input, Output , Employment
	double get_shock(double upper_bound);
	double get_energyuse();
	double get_popularity(); 		//calculates a random shock, having impact 
	void calculate_demand(); 


protected:
	double demand;  //idea is demand between 0-100
	double owner_skills; // between 0-1
	double popularity; // between 0-100
	double shock;  //random external factors
	double costs; //costs of the restaurant
	double revenue;  //overall revenue

	





};