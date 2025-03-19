

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

union Color
{
    u32 as_u32;
    
    struct
    {
        u8 blue;
        u8 green;
        u8 red;
        u8 alpha;
    };
    
    struct 
    {
        u8 b;
        u8 g;
        u8 r;
        u8 a;
    };
};


constexpr Color WHITE       = { 0xFFFFFFFF };
constexpr Color BLACK       = { 0xFF000000 };
constexpr Color GRAY        = { 0xFF808080 };
constexpr Color DARK_GRAY   = { 0xFF404040 };
constexpr Color RED         = { 0xFFFF0000 };
constexpr Color YELLOW      = { 0xFFFFFF00 };
constexpr Color GREEN       = { 0xFF00FF00 };
constexpr Color BLUE        = { 0xFF0000FF };
constexpr Color MAGENTA     = { 0xFFFF00FF };