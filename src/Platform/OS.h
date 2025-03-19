

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

struct Work_Data
{
    union
    {
        void* user_ptr;
        u8 buffer[64];        
    };
};

typedef void(*Platform_Work_Function)(Work_Data* data, struct Linear_Allocator* work_memory);


// TODO: Figure something less dumb to replace Char_Array!
struct Char_Array
{
    char buffer[8];
    u8 count;
};


namespace Platform_Flags
{
    enum T : u64
    {
        wants_to_exit   = 1 << 0,
        focused         = 1 << 1,
        fullscreen      = 1 << 2,
        vsync           = 1 << 3,
        resize          = 1 << 4,
    };    
};
static Platform_Flags::T Platform_Get_Flags();


static void Platform_Init(char* window_title, v2s window_position, v2s window_dimensions);

static void Platform_Update();

static void Platform_Swap_Buffers();

static u64 Platform_Get_Frame_Count();

static v2s Platform_Get_Window_Dimensions();

static v2s Platform_Get_Cursor_Position();

static bool Platform_Get_Keyboard_Key_Down(Key_Code key_code);

static f32 Platform_Get_Scroll_Wheel_Delta();

static f64 Platform_Get_Time_Stamp();

static f64 Platform_Get_Frame_Time();

static s32 Platform_Get_OpenGL_Version();

static void* Platform_Allocate_Memory(u32 amount, u32* out_amount);

static void Platform_Free_Memory(void* memory);

static Char_Array Platform_Get_Typing_Information();

static void Platform_Set_Clipboard_Data_As_Text(char* buffer, u32 size);

static char* Platform_Get_Clipboard_Data_As_Text();

static bool Platform_Get_File_Size(char* path, u32* out_size);

static bool Platform_Write_File(char* path, char* buffer, u32 buffer_size);

static u32 Platform_Get_Sound_Samples_Per_Second();

static bool Platform_Read_File(char* path, u8* buffer, u32 buffer_size);

static Controller_State Platform_Get_Controller_State(u32 controller);

static void Platform_Queue_Threaded_Work(Platform_Work_Function work_function, Work_Data* work_data);

static void Platform_Set_Fullscreen(bool enabled);

static void Platform_Set_VSync(bool enabled);

static Color* Platform_Resize_Software_Render_Target_Pixel_Buffer(v2s dim);