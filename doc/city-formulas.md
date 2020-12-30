# City Formulas

This document contains all the formulas we use in our city and give some brief explanations. 

## City

* income = sum of all income from houses = all income from producing buildings <- could be a way to test the accuracy of our simulation
* GDP = income = output - input of all buildings <- could be a way to test the accuracy of our simulation
* unemployment = 1 - sum of all employees in producing buildings / total population
* carbon emission = sum of CO2emissions of all buildings
* air pollution = carbon emission over time summed / volume of air available over the city (TBD some wind influence) parts per billion
* healthcare = under the assumption of 2 hospital types (medium-end & high-end): 100 - (negative hospital points) 
    for each hospital: if medium, then ( if (occupancy rate - 75%) > 0) += negative hospital points
                        if high-end, then ( if (occupancy rate - 85%) > 0) += negative hospital points
    min: 0
* population = sum of people living in houses
* energyDemand = sum of energy use by buildings needing energy
* energySupply = - sum of energy use by buuldings supplying energy
* totalsatisfaction = coefs TBD : buildingsatisfaction = (for every building: coef related to type of building * satisfaction number)/(sum of each coef*(number of buildings))  - coef1*radiation - coef2*nuclearwaste + exponentialcoef3*(healthcare-95)+ exponentialcoef4*(unemployment-5) - coef5*(if energyDemand-energySupply>0) + number if energydemand-energysupply<0 - coef6*restrictingpolicies + exponentialcoef7*(60-airpollution)

## Restaurant (Structure -> Shop -> Restaurant)

Restaurant is a subclass of Shop

* income =  all income from houses within 500 units * 0.05
* CO2 Emissions = some value, changed by presence of solar panel / windmill, changed by share of type of transportation
* employment = income / 2 * (average wage of low income worker)
