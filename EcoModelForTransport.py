
import numpy as np
import random

#City
airQuality = 1
averageWage = 1600


revenue = 0
alpha = [0, 0, 0, 0]

pourcentage = [40, 20, 20, 20]
quantity = [0, 0, 0, 0]
alpha[0] = 10.0
alpha[1] = 10.0
alpha[2] = 2.0
alpha[3] = 1.0

#Objects
lifetime = [15, 10, 20, 10]
pricetotal = [30000, 50000, 5000, 600]
capacity = [3, 1, 1, 15]
pricepermonth = [0, 0, 0, 0]


for n in range(10000):
    choice = [0, 0, 0, 0]

    revenue = max(np.random.normal(averageWage, 1500), 700)

    alpha[0] = max(np.random.normal(2000, 500), 0)
    alpha[1] = max(np.random.normal(3000, 1000), 0)
    alpha[2] = max(np.random.normal(1000, 800), 0) * np.sqrt(airQuality)
    alpha[3] = max(np.random.normal(1000, 100), 0)

    pricetotal[0] = max(np.random.normal(40000, 50), 0)
    pricetotal[1] = max(np.random.normal(200000, 100), 0)
    pricetotal[2] = max(np.random.normal(5000, 30), 0)
    pricetotal[3] = 4300

    sumAlp= sum(alpha)

    for i in range(len(alpha)):
        alpha[i] = 0.01 * alpha[i] / sumAlp
        pricepermonth[i] = pricetotal[i] / (12 * lifetime[i])

    for i in range(len(alpha)):
        pourcentage[i] = alpha[i] * ( revenue / pricepermonth[i] ) /sum(alpha)
        if pourcentage[i] > 1 : choice[i]= alpha[i]
        else : choice[i] = 0
    quantity[choice.index(max(choice))] += 1

for i in range(len(alpha)):    
    quantity[i] /= capacity[i]
    
print(quantity)

