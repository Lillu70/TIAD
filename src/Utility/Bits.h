

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


static inline void Inverse_Bit_Mask(u32* bitfield, u32 mask)
{
    *bitfield = Inverse_Bit_Mask(*bitfield, mask);
}


static inline bool Bit_Not_Set(u32 bitfield, u32 mask)
{
    bool result = !(bitfield & mask);
    return result;
}