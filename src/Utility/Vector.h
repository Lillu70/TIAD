

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#include "Primitives.h"

template<typename T>
union v2
{
    struct
    {
        T x;
        T y;
    };
    
    T elements[2];
    
    template<typename U>
    static inline v2<U> Cast(v2<T> v)
    {
        v2<U> result = v2<U>{(U)v.x, (U)v.y};
        return result;
    }
    
    template<typename U>
    inline v2<U> As()
    {
        v2<U> result = v2<U>{U(x), U(y)};
        return result;
    }
};


typedef v2<f32> v2f;
typedef v2<u32> v2u;
typedef v2<i32> v2i;


template<typename T>
union v3
{
    struct
    {
        v2<T> xy;
        T z;
    };
    
    struct
    {
        T r;
        T g;
        T b;
    };
    
    struct
    {
        T x;
        T y;
        T z;
    };
    
    T elements[3];
    
    template<typename U>
    static inline v3<U> Cast(v3<T> v)
    {
        v3<U> result = v3<U>{(U)v.x, (U)v.y, (U)v.z};
        return result;
    }
    
    template<typename U>
    inline v2<U> As()
    {
        v2<U> result = v2<U>{U(x), U(y), U(z)};
        return result;
    }
};


typedef v3<f32> v3f;


template<typename T>
union v4
{
    struct
    {
        v3<T> xyz;
        T w;
    };
    
    struct
    {
        v3<T> rgb;
        T a;
    };
    
    struct
    {
        T r;
        T g;
        T b;
        T a;
    };
    
    struct
    {
        T x;
        T y;
        T z;
        T w;
    };
    
    T elements[4];
    
    template<typename U>
    static inline v4<U> Cast(v4<T> v)
    {
        v4<U> result = v4<U>{(U)v.x, (U)v.y, (U)v.z, (U)v.w};
        return result;
    }
    
    template<typename U>
    inline v2<U> As()
    {
        v2<U> result = v2<U>{U(x), U(y), U(z), U(w)};
        return result;
    }
};

typedef v4<f32> v4f;


template<typename T>
inline bool operator == (v2<T> a, v2<T> b)
{
    bool result = a.x == b.x && a.y == b.y;
    return result;
}


template<typename T>
inline bool operator != (v2<T> a, v2<T> b)
{
    bool result = !(a == b);
    return result;
}


template<typename T>
static inline bool operator == (v3<T> a, v3<T> b)
{
    bool result = a.x == b.x && a.y == b.y && a.z == b.z;
    return result;
}


template<typename T>
static inline bool operator != (v3<T> a, v3<T> b)
{
    bool result = !(a == b);
    return result;
}


template<typename T>
static inline bool operator == (v4<T> a, v4<T> b)
{
    bool result = a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
    return result;
}


template<typename T>
static inline bool operator != (v4<T> a, v4<T> b)
{
    bool result = !(a == b);
    return result;
}