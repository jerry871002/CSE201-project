# CSE201-project

C++ project for understanding the link between human activities and social environmental effects.
Aim is for the user to have a better understanding of how certain environmental policies work and their impact on different objectives. 



## Members
| name surname | github username | trello username | [role] |
|:------------:|:---------------:|:---------------:|:------:|
| Vincent Sietses | Vincent-Sietses | vincentsietses | Project boss + Algorithms team leader|
| Antoine De Mascarel| antoine-demascarel | antoinedemascarel | Trello leader + Objects team leader|
| Jerry Yang| jerry871002| jerry871002| Git leader|
| Eliott Sarrey| Eliott-Sarrey | eliottsarrey | Interface team leader|
| Paloma Thevenet | tpaloma | palomathevenet |
| Ambroise Müller | ambroisemuller | ambroisemuller |
| Luca Bonengel | lucabonengel | lucabonengel |
| Yury Nevenchannyy | Yury-Nevencahnnyy | yurynevenchannyy |
| Samuel Sahel-Schackis | samsahsch | samuelsahelschackis |
| Saga David | sagdav | sagadavid ||
| Anna Vlasova | annavlasova19 | anya_vlasova ||
| Arthur Chambat | arthurchambat| arthurchambat ||

## Team assignments
### Interface
#### 3D world
- Eliott
- Ambroise
#### UI
- Luca
- Arthur
### Algorithms
- Vincent
- Jerry
- Yury
- Sam
### Objects
- Antoine
- Saga
- Anna
- Paloma



## How to compile and run our project


####
!!! DISCLAIMER MINIMUM REQUIREMENTS !!!

This game features a large amount of 3D Graphics and will not run on every computer.
During our programming, most teammembers were not able to run the full game themselves on their laptops.
Make sure you have a capable computer before going through the steps below.

Windows recommended specs:
- Dedicated GPU or i7-10700 integrated graphics or better 
- 16 GB RAM 

MacBook minimum specs:
- MacBook Pro Core i5
- 16 GB RAM


### Windows
Follow the steps on imgur : https://imgur.com/a/WZNLLwY

1. Install the standard version of Godot via [this page](https://godotengine.org/download)
2. Install the IDE VS Community from this page [this page](https://visualstudio.microsoft.com/fr/downloads/)
3. Make sure to select "Desktop development with C++" and finish the set up of VS
4. Download the complete github repository 
5. Open the project solution from VS in CSE201-project/climate-change/src/VS-Project/VS-Project.sln
6. Rebuild the solution in release mode x64
7. Open godot and import the project from CSE201-project/climate-change/
8. Once the project is opened in the Godot engine, click `Project > Export` on the top left corner
9. Add a new configuration for Windows Desktop
10. Manage the export templates
11. Download the templates
12. Give it a name, select the Icon res://Icon.ico and export it with "Export project", uncheck "Export with debugs"
13. Save it in any place you want, by double clicking on the dmg file, you will see the game inside it
14. Enjoy the game :)

### Mac
1. Install the standard version of Godot via [this page](https://godotengine.org/download)
2. Install the building tool `scons` via Homebrew
    ```
    brew install scons
    ```
3. Open a terminal in our project's base directory
4. Type the command `cd climate-change/src` to move to the directory that contains `SConstruct` (You can check this by the command `ls`)
5. Type `scons platform=osx target=release` to start building the project
6. Go back to the `climate-change/` folder and open `project.godot` by double clicking it
7. Once the project is opened in the Godot engine, click `Project > Export` on the top left corner
![](https://i.imgur.com/BNQKxJV.png)
8. Add a new configuration for Mac OSX
![](https://i.imgur.com/ILaHGf3.png)
9. Manage the export templates
![](https://i.imgur.com/XUVQS0r.png)
10. Download the templates
![](https://i.imgur.com/Kizhovi.png)
![](https://i.imgur.com/RUk9qGy.png)
11. Give it a name and export it
![](https://i.imgur.com/WhzN7eD.png)
12. Save it in any place you want, by double clicking on the dmg file, you will see the game inside it
13. Enjoy the game :)


## How to play the sim

General way of navigating the game:

Use the arrow keys to move around the city. Click on "Q" to zoom out and on "E" to zoom in. Using the mouse, if you hold down "Shift or space"  and your trackpad you can move around the city that way. This can also be done by holding the center click on an external mouse. The escape key is used to quit the game.

At the top of the game, you will see a firstly the exit and restart button. The date is display at the top left corner, and the slider gives you the ability to increase or decrease time in the game. Laslty, there is a option to click on the graphs at the top right. 

There are serveral tabs to display different graphs. You can hover over the data points and will get more information. Some of the graphs have different lines representing different sectors of the city. 

Laslty, at the very right is a car icon. Click on it to get a better understanding of transport in the city. On the left you will see how many of the different type of transports there are. The circles represents different policies that can be implemented specific to transport.

In the game, clicking on a specific object will lead to an info box to appear about this object. Furthermore different policies associated to the specific sector will be displayed. At any time if you do want to go back to the city / leave the info box, hold "shift" and click on the blurred city. 

For example: clicking on a mall, will give you the specific stats of that particular mall. It will also give you the possibility to implement policies for the entirety of the shop branch (mall, small shop and restaurants). One example is providing subsidies to solar panels. Enter a value between 0 and 450, and you will see that over time much more solar panels will appear on the different shops in the city. 

The indexes of the city are displayed on the right side for you to know what is happening in the entire city.

Have fun with EcoVille, we hope that you find the experience rewarding. 


