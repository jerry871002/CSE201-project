# Changing from Unreal to Godot
> [time=Nov 14, 2020]

Since most of us are facing troubles with unreal engine and it will be unconvenient to have to work in the labs or in the lore office.

Hence, we decided to change for Godot, which is really light and will work well on any laptop.

## Installation

Download from the link below (the standard version), just unzip it and it's done, shouldn't be hard!
* [Windows](https://godotengine.org/download/windows)
* [macOS](https://godotengine.org/download/osx)
    > You might face some security issues here, just go to `setting > security and privacy > general` and you can fix this

Make sure you install Godot 3.1 or above!

## Get Familiar with Godot

I recommend you to watch the tutorials on their official documentation first. The most useful ones for me are listed below.
* [Your first game](https://docs.godotengine.org/en/stable/getting_started/step_by_step/your_first_game.html)
    > This one is actually done with GDScript, which we will not use much in our project. But it's a good starting point of understanding how Godot works. 
* [GDNative C example](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/gdnative-c-example.html)
* [GDNative C++ example](https://docs.godotengine.org/en/stable/tutorials/plugins/gdnative/gdnative-cpp-example.html)

After going through those tutorials, the two playlist on YouTube listed below can also be good resources.
* [Using C++ in GODOT Engine](https://www.youtube.com/playlist?list=PLZ2NyOcFnfQOko5ZysZeGkH3bfw2UtbOK)
* [Godot 3.1 C++ Maze Tutorial](https://www.youtube.com/playlist?list=PLWc2hibTd7kq_r0LYLCtMcWL9dd8jByRW)

## File Structure of Our Project

After going through these tutorials, I propose the structure below, which I beleive will make our project well-organised and easy to work on. Feel free to tell me if you have any idea about how we should work or how our project should be structured. 

```
.
+-- godot-cpp/          # clone this from GitHub
+-- godot-project/
|   +-- bin/
|   |   +-- windows/    # store .dll files here
|   |   +-- macos/      # store .dylib files here
|   |   +-- <others>    # .gdnlib and .gdns files
|   +-- scene/         # store .tscn files here
|   +-- img/            # store image files here
|   +-- project.godot
+-- SConstruct          # instructions of how the source code should be compiled
+-- src/                # store .cpp and .h files we wrote here
+-- reports/            # store our weekly report here
+-- doc/                # store documentations here
```

We will be working mostly on `src/` folder, where we will store all our codes. If you want to view the result on Godot, there are some compiling and linking jobs to do, which we will discuss later.

The second part we will obviously work on is the `reports/` folder, where we will submit our weekly report. I am not sure should we also upload our individual report here? If so, I will create another folder for this.

And lastly the `doc/` folder, if you just finished something and would like to share it with your teammate (like this document!), writing documents will be a good way. Since we would easily forget what we said on the chat, leaving a document would be convinient to search for past work.