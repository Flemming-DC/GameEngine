#include "AutoCompletor.h"
#include "ImGuiTools.h"
#include "imgui/imgui_stdlib.h"
#include "ShortHands.h"
#include "logger.h" // temp
#include "InputKey.h" 
Shorts;

namespace ImGui
{
    static vector<string> uninitialized; // this is only used to provide completionOptions with a invalid startup value.
    static vector<string>& completionOptions = uninitialized;
    static vector<string> candidates;
    static int selected = -1;
    static bool openPopup = false;

    static int _CompareFirstNCharecters(const char* s1, const char* s2, int n)
    {
        //compare two strings in a case-insensitive manner up to a specified number of characters.
        int d = 0;
        while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1)
        {
            s1++;
            s2++;
            n--;
        }
        return d;
    }

    static void _UpdateCandidates(string* strInput)
    {
        const char* word_start = strInput->c_str();
        int word_length = strInput->size();
        // Build a list of candidates
        candidates.clear();
        for (int i = 0; i < (int)completionOptions.size(); i++)
            if (_CompareFirstNCharecters(completionOptions[i].c_str(), word_start, word_length) == 0)
                candidates.push_back(completionOptions[i]);
    }


    static void _CompletionCallback(ImGuiInputTextCallbackData* data)
    {
        const char* word_start = data->Buf;
        int word_length = data->CursorPos;
        const char* word_end = word_start + word_length;

        if (candidates.size() == 0) // no match
        {
            P("No match for \"%.*s\"!\n", word_length, word_start);
        }
        else if (candidates.size() == 1) // Single match.
        {
            // Delete the beginning of the word and replace it entirely so we've got nice casing.
            data->DeleteChars(0, word_length);
            data->InsertChars(data->CursorPos, candidates[0].c_str());
        }
        else // Multiple matches. Complete as much as we can.
        {
            // find the length for which all_candidates_matches
            int match_len = word_length;
            for (;;)
            {
                int c = 0;
                bool all_candidates_matches = true;
                for (int i = 0; i < (int)candidates.size() && all_candidates_matches; i++)
                    if (i == 0)
                        c = toupper(candidates[i][match_len]);
                    else if (c == 0 || c != toupper(candidates[i][match_len]))
                        all_candidates_matches = false;
                if (!all_candidates_matches)
                    break;
                match_len++;
            }

            if (match_len > 0)
            {
                data->DeleteChars(0, word_length);
                data->InsertChars(data->CursorPos, candidates[0].c_str(), candidates[0].c_str() + match_len);
            }


            // List matches
            P("Possible matches:\n");
            for (int i = 0; i < (int)candidates.size(); i++)
                P("- %s\n", candidates[i]);
        }

    }


    static int _TextEditCallback(ImGuiInputTextCallbackData* data)
    {
        switch (data->EventFlag)
        {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            _CompletionCallback(data);
            break;
        }
        }
        return 0;
    }

    bool CompletionPopup(string* strInput)
    {
        bool hasChosen = false;
        ImGui::OpenPopup("completion_popup");
        if (ImGui::BeginPopup("completion_popup"))
        {
            for (int i = 0; i < (int)candidates.size(); i++)
            {
                bool isSelected = (selected == i);
                if (ImGui::Selectable(candidates[i].c_str()))
                {
                    *strInput = candidates[i];
                    candidates.clear();
                    hasChosen = true;
                }
            }
            ImGui::EndPopup();
        }
        return hasChosen;
    }

    bool AutoCompletor(const char* label, string* strInput, vector<string>& completionOptions_)
    {
        completionOptions = completionOptions_;
        _UpdateCandidates(strInput);
        ImGui::InputText(label, strInput, ImGuiInputTextFlags_CallbackCompletion, _TextEditCallback);

        if (*strInput != "")
            ImGui::LabelText("matches", logger::make_string(candidates).c_str());
        else
            ImGui::LabelText("", "");
        
        static bool hasChosen = false;
        if (candidates.size() == 1 && *strInput == candidates[0])
            hasChosen = true;
        else
        {
            if (InputKey::BecomesPressed(Key::Keyboard::tab))
                openPopup = true;
            if (openPopup)
                hasChosen = CompletionPopup(strInput);
        }
        if (hasChosen)
            openPopup = false;
        
        return hasChosen;
    }
}

