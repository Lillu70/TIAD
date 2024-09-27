

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#include "Primitives.h"
#include "Assert.h"

//#define Exhaustive_Memory_Verification
//#undef Exhaustive_Memory_Verification

#ifdef Exhaustive_Memory_Verification

#define EMV_Call(X) X

#else

#define EMV_Call(X)

#endif

// TODO: Implement reallc!


struct Free_Block
{
    Free_Block* prew_block = 0;
    Free_Block* next_block = 0;
    u32 size = 0;
};


struct General_Allocator
{
    static u32 constexpr min_alloc_size = sizeof(Free_Block);
    
    Free_Block* first_block;
    Free_Block* last_block;
    u8* memory = 0;
    u32 full_capacity = 0;
    u32 push_call_count = 0;
    u32 free_call_count = 0;
    
    bool owns_memory = 0;
    bool crash_on_out_of_memory = 0;

    void init(void* _memory, u32 _capacity, bool _crash_on_out_of_memory=true)
    {
        memory = (u8*)_memory;
        Free_Block* block = (Free_Block*)memory;
        *block = { 0,0, _capacity };
        first_block = block;
        last_block = block;
        
        full_capacity = _capacity;
        owns_memory = false;
        crash_on_out_of_memory = _crash_on_out_of_memory;
    }
    
    
    void init(Platform_Calltable* platform, u32 _capacity, bool _crash_on_out_of_memory=true)
    {
        if(owns_memory)
        {
            Assert(memory);
            platform->Free_Memory(memory);
        }
        
        void* _memory = platform->Allocate_Memory(_capacity, &_capacity);
        init(_memory, _capacity, _crash_on_out_of_memory);
        
        owns_memory = true;
    }
    
    
    void clear()
    {
        Free_Block* block = (Free_Block*)memory;
        *block = { 0,0, full_capacity };
        first_block = block;
        last_block = block;
        push_call_count = 0;
        free_call_count = 0;
    }
    
    
    u32 count_free_blocks()
    {
        u32 result = 0;
        
        Free_Block* block = first_block;
        while(block)
        {
            result += 1;
            block = block->next_block;
        }
        
        return result;
    }
    
    
    bool address_inside_region(u8* ptr)
    {
        return ptr >= memory && ptr < memory + full_capacity;
    }
    
    void test_free_block_start_to_end_ordering()
    {
        
        Free_Block* block = first_block;
        
        while(block)
        {
            Assert(!block->next_block || block->next_block > block);
            Assert((block == first_block && block == last_block) || block->prew_block != block->next_block);
            Assert(block != block->prew_block && block != block->next_block);
            
            Assert(!block->next_block || (u8*)(block->next_block) >= (u8*)block + block->size);
            
            block = block->next_block;
        }
    
    }
    
    void test_free_block_end_to_start_ordering()
    {
        Free_Block* block = last_block;
        
        while(block)
        {
            Assert(!block->prew_block || block->prew_block < block);
            Assert((block == first_block && block == last_block) ||block->prew_block != block->next_block);
            Assert(block != block->prew_block && block != block->next_block);
            block = block->prew_block;
        }
    
    }
    
