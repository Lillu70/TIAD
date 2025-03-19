

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once


static constexpr Color Make_Color(u8 red, u8 green, u8 blue, u8 alpha = 0xFF)
{
    Color result = {};
    result.red = red;
    result.green = green;
    result.blue = blue;
    result.alpha = alpha;
    
    return result;
}


// Returns the lenght of the buffer. Does NOT include the null terminator.
static inline u32 Null_Terminated_Buffer_Lenght(u8* buffer)
{
    u8* b;
    for(b = buffer; *b != 0; ++b){}
    return u32(b - buffer);
}


// Returns the lenght of the buffer. Does NOT include the null terminator.
static inline u32 Null_Terminated_Buffer_Lenght(char* buffer)
{
    char* b;
    for(b = buffer; *b != 0; ++b){}
    return u32(b - buffer);
}


static inline void Mem_Copy(void* dest, void* source, u32 byte_count)
{
    Assert(dest);
    Assert(source);
    Assert(byte_count);
    
    for(u32 i = 0; i < byte_count; ++i)
    {
        u8* sb = ((u8*)source) + i;
        u8* db = ((u8*)dest) + i;
        
        *db = *sb;
    }
}


static inline void Mem_Copy_Allow_Zero_Bytes(void* dest, void* source, u32 byte_count)
{
    Assert(dest);
    Assert(source);
    
    for(u32 i = 0; i < byte_count; ++i)
    {
        u8* sb = ((u8*)source) + i;
        u8* db = ((u8*)dest) + i;
        
        *db = *sb;
    }
}


static inline void Mem_Zero(void* mem, u32 byte_count)
{
    u8* mem_start = (u8*)mem;
    for(u8* p = mem_start; p < mem_start + byte_count; ++p)
        *p = 0;
}


static void Remove_Element_From_Packed_Array(
    void* array, 
    u32* array_count, 
    u32 element_size, 
    u32 remove_idx)
{
    Assert(remove_idx < *array_count);
    Assert(*array_count > 0);
    
    for(u32 i = remove_idx; i < *array_count - 1; ++i)
    {
        u8* a = (u8*)array + (i * element_size);
        u8* b = a + element_size;
        Mem_Copy(a, b, element_size);
    }
    
    *array_count -= 1;
    Mem_Zero((u8*)array + (*array_count * element_size), element_size);
}


static void Insert_Element_Into_Packed_Array(
    void* array, 
    void* element, 
    u32* array_count, 
    u32 element_size, 
    u32 insert_idx)
{
    Assert(insert_idx < *array_count);
    Assert(*array_count > 0);
    
    for(u32 i = *array_count; i > insert_idx; --i)
    {
        u8* a = (u8*)array + (i * element_size);
        u8* b = a - element_size;
        Mem_Copy(a, b, element_size);
    }
    
    Mem_Copy(((u8*)array + element_size * insert_idx), element, element_size);
    
    *array_count += 1;
}


// Buffer size is assumed to be 11 or greater.
static char* U32_To_Char_Buffer(u8* buffer, u32 integer)
{
    // TODO: This works, but's odd and not very intuitive, so rethink and rework this.
    
    u32 buffer_size = 11;
    
    buffer[buffer_size - 1] = 0;
    
    u32 ascii_numeric_offset = 48;
    u32 last_non_zero = buffer_size - 2;
    for(u32 i = 0; i < buffer_size - 1; ++i)
    {
        u32 digit = 0;
        if(i > 0)
            digit = (u32)(integer / Pow32(10, i)) % 10;
        else
            digit = integer % 10;
        u32 write_pos = buffer_size - 2 - i;
        if(digit)
            last_non_zero = write_pos;
        
        buffer[write_pos] = ascii_numeric_offset + digit;
    }
    
    return (char*)(buffer + last_non_zero);
}