#include "KeyMap.h"
#include "ErrorChecker.h"

using namespace Key;
using std::string;

// nb: you can in fact support left/right ctrl distinction via
// ImGuiKey_LeftCtrl, ImGuiKey_RightCtrl, GLFW_KEY_LEFT_CONTROL, GLFW_KEY_RIGHT_CONTROL if you so want

// --------------- Glfw --------------- //

int KeyMap::ToGlfw(Keyboard key)
{
    switch (key)
    {
    case Keyboard::exc: return GLFW_KEY_ESCAPE;
    case Keyboard::_1: return GLFW_KEY_1;
    case Keyboard::_2: return GLFW_KEY_2;
    case Keyboard::_3: return GLFW_KEY_3;
    case Keyboard::_4: return GLFW_KEY_4;
    case Keyboard::_5: return GLFW_KEY_5;
    case Keyboard::_6: return GLFW_KEY_6;
    case Keyboard::_7: return GLFW_KEY_7;
    case Keyboard::_8: return GLFW_KEY_8;
    case Keyboard::_9: return GLFW_KEY_9;
    case Keyboard::_0: return GLFW_KEY_0;
    //case Keyboard::plus: return ;
    case Keyboard::apostrophe: return GLFW_KEY_APOSTROPHE;
    case Keyboard::back: return GLFW_KEY_BACKSPACE;
    case Keyboard::insert: return GLFW_KEY_INSERT;
    case Keyboard::home: return GLFW_KEY_HOME;
    case Keyboard::pageup: return GLFW_KEY_PAGE_UP;
    case Keyboard::num: return GLFW_KEY_NUM_LOCK;
    case Keyboard::numpad_div: return GLFW_KEY_KP_DIVIDE;
    case Keyboard::numpad_mul: return GLFW_KEY_KP_MULTIPLY;
    case Keyboard::numpad_minus: return GLFW_KEY_KP_SUBTRACT;
    case Keyboard::Q: return GLFW_KEY_Q;
    case Keyboard::W: return GLFW_KEY_W;
    case Keyboard::E: return GLFW_KEY_E;
    case Keyboard::R: return GLFW_KEY_R;
    case Keyboard::T: return GLFW_KEY_T;
    case Keyboard::Y: return GLFW_KEY_Y;
    case Keyboard::U: return GLFW_KEY_U;
    case Keyboard::I: return GLFW_KEY_I;
    case Keyboard::O: return GLFW_KEY_O;
    case Keyboard::P: return GLFW_KEY_P;
    //case Keyboard::hat: return ;
    case Keyboard::end: return GLFW_KEY_END;
    case Keyboard::pagedown: return GLFW_KEY_PAGE_DOWN;
    case Keyboard::numpad_home: return GLFW_KEY_KP_7;
    case Keyboard::numpad_7: return GLFW_KEY_KP_7;
    case Keyboard::numpad_8: return GLFW_KEY_KP_8;
    case Keyboard::numpad_9: return GLFW_KEY_KP_9;
    case Keyboard::numpad_plus: return GLFW_KEY_KP_ADD;
    case Keyboard::A: return GLFW_KEY_A;
    case Keyboard::S: return GLFW_KEY_S;
    case Keyboard::D: return GLFW_KEY_D;
    case Keyboard::F: return GLFW_KEY_F;
    case Keyboard::G: return GLFW_KEY_G;
    case Keyboard::H: return GLFW_KEY_H;
    case Keyboard::J: return GLFW_KEY_J;
    case Keyboard::K: return GLFW_KEY_K;
    case Keyboard::L: return GLFW_KEY_L;
    case Keyboard::pling: return GLFW_KEY_APOSTROPHE;
    case Keyboard::numpad_4: return GLFW_KEY_KP_4;
    case Keyboard::numpad_5: return GLFW_KEY_KP_5;
    case Keyboard::numpad_6: return GLFW_KEY_KP_6;
    //case Keyboard::lessThan: return GLFW_KEY_COMMA;
    case Keyboard::Z: return GLFW_KEY_Z;
    case Keyboard::X: return GLFW_KEY_X;
    case Keyboard::C: return GLFW_KEY_C;
    case Keyboard::V: return GLFW_KEY_V;
    case Keyboard::B: return GLFW_KEY_B;
    case Keyboard::N: return GLFW_KEY_N;
    case Keyboard::M: return GLFW_KEY_M;
    case Keyboard::comma: return GLFW_KEY_COMMA;
    case Keyboard::period: return GLFW_KEY_PERIOD;
    case Keyboard::dash: return GLFW_KEY_MINUS;
    case Keyboard::leftShift: return GLFW_KEY_LEFT_SHIFT;
    case Keyboard::rightShift: return GLFW_KEY_RIGHT_SHIFT;
    case Keyboard::upArrow: return GLFW_KEY_UP;
    case Keyboard::numpad_1: return GLFW_KEY_KP_1;
    case Keyboard::numpad_2: return GLFW_KEY_KP_2;
    case Keyboard::numpad_3: return GLFW_KEY_KP_3;
    case Keyboard::numpad_enter: return GLFW_KEY_KP_ENTER;
    case Keyboard::alt: return GLFW_KEY_LEFT_ALT;
    case Keyboard::space: return GLFW_KEY_SPACE;
    case Keyboard::altgr: return GLFW_KEY_RIGHT_ALT;
    case Keyboard::leftArrow: return GLFW_KEY_LEFT;
    case Keyboard::downArrow: return GLFW_KEY_DOWN;
    case Keyboard::rightArrow: return GLFW_KEY_RIGHT;
    case Keyboard::numpad_0: return GLFW_KEY_KP_0;
    //case Keyboard::numpad_del: return GLFW_KEY_KP_DECIMAL;
    case Keyboard::ctrl: 
        // RaiseError("KeyMap::ToGlfw() returns GLFW_KEY_LEFT_CONTROL, which ignores GLFW_KEY_RIGHT_CONTROL");
        return GLFW_KEY_LEFT_CONTROL; //GLFW_KEY_LEFT_CONTROL || GLFW_KEY_RIGHT_CONTROL;
    default:
        RaiseError("Missing key ", key);
        return -1; // dummy return value
    }
}

