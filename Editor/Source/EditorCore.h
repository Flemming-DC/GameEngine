#pragma once
#include "Dynamic.h"

namespace Editor
{
    class EditorCore : public Dynamic
    {
    public:
        static void MarkAsEditor();

    private:
        void OnEditorStart() override;
        void OnEditorUpdate() override;
        void HandleOpenClose();
    };
}