    void test_free_block_allignement()
    {
        test_free_block_start_to_end_ordering();
        test_free_block_end_to_start_ordering();
        
        Free_Block* a_block = first_block;
        u32 a_count = 0;
        
        while(a_block)
        {
            Free_Block* b_block = first_block;
            
            while(b_block)
            {
                if(a_block != b_block)
                {
                    if((u8*)b_block == (u8*)a_block + a_block->size)
                    {
                        Terminate;
                    }
                    
                    if((u8*)b_block + b_block->size == (u8*)a_block)
                    {
                        Terminate;
                    }
                }
                
                b_block = b_block->next_block;
            }
            
            a_count += 1;
            a_block = a_block->next_block;
        }
    }
    
    
    void* push(u32 size)
    {
        EMV_Call(test_free_block_allignement();)
        
        push_call_count += 1;
        
        size += sizeof(size);
        if (size < min_alloc_size)
            size = min_alloc_size;
        
        u32 best_diff = 0;
        Free_Block* block = 0;
        
        Free_Block* fb = first_block;
        
        while (fb)
        {            
            
            DB_Call({
                if(fb != first_block)
                {
                    if(!fb->prew_block)
                        Terminate;
                    
                    if(!address_inside_region((u8*)fb->prew_block))
                        Terminate;
                }    
            });
            
            // Exact match. Push in here!
            if (fb->size == size)
            {
                remove_fb_from_chain(fb);
                
                u32* result = (u32*)fb;
                *result = size;
                
                EMV_Call(test_free_block_allignement();)
                
                return (void*)(result + 1);
            }
            
            // Can fit the alloc in here, but it's not an exact match.
            else if (fb->size > size)
            {
                u32 diff = fb->size - size;
                if (diff > best_diff)
                {
                    best_diff = diff;
                    block = fb;
                }
            }
            
            fb = fb->next_block;
        }
        
        if (block)
        {
            // This is the biggest free block and it can't fit a free header in it.
            if (block->size < size + sizeof(Free_Block))
            {
                EMV_Call(test_free_block_allignement();)
                
                // So we allocate all of this, sorry; you get more than what you asked for.
                // but, now we have to get rid of this block.
                remove_fb_from_chain(block);
                size = block->size;
                
                EMV_Call(test_free_block_allignement();)
            }
        
            // Shave the fb, and then move the fb definition.    
            else
            {
                Assert(block != block->prew_block && block != block->next_block);
                EMV_Call(test_free_block_allignement();)
                
                Free_Block block_copy = *block;
                
                // This has similar concerns to killing a block;
                // next and previous have to be informed about where the fb now resides.
                Free_Block* new_pos = (Free_Block*)((u8*)block + size);
                Assert(new_pos != block->next_block);
                Assert(new_pos != block->prew_block);
                Assert(new_pos != block);
                
                Assert(block != block->prew_block && block != block->next_block);
                
                *new_pos = { block->prew_block, block->next_block, block->size - size };
                
                Assert(new_pos != new_pos->prew_block && new_pos != new_pos->next_block);
                
                relink_free_block(new_pos);
                Assert(new_pos != new_pos->prew_block && new_pos != new_pos->next_block);
                
                EMV_Call
                (
                    test_free_block_allignement();
                    int a = 0;
                )
            }
            
            u32* result = (u32*)block;
            *result = size;
            
            EMV_Call(test_free_block_allignement();)
            
            return (void*)(result + 1);
        
        }
        
        if(crash_on_out_of_memory)
        {
            Terminate;            
        }
        
        return 0;
    }

    template<typename T>
    T* push()
    {
        return (T*)push(sizeof(T));
    }