int KeyMap::ToGlfw(Mouse key)
{
    switch (key)
    {
    case Mouse::left: return GLFW_MOUSE_BUTTON_LEFT;
    case Mouse::middle: return GLFW_MOUSE_BUTTON_MIDDLE;
    case Mouse::right: return GLFW_MOUSE_BUTTON_RIGHT;
    default:
        RaiseError("Missing key ", key);
        return -1; // dummy return value
    }
}

int KeyMap::ToGlfw(Gamepad key)
{
    switch (key)
    {
    case Gamepad::A: return GLFW_GAMEPAD_BUTTON_A;
    case Gamepad::B: return GLFW_GAMEPAD_BUTTON_B;
    case Gamepad::X: return GLFW_GAMEPAD_BUTTON_X;
    case Gamepad::Y: return GLFW_GAMEPAD_BUTTON_Y;
    case Gamepad::leftBumper: return GLFW_GAMEPAD_BUTTON_LEFT_BUMPER;
    case Gamepad::rightBumper: return GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER;
    case Gamepad::back: return GLFW_GAMEPAD_BUTTON_BACK;
    case Gamepad::start: return GLFW_GAMEPAD_BUTTON_START;
    case Gamepad::guide: return GLFW_GAMEPAD_BUTTON_GUIDE;
    case Gamepad::leftStickPress: return GLFW_GAMEPAD_BUTTON_LEFT_THUMB;
    case Gamepad::rightStickPress: return GLFW_GAMEPAD_BUTTON_RIGHT_THUMB;
    case Gamepad::dpad_up: return GLFW_GAMEPAD_BUTTON_DPAD_UP;
    case Gamepad::dpad_right: return GLFW_GAMEPAD_BUTTON_DPAD_RIGHT;
    case Gamepad::dpad_down: return GLFW_GAMEPAD_BUTTON_DPAD_DOWN;
    case Gamepad::dpad_left: return GLFW_GAMEPAD_BUTTON_DPAD_LEFT;
    default:
        RaiseError("Missing key ", key);
        return -1; // dummy return value
    }
}
int KeyMap::ToGlfw(FloatKey key)
{
    switch (key)
    {
    case FloatKey::leftStick_x: return GLFW_GAMEPAD_AXIS_LEFT_X;
    case FloatKey::leftStick_y: return GLFW_GAMEPAD_AXIS_LEFT_Y;
    case FloatKey::rightStick_x: return GLFW_GAMEPAD_AXIS_RIGHT_X;
    case FloatKey::rightStick_y: return GLFW_GAMEPAD_AXIS_RIGHT_Y;
    case FloatKey::leftTrigger: return GLFW_GAMEPAD_AXIS_LEFT_TRIGGER;
    case FloatKey::rightTrigger: return GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER;
    default:
        RaiseError("Missing key ", key);
        return -1; // dummy return value
    }
}


