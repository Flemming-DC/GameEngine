#pragma once


class ImGuiSetup
{
public:
    static void Setup(bool inEditor);
    static void EarlyUpdate();
    static void LateUpdate();
    static void Shutdown();
};