    void free(void* ptr)
    {
        EMV_Call(test_free_block_allignement();)
        
        free_call_count += 1;
        
        u8* block_start = (u8*)ptr - sizeof(Free_Block::size);
        u32 block_size = *((u32*)block_start);
        
        Assert(block_start >= memory && block_start < memory + full_capacity);
        
        // We need to find out where on the memory line this block is.
        
        if (first_block)
        {
        
            // Seek for the last free block that is behind this address. 
            Free_Block* fb = 0;
            Assert(block_start != (u8*)first_block);
            
            
            if (block_start > (u8*)first_block)
            {
                fb = first_block;
                while (fb->next_block && (u8*)fb->next_block < block_start)
                    fb = fb->next_block;
                
                if(!fb)
                    fb = last_block;
                
                
                Assert(fb >= first_block);
                Assert(block_start >= (u8*)(first_block + 1));
                Assert(!fb->next_block || (u8*)block_start + block_size <= (u8*)fb->next_block);
                
                // some block was found to be before this point on the memory line.
                
                // that block ends where this one starts.
                if ((u8*)fb + fb->size == block_start)
                {
                    // expand fp to include this memory area.
                    fb->size += block_size;
                    
                    // This block is in between 2 free blocks.
                    if ((u8*)fb->next_block == block_start + block_size)
                    {
                        // Again grow left block, now by next free block size.
                        fb->size += fb->next_block->size;
                        
                        // this kills a free block.
                        // TODO: this func is overkill. In this case we know we're not the first block.
                        remove_fb_from_chain(fb->next_block);
                        
                        EMV_Call
                        (
                            test_free_block_allignement();
                            int a = 0;
                        )
                    }
                    
                    EMV_Call(test_free_block_allignement();)
                }
                
                // Some block is right after this one. Merge that backwards.
                else if ((u8*)fb->next_block == block_start + block_size)
                {
                    // ~ Moving the next block backwards.
                    
                    EMV_Call(test_free_block_allignement();)
                    
                    u8* fb_copy = (u8*)(first_block);
                    u8* lb_copy = (u8*)(last_block);
                    
                    Free_Block* new_pos = (Free_Block*)block_start;
                    *new_pos = { fb->next_block->prew_block, fb->next_block->next_block, fb->next_block->size + block_size };
                    relink_free_block(new_pos);
                    
                    
                    EMV_Call(test_free_block_allignement();)
                }
                
                // We have a previous block, but we don't allign with it or the it's next block.
                // We have to create a new free block!
                else
                {
                    EMV_Call(test_free_block_allignement();)
                    Free_Block* new_block = (Free_Block*)block_start;
                    
                    Assert(fb < new_block);
                    Assert(!fb->next_block || (u8*)new_block < (u8*)fb->next_block);
                    Assert(!fb->next_block || (u8*)new_block + block_size < (u8*)fb->next_block);
                    
                    *new_block = { fb, fb->next_block, block_size };
                    new_block->prew_block->next_block = new_block;
                    
                    if (new_block->next_block)
                        new_block->next_block->prew_block = new_block;
                    else
                        last_block = new_block;
                    
                    
                    EMV_Call(test_free_block_allignement();)
                }
            
            }
            else
            {
                Assert(block_start < (u8*)first_block);
                
                // First block is right after this one. Merge that backwards.
                if ((u8*)first_block == block_start + block_size)
                {
                    Free_Block* new_pos = (Free_Block*)block_start;
                    *new_pos = { 0, first_block->next_block, first_block->size + block_size };
                    first_block = new_pos;
                    
                    if (first_block->next_block)
                        first_block->next_block->prew_block = first_block;
                    else
                        last_block = first_block;
                    
                    EMV_Call(test_free_block_allignement();)
                }
                
                // We are before the first block and we don't allgin with it.
                // Create a new block here and link with prev first.
                else
                {
                    EMV_Call(test_free_block_allignement();)
                    
                    Free_Block* new_block = (Free_Block*)block_start;
                    
                    *new_block = { 0, first_block, block_size };
                    first_block->prew_block = new_block;
                    first_block = new_block;
                    
                    
                    EMV_Call(test_free_block_allignement();)
                }
            }
        }
        
        // We don't have a first block.
        // Make this the first and the last block.
        else
        {
            Free_Block* new_block = (Free_Block*)block_start;
            *new_block = { 0, 0, block_size };
            first_block = new_block;
            last_block = new_block;
            
            EMV_Call(test_free_block_allignement();)
            
        }
        
        
        EMV_Call
        (
            test_free_block_allignement();
            int a = 0;
        )
    }
    
    static void* _Static_Push(void* allocator, u32 size)
    {
        void* result = ((General_Allocator*)allocator)->push(size);
        return result;
    }
    
    
    static void _Static_Free(void* allocator, void* ptr)
    {
        ((General_Allocator*)allocator)->free(ptr);
    }

private:
    inline void remove_fb_from_chain(Free_Block* fb)
    {
        if (fb->prew_block && fb->next_block)
        {
        fb->prew_block->next_block = fb->next_block;
        fb->next_block->prew_block = fb->prew_block;
        }
        
        else
        {
            // this is a case where we can be;
            // - the only block. Clear first and last block to 0.
            // When freeing create a free block at the free site,
            // and link up the first and last in there.
            
            // - the first block. (but not the last one)
            // Shift up the first block into the next block.
            
            // - the the last block. (but not the first one)
            // shift last block down into the previous block.
            
            bool is_first = (fb == first_block);
            bool is_last = (fb == last_block);
            
            if (is_first && is_last)
            {
                first_block = 0;
                last_block = 0;
            }
            
            else if (is_first)
            {
                first_block = fb->next_block;
                first_block->prew_block = 0;
            }
            
            else // is last assumed.
            {
                last_block = fb->prew_block;
                last_block->next_block = 0;
            }
        }
    
    }
    
