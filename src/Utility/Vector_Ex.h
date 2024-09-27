

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#include "Vector.h"

// v2i ----------------------------------------------------------
static inline v2i operator + (v2i a, v2i b)
{
    v2i result = v2i{a.x + b.x, a.y + b.y};
    return result;
}


static inline v2i operator - (v2i a, v2i b)
{
    v2i result = v2i{a.x - b.x, a.y - b.y};
    return result;
}


// v2u ----------------------------------------------------------
static inline v2u operator + (v2u a, v2u b)
{
    v2u result = v2u{a.x + b.x, a.y + b.y};
    return result;
}


static inline v2u operator - (v2u a, v2u b)
{
    v2u result = v2u{a.x - b.x, a.y - b.y};
    return result;
}



// v2f
// --------------------------------------------------------------
static inline v2f operator * (v2f a, f32 scalar)
{
    v2f result = v2f{a.x * scalar, a.y * scalar};
    return result;
}


static inline v2f operator * (f32 scalar, v2f v)
{
    v2f result = v * scalar;
    return result;
}


static inline v2f operator / (v2f a, f32 scalar)
{
    v2f result = v2f{a.x / scalar, a.y / scalar};
    return result;
}


static inline v2f operator / (f32 scalar, v2f a)
{
    v2f result = v2f{scalar / a.x, scalar / a.y};
    return result;
}


static inline v2f operator + (v2f v, f32 scalar)
{
    v2f result = v2f{v.x + scalar, v.y + scalar};
    return result;
}


static inline v2f operator - (v2f v, f32 scalar)
{
    v2f result = v2f{v.x - scalar, v.y - scalar};
    return result;
}


static inline v2f operator + (v2f a, v2f b)
{
    v2f result = v2f{a.x + b.x, a.y + b.y};
    return result;
}


static inline v2f operator - (v2f a, v2f b)
{
    v2f result = v2f{a.x - b.x, a.y - b.y};
    return result;
}


static inline v2f operator - (f32 scalar, v2f v)
{
    v2f result = v2f{scalar - v.x, scalar - v.y};
    return result;
}


static inline v2f operator + (f32 scalar, v2f v)
{
    v2f result = v2f{scalar + v.x, scalar + v.y};
    return result;
}


static inline v2f operator - (v2f v)
{
    v2f result = v2f{-v.x, -v.y};
    return result;
}


static inline void operator += (v2f& a, v2f b)
{
    a = a + b;
}


static inline void operator -= (v2f& a, v2f b)
{
    a = a - b;
}


static inline void operator += (v2f& v, f32 scalar)
{
    v = v + scalar;
}


static inline void operator -= (v2f& v, f32 scalar)
{
    v = v - scalar;
}


static inline void operator *= (v2f& v, f32 scalar)
{
    v = v * scalar;
}


static inline void operator /= (v2f& v, f32 scalar)
{
    v = v / scalar;
}


// v3f
// --------------------------------------------------------------


static inline v3f operator * (v3f v, f32 scalar)
{
    v3f result = v3f{v.x * scalar, v.y * scalar, v.z * scalar};
    return result;
}


static inline v3f operator * (f32 scalar, v3f v)
{
    v3f result = v * scalar;
    return result;
}


static inline v3f operator / (v3f v, f32 scalar)
{
    v3f result = v3f{v.x / scalar, v.y / scalar, v.z / scalar};
    return result;
}


static inline v3f operator / (f32 scalar, v3f v)
{
    v3f result = v3f{scalar / v.x, scalar / v.y, scalar / v.z};
    return result;
}


static inline v3f operator - (f32 scalar, v3f v)
{
    v3f result = v3f{scalar - v.x, scalar - v.y, scalar - v.z};
    return result;
}


static inline v3f operator + (f32 scalar, v3f v)
{
    v3f result = v3f{scalar + v.x, scalar + v.y, scalar + v.z};
    return result;
}


static inline v3f operator + (v3f a, v3f b)
{
    v3f result = v3f{a.x + b.x, a.y + b.y, a.z + b.z};
    return result;
}


static inline v3f operator - (v3f a, v3f b)
{
    v3f result = v3f{a.x - b.x, a.y - b.y, a.z - b.z};
    return result;
}


static inline v3f operator - (v3f v)
{
    v3f result = v3f{-v.x, -v.y, -v.z};
    return result;
}


static inline v3f operator + (v3f v, f32 scalar)
{
    v3f result = v3f{v.x + scalar, v.y + scalar, v.z + scalar};
    return result;
}


static inline v3f operator - (v3f v, f32 scalar)
{
    v3f result = v3f{v.x - scalar, v.y - scalar, v.z - scalar};
    return result;
}


static inline void operator += (v3f& a, v3f b)
{
    a = a + b;
}


static inline void operator -= (v3f& a, v3f b)
{
    a = a - b;
}


static inline void operator += (v3f& v, f32 scalar)
{
    v = v + scalar;
}


static inline void operator -= (v3f& v, f32 scalar)
{
    v = v - scalar;
}


static inline void operator *= (v3f& v, f32 scalar)
{
    v = v * scalar;
}


static inline void operator /= (v3f& v, f32 scalar)
{
    v = v / scalar;
}


// v4f
// --------------------------------------------------------------


static inline v4f operator * (v4f v, f32 scalar)
{
    v4f result = v4f{v.x * scalar, v.y * scalar, v.z * scalar, v.w * scalar};
    return result;
}


static inline v4f operator * (f32 scalar, v4f v)
{
    v4f result = v * scalar;
    return result;
}


static inline v4f operator / (v4f v, f32 scalar)
{
    v4f result = v4f{v.x / scalar, v.y / scalar, v.z / scalar, v.w / scalar};
    return result;
}


static inline v4f operator / (f32 scalar, v4f v)
{
    v4f result = v4f{scalar / v.x, scalar / v.y, scalar / v.z, scalar / v.w};
    return result;
}


static inline v4f operator - (f32 scalar, v4f v)
{
    v4f result = v4f{scalar - v.x, scalar - v.y, scalar - v.z, scalar - v.w};
    return result;
}


static inline v4f operator + (f32 scalar, v4f v)
{
    v4f result = v4f{scalar + v.x, scalar + v.y, scalar + v.z, scalar + v.w};
    return result;
}


static inline v4f operator + (v4f a, v4f b)
{
    v4f result = v4f{a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return result;
}


static inline v4f operator - (v4f a, v4f b)
{
    v4f result = v4f{a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return result;
}


static inline v4f operator - (v4f v)
{
    v4f result = v4f{-v.x, -v.y, -v.z, -v.w};
    return result;
}


static inline v4f operator + (v4f v, f32 scalar)
{
    v4f result = v4f{v.x + scalar, v.y + scalar, v.z + scalar, v.w + scalar};
    return result;
}


static inline v4f operator - (v4f v, f32 scalar)
{
    v4f result = v4f{v.x - scalar, v.y - scalar, v.z - scalar, v.w - scalar};
    return result;
}


static inline void operator += (v4f& a, v4f b)
{
    a = a + b;
}


static inline void operator -= (v4f& a, v4f b)
{
    a = a - b;
}


static inline void operator += (v4f& v, f32 scalar)
{
    v = v + scalar;
}


static inline void operator -= (v4f& v, f32 scalar)
{
    v = v - scalar;
}


static inline void operator *= (v4f& v, f32 scalar)
{
    v = v * scalar;
}


static inline void operator /= (v4f& v, f32 scalar)
{
    v = v / scalar;
}
