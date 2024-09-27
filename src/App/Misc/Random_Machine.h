

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once



struct Random_Machine
{
    i32 noise_position = 0;
    i32 seed = 1;
    
    u32 random_u32(u32 max)
    {
        u32 result = (Noise_Squirrel3(noise_position++, seed) % max);
        return result;
    }
    
    f32 random_f32()
    {
        u32 r = Noise_Squirrel3(noise_position++, seed) % 1000;
        
        f32 result = ((f32)r / (f32)1000);
        
        return result;
    }
};
