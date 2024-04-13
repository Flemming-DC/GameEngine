#pragma once
class Core;

class EngineMode
{
public:
    // nb: the editor might be running even if the game doesn't
    inline static bool GameIsRunning() { return gameIsRunning; }
    inline static bool InEditor() { return inEditor; }
    inline static void ExitEditor() { exitEditorhasBeenCalled = true; }
    static void MarkAsEditor();

    friend Core;
private:
    static bool gameIsRunning;
    static bool inEditor;
    static bool exitEditorhasBeenCalled;

    static bool ShouldClose() { return (!inEditor && !gameIsRunning) || (inEditor && exitEditorhasBeenCalled); }
    static void MarkGameCloseButtonAsUnclicked();
    static bool GameCloseButtonIsClicked(); // its the gameWindow, not the editor
};


