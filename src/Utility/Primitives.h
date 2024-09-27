

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#define Array_Lenght(array) sizeof(array) / sizeof(array[0])

#define KiB 1024
#define MiB KiB * KiB

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef float f32;
typedef double f64;
typedef i32 b32;

constexpr f64 PI = 3.141592653589793;
constexpr f32 PI32 = f32(PI);
constexpr f32 HALF_PI32 = PI32 / 2;
constexpr f64 TAU = 6.283185307179586;
constexpr f32 TAU32 = f32(TAU);
constexpr u32 DEG_FULL_CIRCLE = 360;
constexpr u32 DEG_HALF_CIRCLE = DEG_FULL_CIRCLE / 2;
constexpr u32 X_AXIS = 0;
constexpr u32 Y_AXIS = 1;

// TODO: Define all mins (for signed types) and maxes.
constexpr f32 F32_MIN = 1.175494351e-38F;
constexpr f32 F32_MAX = 3.402823466e+38F;
constexpr f64 F64_MAX = 1.7976931348623157E+308;
constexpr u64 U64_MAX = 0xFFFFFFFFFFFFFFFF;
constexpr u32 U32_MAX = 0xFFFFFFFF;
constexpr i32 I32_MAX = 2147483647;
constexpr u16 U16_MAX = 0xFFFF;
constexpr u8 U8_MAX = 0xFF;
constexpr i8 I8_MAX = 127;


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


#include "Vector.h" // v2i, v2u, v2f 

struct Rect
{
    v2f min = {};
    v2f max = {};
};
