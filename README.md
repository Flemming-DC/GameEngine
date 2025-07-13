# Introduction
This is a game engine with an editor and an example game. 

Each of these things are a C++ project and they fit together into a C++ solution. 
The editor and engine are liberies, while the game is an executable. 
The project is only tested for Windows and the Linux Subsystem for Windows. It is not tested on a genuine Linux machine nor on Mac. 

The Game has a macro named InEditor that controls whether it is running inside the editor or as a standalone executable. 

The dependencies are as follows
    The game depends in the engine and if InEditor is defined, then it also depends on the editor.
    The editor depends on the engine.

## get started
Run Setup-Windows.bat to build Engine, Game and Editor.
Choose the starting project to Game or Game-InEditor, depending on whether it should run as standalone or inn hte editor.

nb: Visual Studio will display Game and Game-InEditor as two seperate projects, but they actually use the same code, except for the macro flag InEditor.
