

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#define each(T, X, A) T X = Begin(A); X < End(A); ++X

template<typename T>
struct Dynamic_Array
{    
    u32 count;
    u32 capacity;
};


template<typename T>
static Dynamic_Array<T>* Create_Dynamic_Array(Allocator_Shell* allocator, u32 capacity)
{
    Assert(capacity);
    
    u32 size = sizeof(Dynamic_Array<T>) + capacity * sizeof(T);
    
    if(size < allocator->min_alloc_size)
    {
        u32 s = allocator->min_alloc_size - sizeof(Dynamic_Array<T>);
        s /= sizeof(T);
        capacity = s;
        size = sizeof(Dynamic_Array<T>) + capacity * sizeof(T);
    }
    Dynamic_Array<T>* darray = (Dynamic_Array<T>*)allocator->push(size);
    
    darray->count = 0;
    darray->capacity = capacity;

    
    Assert(darray->capacity);
    return darray;
}


template<typename T>
static inline T* Begin(Dynamic_Array<T>* darray)
{
    return (T*)(darray + 1);
}


template<typename T>
static inline T* End(Dynamic_Array<T>* darray)
{
    return (T*)(darray + 1) + darray->count;
}


template<typename T>
static T* Push(Dynamic_Array<T>** darray_ptr, Allocator_Shell* allocator)
{
    Assert(*darray_ptr);
    Assert((*darray_ptr)->count <= (*darray_ptr)->capacity);
    
    if((*darray_ptr)->count == (*darray_ptr)->capacity)
    {
        u32 new_capacity = (*darray_ptr)->capacity * 2;
        
        u32 new_size = new_capacity * sizeof(T) + sizeof(Dynamic_Array<T>);
        Dynamic_Array<T>* new_header = (Dynamic_Array<T>*)allocator->push(new_size);
        
        u32 old_size = (*darray_ptr)->capacity * sizeof(T) + sizeof(Dynamic_Array<T>);
        Mem_Copy(new_header, *darray_ptr, old_size);
        allocator->free(*darray_ptr);
        
        new_header->capacity = new_capacity;
        *darray_ptr = new_header;
        
    }
    
    T* result = Begin(*darray_ptr) + (*darray_ptr)->count;
    (*darray_ptr)->count += 1;
    
    *result = {};
    
    return result;
}


template<typename T>
static inline void Unordered_Remove(Dynamic_Array<T>* darray_ptr, u32 idx_to_remove)
{
    Assert(darray_ptr->count);
    Assert(idx_to_remove < darray_ptr->count);
    
    darray_ptr->count -= 1;
    
    T* buffer = Begin(darray_ptr);
    T* slot_to_remove = buffer + idx_to_remove;
    T* last_element = buffer + darray_ptr->count;
    
    *slot_to_remove = *last_element;
}