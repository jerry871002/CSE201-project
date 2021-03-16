# Autocomplete on VScode

To use the autocomplete feature on VScode, there are some settings to do.

First open the `CSE201-project` folder with VScode, make sure you installed the `C/C++` extension

![](https://i.imgur.com/K7VtEjP.png)

Then press `shift`+`command`+`P` and search for `C/C++: Edit Configurations (JSON)`

![](https://i.imgur.com/61g6V5h.png)

You will end up opening the file `c_cpp_properties.json`, we have to set the `includePath` in order to let the editor help us on autocomplete. Also it's good to check if the `compilerPath` matches the outcome when executing the command `which gcc` on terminal. My setting is as below:

```json
{
    "configurations": [
        {
            "name": "Mac",
            "includePath": [
                "${workspaceFolder}/**",
                "climate-change/src/VS-Project/Libraries/godot-cpp-bindings/godot_headers",
                "climate-change/src/VS-Project/Libraries/godot-cpp-bindings/include",
                "climate-change/src/VS-Project/Libraries/godot-cpp-bindings/include/core",
                "climate-change/src/VS-Project/Libraries/godot-cpp-bindings/include/gen"
            ],
            "defines": [],
            "macFrameworkPath": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "gnu17",
            "cppStandard": "gnu++14",
            "intelliSenseMode": "${default}"
        }
    ],
    "version": 4
}
```
