

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once


static inline u32 Inverse_Bit_Mask(u32 bitfield, u32 mask)
{
    mask = ~mask;
    u32 result = bitfield & mask;
    return result;
}


static inline u64 Inverse_Bit_Mask(u64 bitfield, u64 mask)
{
    mask = ~mask;
    u64 result = bitfield & mask;
    return result;
}


static inline void Inverse_Bit_Mask(u32* bitfield, u32 mask)
{
    *bitfield = Inverse_Bit_Mask(*bitfield, mask);
}


static inline void Inverse_Bit_Mask(u64* bitfield, u64 mask)
{
    *bitfield = Inverse_Bit_Mask(*bitfield, mask);
}


static inline bool Bit_Not_Set(u32 bitfield, u32 mask)
{
    bool result = !(bitfield & mask);
    return result;
}


static inline bool Bit_Not_Set(u64 bitfield, u64 mask)
{
    bool result = !(bitfield & mask);
    return result;
}


static inline u64 Set_Nth_Bit(u64 bitfield, u64 n, bool state)
{
    u64 result = (bitfield & ~(1 << n)) | (state << n);
    return result;
}


static inline bool Is_Bit_Set(u64 bitfield, u64 mask)
{
    bool result = (bitfield & mask) > 0;
    return result;
}