

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

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


static u32 Convert_String_View_Into_U32(String_View view)
{
    u32 result = 0;
    u32 digit = view.lenght;
    
    char* begin = view.buffer;
    for(char* c = begin; c < begin + view.lenght; ++c)
    {
        u32 v = *c - u32('0');
        Assert(v < 10);
        
        v *= Pow32(10, --digit);
        result += v;
    }
    
    return result;
}


static inline u32 Get_Longest_CSTR_Length(char* a, char* b)
{
    u32 a_len = Null_Terminated_Buffer_Lenght(a);
    u32 b_len = Null_Terminated_Buffer_Lenght(b);
    
    u32 result = a_len > b_len? a_len : b_len;
    return result;
}


static inline char* Get_Longest_CSTR(char* a, char* b)
{
    u32 a_len = Null_Terminated_Buffer_Lenght(a);
    u32 b_len = Null_Terminated_Buffer_Lenght(b);
    
    char* result = a_len > b_len? a : b;
    return result;
}


static inline char* Get_Longest_CSTR(char* a, char* b, char* c)
{
    char* a_to_b = Get_Longest_CSTR(a, b);
    char* result = Get_Longest_CSTR(a_to_b, c);
    
    return result;
}


static inline char* Get_Longest_CSTR(char* a, char* b, char* c, char* d)
{
    char* a_to_c = Get_Longest_CSTR(a, b, c);
    char* result = Get_Longest_CSTR(a_to_c, d);
    
    return result;
}


static inline char* Get_Longest_CSTR(char* a, char* b, char* c, char* d, char* f)
{
    char* a_to_d = Get_Longest_CSTR(a, b, c, d);
    char* result = Get_Longest_CSTR(a_to_d, f);
    
    return result;
}


static char* Get_Longest_CSTR(CSTR_List strings)
{
    Assert(strings.count);
    
    char* result = strings.list[0];
    u32 max_lenght = Null_Terminated_Buffer_Lenght(result);
    
    for(u32 i = 1; i < strings.count; ++i)
    {
        char* element = strings.list[i];
        u32 len = Null_Terminated_Buffer_Lenght(element);
        if(len > max_lenght)
        {
            max_lenght = len;
            result = element;
        }
    }
    
    return result;
}


static char* Get_Longest_CSTR(CSTR_List strings_a, CSTR_List strings_b)
{
    char* a = Get_Longest_CSTR(strings_a);
    char* b = Get_Longest_CSTR(strings_b);
    
    char* result = Get_Longest_CSTR(a, b);
    return result;
}