

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once


#include "Azewin.h"
#include "Windows_Keymap.h"

static XInputGetState* _XInputGetState = 0;

struct Bitmap
{
    Color* buffer;
    u32 allocated_capacity;
    s32 width;
    s32 height;
};
static Bitmap s_software_render_target = {};


struct Platform_State
{
    WWindowHandle window;
    WDeviceHandle device_context;
    
    f64 frame_time;
    u64 frame_count;
    u64 flags;
    u64 system_clock_freqeuncy;
    u64 last_time_stamp;

    v2s window_dimensions;

    Controller_State controller_state[1];
    f64 next_controler_test_time;

    WSystemInfo system_info;
    WWindowPlacement window_placement;
    
    volatile u32 main_thread_id;
};
static Platform_State s_platform_state = {};


static void Software_Renderer_Init();


static Platform_Flags::T Platform_Get_Flags()
{
    return Platform_Flags::T(s_platform_state.flags);
}


static inline void Set_Zero_To_CW_DEFAULT(s32* value)
{
    Assert(value);
    
    if(!*value)
    {
        *value = CW_DEFAULT;
    }
}


static s64 Display_Window_Proc(WWindowHandle window, u32 message, u64 wparam, s64 lparam)
{
    s64 result = 0;

    switch (message)
    {
        // NOTE: For any message to get passed into the main thread they have to be included here!
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_SIZE:
        case WM_CHAR:
        case WM_MOUSEWHEEL:
        case WM_ACTIVATEAPP:
        {
            PostThreadMessageA(s_platform_state.main_thread_id, message, wparam, lparam);
        }break;
        
        default:
        {
            result = DefWindowProcA(window, message, wparam, lparam);
        }break;
    }
    return result;
}


struct Message_Thread_Paramater
{
    char* window_title;
    v2s window_position;
    v2s window_dimensions;
    
    volatile bool init_done;
};


static u32 __stdcall Message_Thread(void* thread_parameter)
{
    Message_Thread_Paramater* mtp = (Message_Thread_Paramater*)thread_parameter;
    
    WWindowClass window_class = {};
    
    char* window_class_name = "DangerousThreadsCrewClass";
    
    window_class.style = CS_OWNDC;
    window_class.window_procedure = &Display_Window_Proc;
    window_class.instance = GetModuleHandleA(0);
    window_class.icon = LoadIconA(window_class.instance, WMakeResource(101));
    window_class.cursor = LoadCursorA(0, WMakeResource(IDC_CROSS));
    window_class.class_name = window_class_name;
    
    RegisterClassA(&window_class);
    
    WWindowHandle window = CreateWindowExA(
        0,
        window_class_name,
        mtp->window_title,
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        mtp->window_position.x,
        mtp->window_position.y,
        mtp->window_dimensions.x,
        mtp->window_dimensions.y,
        0,
        0,
        window_class.instance,
        0);
    
    
    // Calibrate the window position offset.
    {            
        WRect window_rect;
        b32 get_window_rect_result = GetWindowRect(window, &window_rect);
        
        WRect client_rect;
        b32 get_client_rect_result = GetClientRect(window, &client_rect);
        
        s32 _window_width = window_rect.right - window_rect.left;
        s32 _window_height = window_rect.bottom - window_rect.top;
        
        s32 border_width = (_window_width - client_rect.right);
        s32 border_height = (_window_height - client_rect.bottom);
        
        s32 scale = 1;
        
        s32 new_width = _window_width * scale + border_width;
        s32 new_height = _window_height * scale + border_height;
        
        SetWindowPos(window, 0, window_rect.left, window_rect.top, new_width, new_height, 0);
    }
    
    s_platform_state.window = window;
    
    _ReadWriteBarrier();
    mtp->init_done = true;
    
    for(;;)
    {
        WMessage message;
        
        GetMessageA(&message, 0, 0, 0); 
        TranslateMessage(&message);
        DispatchMessageA(&message);
    }
    
    return 0;
}