// --------------- ImGUI --------------- //



ImGuiKey KeyMap::ToImGui(Keyboard key)
{
    switch (key)
    {
    case Keyboard::exc: return ImGuiKey_Escape;
    case Keyboard::_1: return ImGuiKey_1;
    case Keyboard::_2: return ImGuiKey_2;
    case Keyboard::_3: return ImGuiKey_3;
    case Keyboard::_4: return ImGuiKey_4;
    case Keyboard::_5: return ImGuiKey_5;
    case Keyboard::_6: return ImGuiKey_6;
    case Keyboard::_7: return ImGuiKey_7;
    case Keyboard::_8: return ImGuiKey_8;
    case Keyboard::_9: return ImGuiKey_9;
    case Keyboard::_0: return ImGuiKey_0;
    case Keyboard::F1: return ImGuiKey_F1;
    case Keyboard::F2: return ImGuiKey_F2;
    case Keyboard::F3: return ImGuiKey_F3;
    case Keyboard::F4: return ImGuiKey_F4;
    case Keyboard::F5: return ImGuiKey_F5;
    case Keyboard::F6: return ImGuiKey_F6;
    case Keyboard::F7: return ImGuiKey_F7;
    case Keyboard::F8: return ImGuiKey_F8;
    case Keyboard::F9: return ImGuiKey_F9;
    case Keyboard::F10: return ImGuiKey_F10;
    case Keyboard::F11: return ImGuiKey_F11;
    case Keyboard::F12: return ImGuiKey_F12;
    case Keyboard::printScreen: return ImGuiKey_PrintScreen;
    case Keyboard::keyboradScroll: return ImGuiKey_ScrollLock;
    case Keyboard::pause: return ImGuiKey_Pause;
    //case Keyboard::half: return 
    // case Keyboard::plus: return 
    case Keyboard::apostrophe: return ImGuiKey_Apostrophe;
    case Keyboard::back: return ImGuiKey_Backspace;
    case Keyboard::insert: return ImGuiKey_Insert;
    case Keyboard::home: return ImGuiKey_Home;
    case Keyboard::pageup: return ImGuiKey_PageUp;
    case Keyboard::num: return ImGuiKey_NumLock;
    case Keyboard::numpad_div: return ImGuiKey_KeypadDivide;
    case Keyboard::numpad_mul: return ImGuiKey_KeypadMultiply;
    case Keyboard::numpad_minus: return ImGuiKey_KeypadSubtract;
    case Keyboard::tab: return ImGuiKey_Tab;
    case Keyboard::Q: return ImGuiKey_Q;
    case Keyboard::W: return ImGuiKey_W;
    case Keyboard::E: return ImGuiKey_E;
    case Keyboard::R: return ImGuiKey_R;
    case Keyboard::T: return ImGuiKey_T;
    case Keyboard::Y: return ImGuiKey_Y;
    case Keyboard::U: return ImGuiKey_U;
    case Keyboard::I: return ImGuiKey_I;
    case Keyboard::O: return ImGuiKey_O;
    case Keyboard::P: return ImGuiKey_P;
    // case Keyboard::hat: return 
    case Keyboard::enter: return ImGuiKey_Enter;
    case Keyboard::_delete: return ImGuiKey_Delete;
    case Keyboard::end: return ImGuiKey_End;
    case Keyboard::pagedown: return ImGuiKey_PageDown;
    case Keyboard::numpad_home: return ImGuiKey_Home;
    case Keyboard::numpad_7: return ImGuiKey_Keypad7;
    case Keyboard::numpad_8: return ImGuiKey_Keypad8;
    case Keyboard::numpad_9: return ImGuiKey_Keypad9;
    case Keyboard::numpad_plus: return ImGuiKey_KeypadAdd;
    case Keyboard::capslock: return ImGuiKey_CapsLock; //
    case Keyboard::A: return ImGuiKey_A;
    case Keyboard::S: return ImGuiKey_S;
    case Keyboard::D: return ImGuiKey_D;
    case Keyboard::F: return ImGuiKey_F;
    case Keyboard::G: return ImGuiKey_G;
    case Keyboard::H: return ImGuiKey_H;
    case Keyboard::J: return ImGuiKey_J;
    case Keyboard::K: return ImGuiKey_K;
    case Keyboard::L: return ImGuiKey_L;
    case Keyboard::pling: return ImGuiKey_Apostrophe;
    case Keyboard::numpad_4: return ImGuiKey_Keypad4;
    case Keyboard::numpad_5: return ImGuiKey_Keypad5;
    case Keyboard::numpad_6: return ImGuiKey_Keypad6;
    case Keyboard::leftShift: return ImGuiKey_LeftShift;
    // case Keyboard::lessThan: return 
    case Keyboard::Z: return ImGuiKey_Z;
    case Keyboard::X: return ImGuiKey_X;
    case Keyboard::C: return ImGuiKey_C;
    case Keyboard::V: return ImGuiKey_V;
    case Keyboard::B: return ImGuiKey_B;
    case Keyboard::N: return ImGuiKey_N;
    case Keyboard::M: return ImGuiKey_M;
    case Keyboard::comma: return ImGuiKey_Comma;
    case Keyboard::period: return ImGuiKey_Period;
    case Keyboard::dash: return ImGuiKey_Minus;
    case Keyboard::rightShift: return ImGuiKey_RightShift;
    case Keyboard::upArrow: return ImGuiKey_UpArrow;
    case Keyboard::numpad_1: return ImGuiKey_Keypad1;
    case Keyboard::numpad_2: return ImGuiKey_Keypad2;
    case Keyboard::numpad_3: return ImGuiKey_Keypad3;
    case Keyboard::numpad_enter: return ImGuiKey_KeypadEnter;
    case Keyboard::ctrl: return ImGuiMod_Ctrl;
    case Keyboard::alt: return ImGuiKey_LeftAlt;
    case Keyboard::space: return ImGuiKey_Space;
    case Keyboard::altgr: return ImGuiKey_RightAlt;
    case Keyboard::leftArrow: return ImGuiKey_LeftArrow;
    case Keyboard::downArrow: return ImGuiKey_DownArrow;
    case Keyboard::rightArrow: return ImGuiKey_RightArrow;
    case Keyboard::numpad_0: return ImGuiKey_Keypad0;
    // case Keyboard::numpad_del: return ImGuiKey_KeypadDecimal;

    default:
        RaiseError("Missing key ", key);
        return ImGuiKey_COUNT; // dummy return value
    }               
}

