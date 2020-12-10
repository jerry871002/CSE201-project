# Coding Style

## Naming

### Class
Start by a uppercase letter and use camel case
```cpp
class Structure;
class EnergyStructure;
```

### Variable
Start by a lowercase letter and use camel case
```cpp
double numberOfEmplyees; 
double carbonEmission;
double energyDemand;
double energySupply;
```

### Function
Start by a lowercase letter and use snake case
```cpp
void add_building(Structure*);
```

### Constant
Use uppercase letters
```cpp
const double PI = 3.14159;
```

## Indentation
* 4 spaces are used for indentation
* Spaces, not tabs!

## Braces
The opening brace goes on the same line
:::warning
We can discuss about this since I beleive Visual Studio's default is to put the opening brace on the next line
:::
```cpp
// do these
class Structure : public Node2D {
    
}

void add_building(Structure* building) {
    
}

// not these
class Structure : public Node2D 
{
    
}

void add_building(Structure* building) {

}
```
If the closing brace is followed by another keyword, it goes into the same line as well
```cpp
// do this
if (condition) {

} else {

}

// not this
if (condition) 
{

} 
else 
{

}
```

### Whitespace
* Use blank lines to group statements together where suited
* Always use only one blank line
* Always use a single space after a keyword and before a curly brace
* Leave a space after each comma (or semicolon)

```cpp
// do these
if (condition) {

}

for (int i = 0; i < size; i++) {

}

while (condition) {

}

// not these
if(condition){

}

for(int i=0;i<size;i++){

}

while(condition){

}
```