static void Platform_Init(char* window_title, v2s window_position, v2s window_dimensions)
{
    Set_Zero_To_CW_DEFAULT(&window_position.x);
    Set_Zero_To_CW_DEFAULT(&window_position.y);
    Set_Zero_To_CW_DEFAULT(&window_dimensions.x);
    Set_Zero_To_CW_DEFAULT(&window_dimensions.y);
    
    GetSystemInfo(&s_platform_state.system_info);
    
    {
        WModuleHandle xinput = {};
        for(u64 i = 0; i < Array_Lenght(WModuleMamesXInput[i]) && !xinput; ++i)
        {
            xinput = LoadLibraryA(WModuleMamesXInput[i]);
            if(xinput)
            {
                _XInputGetState = (XInputGetState*)GetProcAddress(xinput, XInputGetStateProcName);
            }
        }
    }
    
    
    s_platform_state.main_thread_id = GetCurrentThreadId();
    
    u32 message_thread_id;
    Message_Thread_Paramater message_thread_parameter = {window_title, window_position, window_dimensions};
    CloseHandle(CreateThread(0, 0, Message_Thread, &message_thread_parameter, 0, &message_thread_id));
    
    while(!message_thread_parameter.init_done);
    
    s_platform_state.flags |= Platform_Flags::focused;
    
    _ReadWriteBarrier();
    
    {
        WRect client_rect;
        if(GetClientRect(s_platform_state.window, &client_rect))
        {
            s32 _window_width = client_rect.right - client_rect.left;
            s32 _window_height = client_rect.bottom - client_rect.top;
            s_platform_state.window_dimensions = {_window_width, _window_height};
        }
    }    
    
    s_platform_state.device_context = GetDC(s_platform_state.window);

    QueryPerformanceFrequency(&s_platform_state.system_clock_freqeuncy);
    QueryPerformanceCounter(&s_platform_state.last_time_stamp);
}


static void Flush_Message_Queue()
{   
    u64 clear_mask = Platform_Flags::resize | Platform_Flags::wants_to_exit;
    Inverse_Bit_Mask(&s_platform_state.flags, clear_mask);
    
    // This message loop resives messages we choose to send it. Using ThreadMessage and PostMessage.
    WMessage message;
    while(PeekMessageA(&message, 0, 0, 0, PM_REMOVE))
    {
        switch(message.message)
        {
            case WM_SIZE:
            {
                WRect client_rect;
                GetClientRect(s_platform_state.window, &client_rect);
                
                s_platform_state.window_dimensions = 
                {
                    client_rect.right - client_rect.left,
                    client_rect.bottom - client_rect.top
                };
                
                s_platform_state.flags |= Platform_Flags::resize;
            }break;
            
            
            case WM_DESTROY:
            case WM_CLOSE:
            {
                s_platform_state.flags |= Platform_Flags::wants_to_exit;
            }break;
            
            case WM_ACTIVATEAPP:
            {
                Inverse_Bit_Mask(&s_platform_state.flags, Platform_Flags::focused);
                b32 active = (b32)message.wparam;
                if(active)
                {
                    s_platform_state.flags |= Platform_Flags::focused;
                }
            }break;
        }
    }
    
    
    // Update controller state.
    {
        for(s32 i = 0; i < Array_Lenght(s_platform_state.controller_state); ++i)
        {
            s_platform_state.controller_state[i].m_prev = s_platform_state.controller_state[i].m_curr;
            
            Controller_State::Data* controller = &(s_platform_state.controller_state + i)->m_curr;
            
            if(_XInputGetState)
            {
                XInputState xinput_state;
                f64 time_stamp = Platform_Get_Time_Stamp();
                if(time_stamp >= s_platform_state.next_controler_test_time)
                {
                    u32 result = _XInputGetState(i, &xinput_state);
                    if(result == ERROR_SUCCESS)
                    {
                        XInputGamepad pad = xinput_state.gamepad;
                        controller->button_states = 0;
                        
                        for(u16 button_idx = 0; button_idx < (u16)Button::BUTTON_COUNT; ++button_idx)
                        {
                            if(s_controller_map[button_idx] & pad.buttons)
                            {
                                controller->button_states |= (1 << button_idx);
                            }
                        }
                        
                        constexpr u32 negative_max_range = 32768;
                        constexpr u32 positive_max_range = 32767;
                        
                        if(pad.thumb_left_x < 0)
                        {
                            controller->l_thumb_x = (f32)pad.thumb_left_x / negative_max_range;
                        }
                        else
                        {
                            controller->l_thumb_x = (f32)pad.thumb_left_x / positive_max_range;
                        }
                        
                        if(pad.thumb_left_y < 0)
                        {
                            controller->l_thumb_y = (f32)pad.thumb_left_y / negative_max_range;
                        }
                        else
                        {
                            controller->l_thumb_y = (f32)pad.thumb_left_y / positive_max_range;
                        }
                        
                        
                        if(pad.thumb_right_x < 0)
                        {
                            controller->r_thumb_x = (f32)pad.thumb_right_x / negative_max_range;
                        }
                        else
                        {
                            controller->r_thumb_x = (f32)pad.thumb_right_x / positive_max_range;
                        }
                        
                        if(pad.thumb_right_y < 0)
                        {
                            controller->r_thumb_y = (f32)pad.thumb_right_y / negative_max_range;
                        }
                        else
                        {
                            controller->r_thumb_y = (f32)pad.thumb_right_y / positive_max_range;
                        }
                        
                        constexpr u32 trigger_max_range = 255;
                        
                        controller->l_trig = (f32)pad.left_trigger / trigger_max_range;
                        controller->r_trig = (f32)pad.right_trigger / trigger_max_range;
                    }
                    else if(result == ERROR_DEVICE_NOT_CONNECTED)
                    {
                        s_platform_state.controller_state[i] = {0};
                        s_platform_state.next_controler_test_time = time_stamp + 3;
                    }
                }
            }
        }
    }
}


