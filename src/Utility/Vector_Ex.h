

#pragma once


// v2s ----------------------------------------------------------
static inline v2s operator + (v2s a, v2s b)
{
    v2s result = v2s{a.x + b.x, a.y + b.y};
    return result;
}


static inline v2s operator - (v2s a, v2s b)
{
    v2s result = v2s{a.x - b.x, a.y - b.y};
    return result;
}


static inline v2s operator + (v2s a, s32 i)
{
    v2s result = v2s{a.x + i, a.y + i};
    return result;
}


static inline v2s operator - (v2s a, s32 i)
{
    v2s result = v2s{a.x - i, a.y - i};
    return result;
}


static inline v2s operator * (v2s a, s32 i)
{
    v2s result = v2s{a.x * i, a.y * i};
    return result;
}


static inline v2s operator / (v2s a, s32 i)
{
    v2s result = v2s{a.x  / i, a.y  / i};
    return result;
}


static inline void operator += (v2s& a, v2s b)
{
    a = a + b;
}


static inline void operator -= (v2s& a, v2s b)
{
    a = a - b;
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

// Casting
// --------------------------------------------------------------

static _inline v2f V2F(v2s v)
{
    v2f result;
    result.x = f32(v.x);
    result.y = f32(v.y);
    
    return result;
}


static _inline v2f V2F(v2u v)
{
    v2f result;
    result.x = f32(v.x);
    result.y = f32(v.y);
    
    return result;
}


static _inline v2s AddX(v2s v, s32 X)
{
    v2s result = v;
    result.x += X;
    
    return result;
}


static _inline v2s AddY(v2s v, s32 Y)
{
    v2s result = v;
    result.y += Y;
    
    return result;
}


static _inline v2f AddX(v2f v, f32 X)
{
    v2f result = v;
    result.x += X;
    
    return result;
}


static _inline v2f AddY(v2f v, f32 Y)
{
    v2f result = v;
    result.y += Y;
    
    return result;
}