    inline void relink_free_block(Free_Block* fb)
    {
        if (fb->prew_block)
            fb->prew_block->next_block = fb;
        else
            first_block = fb;
        
        if (fb->next_block)
            fb->next_block->prew_block = fb;
        else
            last_block = fb;
    }
    
};


struct Paged_General_Allocator
{    
    static u32 constexpr min_alloc_size = General_Allocator::min_alloc_size + sizeof(u32);

    Platform_Calltable* platform;
    General_Allocator* page_table = 0;
    u32 page_capacity = 0;
    u32 active_page_count = 0;
    u32 active_page_idx = 0;
    u32 default_page_size = 0;
    
    u32 push_call_count = 0;
    u32 free_call_count = 0;

    void clear()
    {
        for(u32 i = 0; i < active_page_count; ++i)
        {
            page_table[i].clear();
        }
        active_page_idx = 0;
        
        push_call_count = 0;
        free_call_count = 0;
    }
    
    
    void resize_page_table(u32 new_table_capacity)
    {
        if(new_table_capacity <= page_capacity)
            return;
        
        u32 page_size = sizeof(*page_table);
        u32 alloc_size = page_size * new_table_capacity;
        u32 result_alloc_size;
        General_Allocator* new_table = 
            (General_Allocator*)platform->Allocate_Memory(alloc_size, &result_alloc_size);
        
        if(result_alloc_size != alloc_size)
            new_table_capacity = result_alloc_size / page_size;
        
        page_capacity = new_table_capacity;
        
        if(page_table)
        {
            u32 old_table_size = active_page_count * page_size;
            Mem_Copy(new_table, page_table, old_table_size);
            
            platform->Free_Memory(page_table);
        }
        
        page_table = new_table;
    }
    
    
    void init(Platform_Calltable* _platform, u32 _page_count=255, u32 _default_page_size = MiB)
    {
        
        default_page_size = _default_page_size;
        platform = _platform;
        
        clear();
        resize_page_table(_page_count);
        active_page_count = 1;
        active_page_idx = 0;
        (*page_table).init(platform, default_page_size, false);
    }
    
    
    void* push(u32 size)
    {
        push_call_count += 1;
        size += sizeof(size);
        
        void* result = page_table[active_page_idx].push(size);
        
        // Try to push the allocation in one of the existing buckets.
        if(!result)
        {
            for(u32 i = 0; i < active_page_count; ++i)
            {
                if(i == active_page_idx)
                    continue;
                
                // TODO: Keep track of the largest free chunk to quickly see if this allocation could fit.
                result = page_table[i].push(size);
                if(result)
                {
                    active_page_idx = i;
                    break;
                }
            }
        
            // Still no luck. Make a new bucket.
            if(!result)
            {
                active_page_idx = active_page_count;
                active_page_count += 1;
                
                if(active_page_count > page_capacity)
                    resize_page_table(page_capacity * 2);
                
                Assert(active_page_count < page_capacity);
                
                u32 page_size = Max(default_page_size, u32(size + sizeof(size)));
                page_table[active_page_idx].init(platform, page_size, false);
                
                result = page_table[active_page_idx].push(size);
            }
        }
        
        Assert(result);
        
        u32* raw_result = (u32*)result;
        *raw_result = active_page_idx;
        
        return raw_result + 1;
    }
    
    
    template<typename T>
    T* push()
    {
        return (T*)push(sizeof(T));
    }
    
    
    void free(void* ptr)
    {
        Assert(ptr);
        
        free_call_count += 1;
        
        u32* source_page_idx = ((u32*)ptr) - 1;
        Assert(*source_page_idx < active_page_count);
        
        page_table[*source_page_idx].free(source_page_idx);
    }
    
    
    void validate_heap()
    {
        for(u32 i = 0; i < active_page_count; ++i)
        {
            page_table[i].test_free_block_allignement();
        }
    }
    
    
    static void* _Static_Push(void* allocator, u32 size)
    {
        void* result = ((Paged_General_Allocator*)allocator)->push(size);
        return result;
    }
    
    
    static void _Static_Free(void* allocator, void* ptr)
    {
        ((Paged_General_Allocator*)allocator)->free(ptr);
    }
};


