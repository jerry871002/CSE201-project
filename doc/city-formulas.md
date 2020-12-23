# City Formulas

This document contains all the formulas we use in our city and give some brief explanations. 

## Restaurant (Structure -> Shop -> Restaurant)

Restaurant is a subclass of Shop

* income =  all income from houses within 500 units * 0.05
* CO2 Emissions = some value, changed by presence of solar panel / windmill
* employment = income / 2 * (average wage of low income worker)



City Satisfaction:

Satisfaction should be only tied to the houses : total satisfaction is weighted by population
each hous's satisfaction is counted up by the houses around it 