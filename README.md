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
### Windows
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
12. Give it a name, select the Icon res://Icon.ico and export it with "Export project", unckeck "Export with debugs"
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
