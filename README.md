# Introduction
This is a game engine with an editor and an example game. 
Each of these three items are a C++ project and they fit together into a C++ solution. 
The Game has a macro named InEditor that controls whether it is running inside the editor or as a standalone executable. 

The dependencies are as follows
    Game depends in Engine and if InEditor is defined, then it also depends on Editor.
    Editor depends on Engine.
    Engine doesn't depend on the other projects.

Editor and Engine are liberies, while Game is an executable. 
The project is only tested for Windows and the Linux Subsystem for Windows. It is not tested on a genuine Linux machine nor on Mac. 

## get started
Run Setup-Windows.bat to build Engine, Game and Editor.
Choose the starting project to Game or Game-InEditor, depending on whether it should run as standalone or inn hte editor.

nb: Visual Studio will display Game and Game-InEditor as two seperate projects, but they actually use the same code, except for the macro flag InEditor.
