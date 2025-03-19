

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

constexpr s32 s_controller_map[(u64)Button::BUTTON_COUNT] =
{
    0x0001,     //0
    0x0002,     //1
    0x0004,     //2
    0x0008,     //3
    0x0010,     //4
    0x0020,     //5
    0x0040,     //6
    0x0080,     //7
    0x0100,     //8
    0x0200,     //9
    0x1000,     //10
    0x2000,     //11
    0x4000,     //12
    0x8000      //13
};


constexpr s32 s_windows_keycode_map[(u64)Key_Code::COUNT] = 
{
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    VK_NUMPAD0,
    VK_NUMPAD1,
    VK_NUMPAD2,
    VK_NUMPAD3,
    VK_NUMPAD4,
    VK_NUMPAD5,
    VK_NUMPAD6,
    VK_NUMPAD7,
    VK_NUMPAD8,
    VK_NUMPAD9,
    VK_DIVIDE,
    VK_MULTIPLY,
    VK_ADD,
    VK_SUBTRACT,
    VK_DECIMAL,
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    VK_F1,
    VK_F2,
    VK_F3,
    VK_F4,
    VK_F5,
    VK_F6,
    VK_F7,
    VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,
    VK_LEFT,
    VK_RIGHT,
    VK_UP,
    VK_DOWN,
    VK_ESCAPE,
    VK_LCONTROL,
    VK_LSHIFT,
    VK_LMENU,
    VK_LWIN,
    VK_LBUTTON,
    VK_RCONTROL,
    VK_RSHIFT,
    VK_RMENU,
    VK_RWIN,
    VK_RBUTTON,
    VK_SPACE,
    VK_PRIOR,
    VK_NEXT,
    VK_END,
    VK_HOME,
    VK_INSERT,
    VK_DELETE,
    VK_BACK,
    VK_RETURN,
    VK_TAB
};