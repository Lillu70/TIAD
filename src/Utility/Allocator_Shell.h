

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

struct Allocator_Shell
{
    void* internal_alloc_adr = 0;
    void*(*_push)(void*, u32) = 0;
    void(*_free)(void*, void*) = 0;
    
    u32 min_alloc_size = 32;
    
    void* push(u32 size)
    {
        Assert(_push);
        return _push(internal_alloc_adr, size);
    }
    
    void free(void* ptr)
    {
        Assert(_free);
        _free(internal_alloc_adr, ptr);
    }
};