ImGuiKey KeyMap::ToImGui(Mouse key)
{
    switch (key)
    {
    case Mouse::left: return ImGuiKey_MouseLeft;
    case Mouse::middle: return ImGuiKey_MouseMiddle;
    case Mouse::right: return ImGuiKey_MouseRight;
    default:
        RaiseError("Missing key ", key);
        return ImGuiKey_COUNT; // dummy return value
    }
}

ImGuiKey KeyMap::ToImGui(Gamepad key)
{
    switch (key)
    {
    case Gamepad::A: return ImGuiKey_GamepadFaceDown;
    case Gamepad::B: return ImGuiKey_GamepadFaceRight;
    case Gamepad::X: return ImGuiKey_GamepadFaceLeft;
    case Gamepad::Y: return ImGuiKey_GamepadFaceUp;
    case Gamepad::leftBumper: return ImGuiKey_GamepadL1;
    case Gamepad::rightBumper: return ImGuiKey_GamepadR1;
    case Gamepad::back: return ImGuiKey_GamepadBack;
    case Gamepad::start: return ImGuiKey_GamepadStart;
    // case Gamepad::guide: return GLFW_GAMEPAD_BUTTON_GUIDE;
    case Gamepad::leftStickPress: return ImGuiKey_GamepadL3;
    case Gamepad::rightStickPress: return ImGuiKey_GamepadR3;
    case Gamepad::dpad_up: return ImGuiKey_GamepadDpadUp;
    case Gamepad::dpad_right: return ImGuiKey_GamepadDpadRight;
    case Gamepad::dpad_down: return ImGuiKey_GamepadDpadDown;
    case Gamepad::dpad_left: return ImGuiKey_GamepadDpadLeft;
    default:
        RaiseError("Missing key ", key);
        return ImGuiKey_COUNT; // dummy return value
    }
}