static inline void Init_Shell_From_General_Allocator(
    Allocator_Shell* shell, 
    General_Allocator* general_allocator)
{
    shell->internal_alloc_adr = general_allocator;
    shell->_push = General_Allocator::_Static_Push;
    shell->_free = General_Allocator::_Static_Free;
    shell->min_alloc_size = General_Allocator::min_alloc_size;
}


static inline void Init_Shell_From_Paged_General_Allocator(
    Allocator_Shell* shell, 
    Paged_General_Allocator* page_general_allocator)
{
    shell->internal_alloc_adr = page_general_allocator;
    shell->_push = Paged_General_Allocator::_Static_Push;
    shell->_free = Paged_General_Allocator::_Static_Free;
    shell->min_alloc_size = Paged_General_Allocator::min_alloc_size;
}


#if 0
// Don't think I want to include stdlib in this project, 
// but this is here to test Allocator_Shell.

#include <stdlib.h>
static void* Shell_CMEM_Push(void* _, u32 size)
{
    void* result = malloc(size);
    return result;
}


static void Shell_CMEM_Free(void* _, void* ptr)
{
    free(ptr);
}


static inline void Init_Shell_From_CAlloc(Allocator_Shell* shell)
{
    shell->internal_alloc_adr = 0;
    shell->_push = Shell_CMEM_Push;
    shell->_free = Shell_CMEM_Free;
    shell->min_alloc_size = 64;
}
#endif


struct Linear_Allocator
{
    u8* memory = 0;
    u8* next_free = 0;
    u32 capacity = 0;
    u32 push_count = 0;
    bool owns_memory = 0;
    
    
    u32 inline get_free_capacity()
    {
        u32 result = capacity - u32(next_free - memory);
        return result;
    }
    
    
    u32 inline get_used_capacity()
    {
        u32 result = u32(next_free - memory);
        return result;
    }
    
    
    void init(void* _memory, u32 _capacity)
    {
        Assert(_capacity);
        Assert(_memory);
        
        memory = (u8*)_memory;
        next_free = memory;
        capacity = _capacity;
        push_count = 0;
        
        owns_memory = false;
    }
    
    
    void init(Platform_Calltable* platform, u32 _capacity)
    {
        Assert(_capacity);
        
        if(owns_memory)
        {
            Assert(memory);
            platform->Free_Memory(memory);
        }
        
        void* _memory = platform->Allocate_Memory(_capacity, &_capacity);
        
        Assert(_memory);
        
        init(_memory, _capacity);
        
        owns_memory = true;
    }
    
    
    void clear()
    {
        next_free = memory;
        push_count = 0;
    }
    
    
    void* push(u32 size)
    {
        Assert(memory);
        if(size)
        {
            Assert(get_free_capacity() >= size);
            
            push_count += 1;
            u8* result = next_free;
            next_free += size;
            
            return result;
        }
        else
        {
            return 0;
        }
    }
    
    
    void* safe_push(u32 size)
    {
        Assert(memory);
        
        u8* result = 0;
        
        if(get_free_capacity() >= size && size)
        {
            push_count += 1;
            result = next_free;
            next_free += size;
        }
        
        return result;
    }
    
    
    template<typename T>
    T* push()
    {
        return (T*)push(sizeof(T));
    }
};


static Linear_Allocator Create_Linear_Allocator(void* memory, u32 capacity)
{
    Linear_Allocator result;
    result.init(memory, capacity);
    
    return result;
}


static Linear_Allocator Clone_From_Linear_Allocator_Free_Space(Linear_Allocator* other)
{
    Linear_Allocator result;
    result.init(other->next_free, other->get_free_capacity());
    
    return result;
}