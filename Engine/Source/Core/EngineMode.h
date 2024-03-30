#pragma once
class Core;

class EngineMode
{
public:
    inline static bool GameIsRunning() { return gameIsRunning; }
    inline static bool InEditor() { return inEditor; }

    friend Core;
private:
    static bool gameIsRunning;
    static bool inEditor;
};