static void* Platform_Allocate_Memory(u32 amount, u32* out_amount)
{
    u32 page = u32(s_platform_state.system_info.page_size);
    u32 p_count = amount / page;
    if(p_count * page < amount)
    {
        p_count += 1;
        u32 new_amount = p_count * page;
        Assert(new_amount >= amount);
        amount = new_amount;
        
        if(out_amount)
        {
            *out_amount = amount;
        }
    }
    
    void* memory = VirtualAlloc(0, amount, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    return memory;
}


static void Platform_Free_Memory(void* memory)
{
    b32 success = VirtualFree(memory, 0, MEM_RELEASE);
    Assert(success);
}

static v2s Platform_Get_Cursor_Position()
{
    // TODO: cashe the border values.

    WPoint cursor_p;
    b32 get_cursor_pos_result = GetCursorPos(&cursor_p);
    
    WRect window_rect;
    b32 get_window_rect_result = GetWindowRect(s_platform_state.window, &window_rect);
    
    WRect client_rect;
    b32 get_client_rect_result = GetClientRect(s_platform_state.window, &client_rect);
    
    s32 window_width = window_rect.right - window_rect.left;
    s32 window_height = window_rect.bottom - window_rect.top;
    
    s32 border_width = (window_width - client_rect.right) / 2;
    s32 border_height = (window_height - client_rect.bottom) - border_width;
    
    // x and y are now relative to the top left of the windows client area.
    s32 x = cursor_p.x - (window_rect.left + border_width);
    s32 y = cursor_p.y - (window_rect.top + border_height);
    
    // Next they are converted to be relative to the bottom left. Increasing up and rightwards.
    y = (y - client_rect.bottom) * -1;
    
    // Next convert the screenspace coordinates into pixel space coordinates.
    x = s32(x * ((f32)s_platform_state.window_dimensions.x / (f32)client_rect.right));
    y = s32(y * ((f32)s_platform_state.window_dimensions.y / (f32)client_rect.bottom));
    
    return v2s{x, y};
}


static bool Platform_Get_Keyboard_Key_Down(Key_Code key_code)
{
    bool result = false;
    
    if(key_code != Key_Code::NONE)
    {
        result = (GetKeyState(s_windows_keycode_map[(u32)key_code]) & (1 << 15)) > 0;
    }
    
    return result;
}


static f64 Platform_Get_Time_Stamp()
{
    // Memory is memory, so fuck your LARGE_INTEGERS!
    
    u64 time_stamp;
    QueryPerformanceCounter(&time_stamp);
    return f64(time_stamp) / s_platform_state.system_clock_freqeuncy;
}


static f64 Platform_Get_Frame_Time()
{
    return s_platform_state.frame_time;
}


static u64 Platform_Get_Frame_Count()
{
    return s_platform_state.frame_count;
}


static v2s Platform_Get_Window_Dimensions()
{
    return s_platform_state.window_dimensions;
}


static Controller_State Platform_Get_Controller_State(u32 controller)
{
    Assert(controller < Array_Lenght(s_platform_state.controller_state));
    return s_platform_state.controller_state[controller]; 
}


static bool Open_File_Handle(char* path, u32 access, u32 creation_dispotion, WHandle* out_handle)
{
    Assert(path);
    Assert(out_handle);
    
    WHandle handle = CreateFileA(path, access, 0, 0, creation_dispotion, FILE_ATTRIBUTE_NORMAL, 0);
    bool result = handle != INVALID_HANDLE_VALUE;
    *out_handle = handle;
    
    return result;
}


static bool Platform_Get_File_Size(char* path, u32* out_size)
{
    Assert(out_size);
    
    bool result = false;
    
    WHandle file_handle;
    if(Open_File_Handle(path, GENERIC_READ, OPEN_EXISTING, &file_handle))
    {
        *out_size = GetFileSize(file_handle, 0);
        CloseHandle(file_handle);
        result = true;
    }
    
    return result;
}


static bool Platform_Read_File(char* path, u8* buffer, u32 buffer_size)
{
    bool file_read = 0;
    
    WHandle file_handle;
    if(Open_File_Handle(path, GENERIC_READ, OPEN_EXISTING, &file_handle))
    {
        u32 bytes_read;
        file_read = ReadFile(file_handle, buffer, buffer_size, &bytes_read, 0);
        
        Assert(bytes_read == buffer_size);
        CloseHandle(file_handle);
    }
    
    return file_read;
}


static bool Platform_Write_File(char* path, char* buffer, u32 buffer_size)
{
    b32 result = false;
    
    WHandle file_handle;
    if(Open_File_Handle(path, GENERIC_WRITE, CREATE_ALWAYS, &file_handle))
    {
        u32 bytes_written;
        result = WriteFile(file_handle, buffer, buffer_size, &bytes_written, 0);
        Assert(bytes_written == buffer_size);
        CloseHandle(file_handle);
    }
    
    
    return result;
    
}


static void Platform_Set_Fullscreen(bool enabled)
{
    if(((s_platform_state.flags & Platform_Flags::fullscreen) > 0) != enabled)
    {
        u32 dwStyle = GetWindowLongA(s_platform_state.window, GWL_STYLE);
        
        if(enabled)
        {
            if(GetWindowPlacement(s_platform_state.window, &s_platform_state.window_placement))
            {
                WMonitorInfo monitor_info = {sizeof(monitor_info)};
                
                if(GetMonitorInfoA(MonitorFromWindow(s_platform_state.window, MONITOR_DEFAULTTOPRIMARY), &monitor_info))
                {
                    s_platform_state.flags |= Platform_Flags::fullscreen;
                    
                    SetWindowLongA(s_platform_state.window, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                    SetWindowPos
                    (
                        s_platform_state.window,
                        HWND_TOP,
                        monitor_info.monitor.left,
                        monitor_info.monitor.top,
                        monitor_info.monitor.right - monitor_info.monitor.left,
                        monitor_info.monitor.bottom - monitor_info.monitor.top,
                        SWP_NOOWNERZORDER | SWP_FRAMECHANGED
                    );
                }
            }
        }
        else
        {
            Inverse_Bit_Mask(&s_platform_state.flags, Platform_Flags::fullscreen);
            
            SetWindowLongA(s_platform_state.window, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
            SetWindowPlacement(s_platform_state.window, &s_platform_state.window_placement);
            SetWindowPos(s_platform_state.window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
        }
    }
}



static void Update_Frame_Time()
{
    u64 time_stamp;
    QueryPerformanceCounter(&time_stamp);
    
    s64 counter_elapsed_time = time_stamp - s_platform_state.last_time_stamp;
    
    s_platform_state.last_time_stamp = time_stamp;
    
    f64 frame_time = f64(counter_elapsed_time) / s_platform_state.system_clock_freqeuncy;
    s_platform_state.frame_time = frame_time;
}


static void Software_Renderer_SwapBuffers()
{
    Assert(s_software_render_target.buffer);
    
    WBitMapInfoHeader header = {};
    header.size = sizeof(header);
    header.width  = s_software_render_target.width;
    header.height = s_software_render_target.height;
    header.planes = 1;
    header.bit_count = sizeof(Color) * 8;
    
    WBitmapInfo info = {header};
    
    StretchDIBits(
        s_platform_state.device_context,
        0, 
        0,
        s_platform_state.window_dimensions.x,
        s_platform_state.window_dimensions.y,
        0, 
        0,
        s_software_render_target.width,
        s_software_render_target.height,
        s_software_render_target.buffer,
        &info,
        0,
        SRCCOPY
    );
}


static Color* Platform_Resize_Software_Render_Target_Pixel_Buffer(v2s dim)
{
    constexpr u32 SSE_padding = 3; // For SSE wide operations.
    
    Assert(dim.x >= 0);
    Assert(dim.y >= 0);
    
    s_software_render_target.width = dim.x;
    s_software_render_target.height = dim.y;
    
    u32 pixel_count = dim.x * dim.y;
    u32 mem_size = (pixel_count + SSE_padding) * sizeof(*s_software_render_target.buffer);
    if(s_software_render_target.allocated_capacity < mem_size)
    {
        if(s_software_render_target.buffer)
        {
            VirtualFree(s_software_render_target.buffer, 0, MEM_RELEASE);
        }
        
        s_software_render_target.buffer = (Color*)Platform_Allocate_Memory(mem_size, &mem_size);
        s_software_render_target.allocated_capacity = mem_size;
    }
    
    return s_software_render_target.buffer;
}


static void Platform_Update()
{
    Software_Renderer_SwapBuffers();
    
    Flush_Message_Queue();
    
    Update_Frame_Time();
    
    s_platform_state.frame_count += 1;
}