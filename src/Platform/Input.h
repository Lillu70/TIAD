

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once


enum class Key_Code
{
    NONE = -1,
    NUM_0 = 0,
    NUM_1,
    NUM_2,
    NUM_3,
    NUM_4,
    NUM_5,
    NUM_6,
    NUM_7,
    NUM_8,
    NUM_9,
    NP_NUM_0,
    NP_NUM_1,
    NP_NUM_2,
    NP_NUM_3,
    NP_NUM_4,
    NP_NUM_5,
    NP_NUM_6,
    NP_NUM_7,
    NP_NUM_8,
    NP_NUM_9,
    NP_DIVIDE,
    NP_MULTIPLY,
    NP_ADD,
    NP_SUBTRACT,
    NP_DECIMAL,
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    ESC,
    LCTRL,
    LSHIFT,
    LALT,
    LSYS,
    MOUSE_LEFT,
    RCTRL,
    RSHIFT,
    RALT,
    RSYS,
    MOUSE_RIGHT,
    SPACE,
    PAGEUP,
    PAGEDOWN,
    END,
    HOME,
    INSERT,
    DEL,
    BACK,
    ENTER,
    TAB,
    COUNT
};


constexpr char* s_key_names[] = {
    "NUM 0",
    "NUM 1",
    "NUM 2",
    "NUM 3",
    "NUM 4",
    "NUM 5",
    "NUM 6",
    "NUM 7",
    "NUM 8",
    "NUM 9",
    "NP NUM_0",
    "NP NUM_1",
    "NP NUM_2",
    "NP NUM_3",
    "NP NUM_4",
    "NP NUM_5",
    "NP NUM_6",
    "NP NUM_7",
    "NP NUM_8",
    "NP NUM_9",
    "NP DIVIDE",
    "NP MULTIPLY",
    "NP ADD",
    "NP SUBTRACT",
    "NP DECIMAL",
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "LEFT",
    "RIGHT",
    "UP",
    "DOWN",
    "ESC",
    "LEFT CTRL",
    "LEFT SHIFT",
    "LEFT ALT",
    "LEFT SYS",
    "LEFT MOUSE",
    "RIGHT CTRL",
    "RIGHT SHIFT",
    "RIGHT ALT",
    "RIGHT SYS",
    "RIGHT MOUSE",
    "SPACE",
    "PAGE UP",
    "PAGE DOWN",
    "END",
    "HOME",
    "INSERT",
    "DEL",
    "BACK",
    "ENTER",
};


enum class Button
{
    NONE = -1,
    DPAD_UP = 0, 
    DPAD_DOWN,
    DPAD_LEFT,
    DPAD_RIGHT,
    START,
    BACK,
    L_THUMB,
    R_THUMB,
    L_SHLD,
    R_SHLD,
    BUT_A,
    BUT_B,
    BUT_X,
    BUT_Y, 
    BUTTON_COUNT
};


constexpr char* s_button_names[]
{
    "DPAD UP", 
    "DPAD DOWN",
    "DPAD LEFT",
    "DPAD RIGHT",
    "START",
    "BACK",
    "LEFT THUMB",
    "RIGHT THUMB",
    "LEFT SHLD",
    "RIGHT SHLD",
    "A",
    "B",
    "X",
    "Y"   
};

struct Button_State
{
    bool current = 0;
    bool previous = 0;

    inline bool Is_Down() { return current; }
    inline bool Is_Up() { return !current; }
    inline bool Is_Pressed() { return !previous && current; }
    inline bool Is_Released() { return previous && !current; }
};


struct Controller_State
{
    struct Data
    {
        u16 button_states = 0;
        f32 l_thumb_x = 0;
        f32 l_thumb_y = 0;
        f32 r_thumb_x = 0;
        f32 r_thumb_y = 0;
        f32 l_trig = 0;
        f32 r_trig = 0;
    };
    
    Controller_State::Data m_curr;
    Controller_State::Data m_prev;
    
    Button_State Get_Button_State(Button button)
    {
        s16 b = (s16)button;
        return Button_State{(m_curr.button_states & (1 << (int)b)) > 0, (m_prev.button_states & (1 << (int)b)) > 0};
    }
    
    bool Get_Button_Down(Button button)
    {
        return (m_curr.button_states & (1 << (int)button)) > 0;
    }
    
    static constexpr f32 s_default_dead_zone = 0.25f;
    
    #if 0
    bool get_left_stick_x(f32& output, f32 dead_zone = s_default_dead_zone)
    {
        if (abs(m_curr.l_thumb_x) < dead_zone) 
            return false;
        if (m_curr.l_thumb_x > 0)
            output = (m_curr.l_thumb_x - dead_zone) / (1.f - dead_zone);
        else
            output = (m_curr.l_thumb_x + dead_zone) / (-1.f + dead_zone) * -1;
        
        return true;
    }
    
    bool get_left_stick_y(f32& output, f32 dead_zone = s_default_dead_zone)
    {
        if (abs(m_curr.l_thumb_y) < dead_zone) 
            return false;
        if (m_curr.l_thumb_y > 0)
            output = (m_curr.l_thumb_y - dead_zone) / (1.f - dead_zone);
        else
            output = (m_curr.l_thumb_y + dead_zone) / (-1.f + dead_zone) * -1;
        return true;
    }
    
    bool get_right_stick_x(f32& output, f32 dead_zone = s_default_dead_zone)
    {
        if (abs(m_curr.r_thumb_x) < dead_zone) 
            return false;
        if (m_curr.r_thumb_x > 0)
            output = (m_curr.r_thumb_x - dead_zone) / (1.f - dead_zone);
        else
            output = (m_curr.r_thumb_x + dead_zone) / (-1.f + dead_zone) * -1;
        
        return true;
    }
    
    bool get_right_stick_y(f32& output, f32 dead_zone = s_default_dead_zone)
    {
        if (abs(m_curr.r_thumb_y) < dead_zone) 
            return false;
        
        if (m_curr.r_thumb_y > 0)
            output = (m_curr.r_thumb_y - dead_zone) / (1.f - dead_zone);
        else
            output = (m_curr.r_thumb_y + dead_zone) / (-1.f + dead_zone) * -1;
        
        return true;
    }
    #endif
};