ImGuiKey KeyMap::ToImGui(FloatKey key)
{
    switch (key)
    {
    // case FloatKey::leftStick_x: return ImGuiKey_GamepadLStickLeft, ImGuiKey_GamepadLStickRight;
    // case FloatKey::leftStick_y: return ImGuiKey_GamepadLStickDown, ImGuiKey_GamepadLStickUp;
    // case FloatKey::rightStick_x: return ImGuiKey_GamepadRStickLeft, ImGuiKey_GamepadRStickRight;
    // case FloatKey::rightStick_y: return ImGuiKey_GamepadRStickDown, ImGuiKey_GamepadRStickUp;
    case FloatKey::leftTrigger: return ImGuiKey_GamepadL2;
    case FloatKey::rightTrigger: return ImGuiKey_GamepadR2;
    default:
        RaiseError("Missing key ", key);
        return ImGuiKey_COUNT; // dummy return value
    }
}


// -------------- ToString ---------------

namespace logger
{
    string to_string(Keyboard key) { return string("Keyboard") + ImGui::GetKeyName(KeyMap::ToImGui(key)); }
    string to_string(Mouse key) { return ImGui::GetKeyName(KeyMap::ToImGui(key)); }
    string to_string(Gamepad key)
    {
        if (key == Gamepad::guide)
            return "GamepadGuide";
        else
            return ImGui::GetKeyName(KeyMap::ToImGui(key));
    }
    string to_string(FloatKey key)
    {
        switch (key)
        {
        case FloatKey::leftStick_x:  return "FloatKeyLeftStick_x";
        case FloatKey::leftStick_y:  return "FloatKeyLeftStick_y";
        case FloatKey::rightStick_x: return "FloatKeyRightStick_x";
        case FloatKey::rightStick_y: return "FloatKeyRightStick_y";
        case FloatKey::leftTrigger:  return "FloatKeyLeftTrigger";
        case FloatKey::rightTrigger: return "FloatKeyRightTrigger";;
        default:
            RaiseError("Missing key ", key);
            return ""; // dummy return value
        }
    }
    string to_string(VectorKey key)
    {
        switch (key)
        {
        case VectorKey::leftStick: return "VectorKeyLeftStick";
        case VectorKey::rightStick: return "VectorKeyRightStick";
        case VectorKey::dpad: return "VectorKeyDpad";
        case VectorKey::WASD: return "VectorKeyWASD";
        case VectorKey::Arrows: return "VectorKeyArrows";
        case VectorKey::MouseEntireScreenPosition: return "VectorKeyMouseScreenPosition";;
        case VectorKey::MouseGameScreenPosition: return "VectorKeyMouseScreenPosition";;
        case VectorKey::mouseWorldPosition2D: return "VectorKeyMouseWorldPosition2D";;
        default:
            RaiseError("Missing key ", key);
            return ""; // dummy return value
        }
    }

}