

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

struct Win32_App
{
    i32 window_width = 0;
    i32 window_height = 0;
    i32 flags = 0;
    f32 scroll_delta = 0;
    
    HDC dc = 0;
    HINSTANCE instance = 0;
    HWND window = 0;
    DWORD main_thread_id;
    
    WINDOWPLACEMENT window_placement = { sizeof(window_placement) };
    
    f64 frame_time = 0;
    u64 frame_counter = 0;
    i64 timer_counter_freg = 0;
    LARGE_INTEGER last_time_counter;
    
    f64 next_controler_test_time = 0;
    
    Char_Array typing_information = {};
    
    inline static constexpr i32 max_controllers = 1;
    
    Controller_State controller_state[max_controllers];
    
    _SYSTEM_INFO sys_info;
    
    bool force_update_surface = false;
    volatile bool init_done = false;
};


struct Win32_Bitmap
{
    inline static constexpr i32 bytes_per_pixel = 4;
    
    u64 allocated_memory = 0;
    u32* memory = nullptr;
    i32 width = 0, height = 0;
    BITMAPINFO info{};
};


static void Win32_Init(
    HINSTANCE instance, 
    u32 window_width, 
    u32 window_height, 
    u32 window_pos_x, 
    u32 window_pos_y, 
    char* window_title);

    
static void Win32_Update_Surface(bool update_from_game);
static LRESULT Win32_Message_Handler(HWND win_handle, UINT message, WPARAM wparam, LPARAM lParam);
static u32* Win32_Resize_Pixel_Buffer(i32 new_width, i32 new_height);
static void Win32_Flush_Events();
static void Win32_Set_Flag(App_Flags flag, bool value);
static f64 Win32_Update_Frame_Time();
static u32 Win32_Get_Window_Width();
static u32 Win32_Get_window_Height();
static Pixel_Buffer Win32_Get_Pixel_Buffer();
static i32 Win32_Get_Pixel_Buffer_Width();
static i32 Win32_Get_Pixel_Buffer_Height();
static u64 Win32_Get_CPU_Time_Stamp();
static u32 Win32_Get_Flags();
static Controller_State Win32_Get_Controller_State(i32 contoller_idx);
static f64 Win32_Get_Time_Stamp();
static v2i Win32_Get_Cursor_Position();
static Platform_Calltable Win32_Get_Calltable();
static bool Win32_Get_Keyboard_Key_Down(Key_Code key_code);
static u64 Win32_Get_Frame_Count();
static bool Win32_Write_File(const char* file_name, u8* buffer, u32 bytes_to_write);
static bool Win32_Get_File_Size(const char* file_name, u32* out_file_size);
static bool Win32_Read_File(const char* file_name, u8* out_buffer, u32 bytes_to_read);
static Char_Array Win32_Get_Typing_Information();
static char* Win32_Get_Clipboard_Data_As_Text();
static void Win32_Set_Clipboard_Data_As_Text(char* data, u32 lenght);
static f32 Win32_Get_Scroll_Wheel_Delta();
static Create_Directory_Result::T Win32_Create_Directory(char* path);
static void* Win32_Allocate_Memory(u32 amount, u32* out_amount);
static void Win32_Free_Memory(void* memory);
static Dynamic_Array<String>* Win32_Search_Directory_For_Maching_Names(char* file_name, Allocator_Shell* allocator);


// ---------------------------------------------------------------
// Silly maps begin here.



static constexpr i32 s_controller_map[(u64)Button::BUTTON_COUNT] =
{
    0x0001,     //0
    0x0002,     //1
    0x0004,     //2
    0x0008,     //3
    0x0010,     //4
    0x0020,     //5
    0x0040,     //6
    0x0080,     //7
    0x0100,     //8
    0x0200,     //9
    0x1000,     //10
    0x2000,     //11
    0x4000,     //12
    0x8000      //13
};


static constexpr i32 s_keycode_map[(u64)Key_Code::COUNT] = 
{
    '0',
    '1',
    '2',
    '3',
    '4',
    '5',
    '6',
    '7',
    '8',
    '9',
    VK_NUMPAD0,
    VK_NUMPAD1,
    VK_NUMPAD2,
    VK_NUMPAD3,
    VK_NUMPAD4,
    VK_NUMPAD5,
    VK_NUMPAD6,
    VK_NUMPAD7,
    VK_NUMPAD8,
    VK_NUMPAD9,
    VK_DIVIDE,
    VK_MULTIPLY,
    VK_ADD,
    VK_SUBTRACT,
    VK_DECIMAL,
    'A',
    'B',
    'C',
    'D',
    'E',
    'F',
    'G',
    'H',
    'I',
    'J',
    'K',
    'L',
    'M',
    'N',
    'O',
    'P',
    'Q',
    'R',
    'S',
    'T',
    'U',
    'V',
    'W',
    'X',
    'Y',
    'Z',
    VK_F1,
    VK_F2,
    VK_F3,
    VK_F4,
    VK_F5,
    VK_F6,
    VK_F7,
    VK_F8,
    VK_F9,
    VK_F10,
    VK_F11,
    VK_F12,
    VK_LEFT,
    VK_RIGHT,
    VK_UP,
    VK_DOWN,
    VK_ESCAPE,
    VK_LCONTROL,
    VK_LSHIFT,
    VK_LMENU,
    VK_LWIN,
    VK_LBUTTON,
    VK_RCONTROL,
    VK_RSHIFT,
    VK_RMENU,
    VK_RWIN,
    VK_RBUTTON,
    VK_SPACE,
    VK_PRIOR,
    VK_NEXT,
    VK_END,
    VK_HOME,
    VK_INSERT,
    VK_DELETE,
    VK_BACK,
    VK_RETURN,
    VK_TAB
};