

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#define _inline __forceinline
#define Array_Lenght(array) (sizeof(array) / sizeof(array[0]))

#define KiB 1024
#define MiB KiB * KiB

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;
typedef s32 b32;


static_assert(sizeof(s8)  == sizeof(u8));
static_assert(sizeof(s16) == sizeof(u16));
static_assert(sizeof(s32) == sizeof(u32));
static_assert(sizeof(s64) == sizeof(u64));

static_assert(sizeof(s8)  == 8  / 8);
static_assert(sizeof(s16) == 16 / 8);
static_assert(sizeof(s32) == 32 / 8);
static_assert(sizeof(f32) == 32 / 8);
static_assert(sizeof(s64) == 64 / 8);
static_assert(sizeof(f64) == 64 / 8);

static_assert(sizeof(void*) == sizeof(u64), "This codebase assumes a 64 bit platform!");


constexpr f64 PI = 3.141592653589793;
constexpr f32 PI32 = f32(PI);
constexpr f32 HALF_PI32 = f32(PI / 2);
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
constexpr s32 S32_MAX = 2147483647;
constexpr u16 U16_MAX = 0xFFFF;
constexpr u8 U8_MAX = 0xFF;
constexpr s8 S8_MAX = 127;