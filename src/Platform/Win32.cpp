

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#include "Win32.h"


static Win32_App s_app;
static Win32_Bitmap s_bitmap;

typedef DWORD WINAPI x_input_get_state(DWORD dwUserIndex, XINPUT_STATE* state);
static x_input_get_state* XInputGetState_;
#define XInputGetState XInputGetState_

struct Window_Create_Info
{
    u32 window_width = 620 * 2; 
    u32 window_height = 480 * 2;
    u32 window_pos_x = CW_USEDEFAULT; 
    u32 window_pos_y = CW_USEDEFAULT; 
    const wchar_t* window_title = L"Nalkapeli";
};


static LRESULT Win32_Display_Window_Proc(HWND Window, UINT Message, WPARAM WParam, LPARAM LParam)
{
    LRESULT Result = 0;

    switch (Message)
    {
        // NOTE: For any message to get passed into the main thread they have to be included here!
        case WM_CLOSE:
        case WM_DESTROY:
        case WM_SIZE:
        case WM_CHAR:
        case WM_MOUSEWHEEL:
        case WM_ACTIVATEAPP:
        {
            PostThreadMessageW(s_app.main_thread_id, Message, WParam, LParam);
        } break;
        
        default:
        {
            Result = DefWindowProcW(Window, Message, WParam, LParam);
        } break;
    }

    return Result;
}


static u32* Win32_Resize_Pixel_Buffer(i32 new_width, i32 new_height)
{
    static constexpr u32 extra_pp = 3; // For SSE wide operations.
    
    Assert(new_width >= 0);
    Assert(new_height >= 0);
    
    s_bitmap.height = new_height;
    s_bitmap.width = new_width;
    
    s_bitmap.info.bmiHeader.biWidth = new_width;
    s_bitmap.info.bmiHeader.biHeight = new_height;
    
    u32 pixel_count = new_width * new_height;
    u32 required_memory_ammount = (pixel_count + extra_pp) * s_bitmap.bytes_per_pixel;
    if(s_bitmap.allocated_memory < required_memory_ammount)
    {
        if(s_bitmap.memory)
            VirtualFree(s_bitmap.memory, 0, MEM_RELEASE);
        
        s_bitmap.memory = (u32*)Win32_Allocate_Memory(required_memory_ammount, &required_memory_ammount);
        s_bitmap.allocated_memory = required_memory_ammount;
    }
    
    return s_bitmap.memory;
}


DWORD Win32_Create_Message_Thread(LPVOID param)
{
    Window_Create_Info* wci = (Window_Create_Info*)param;
    
    //Windows behavior here is whacky.
    //https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
    
    if(wci->window_pos_x != CW_USEDEFAULT && wci->window_pos_y == CW_USEDEFAULT)
        wci->window_pos_y = 0;
    
    if(wci->window_pos_y != CW_USEDEFAULT && wci->window_pos_x == CW_USEDEFAULT)
        wci->window_pos_x = 0;
    
    if(wci->window_height != CW_USEDEFAULT && wci->window_width == CW_USEDEFAULT)
        wci->window_width  = wci->window_height;
    
    if(wci->window_width != CW_USEDEFAULT && wci->window_height == CW_USEDEFAULT)
        wci->window_height = wci->window_width;
    
    s_app.window_width = wci->window_width;
    s_app.window_height = wci->window_height;
    
    WNDCLASSEXW WindowClass = {};
    WindowClass.cbSize = sizeof(WindowClass);
    WindowClass.lpfnWndProc = &Win32_Display_Window_Proc;
    WindowClass.hInstance = GetModuleHandleW(0);
    WindowClass.hIcon = LoadIconA(0, IDI_APPLICATION);
    WindowClass.hCursor = LoadCursorA(0, IDC_CROSS);
    WindowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    WindowClass.lpszClassName = L"Dangerous Class";
    RegisterClassExW(&WindowClass);
    
    s_app.window = CreateWindowExW(
        0,
        WindowClass.lpszClassName,
        wci->window_title,
        WS_OVERLAPPEDWINDOW|WS_VISIBLE,
        wci->window_pos_x,
        wci->window_pos_y,
        s_app.window_width,
        s_app.window_height,
        0,
        0,
        WindowClass.hInstance,
        0);
    
    {            
        RECT window_rect;
        BOOL get_window_rect_result = GetWindowRect(s_app.window, &window_rect);
        
        RECT client_rect;
        BOOL get_client_rect_result = GetClientRect(s_app.window, &client_rect);
        
        i32 _window_width = window_rect.right - window_rect.left;
        i32 _window_height = window_rect.bottom - window_rect.top;
        
        i32 border_width = (_window_width - client_rect.right);
        i32 border_height = (_window_height - client_rect.bottom);
        
        u32 scale = 1;
        
        i32 new_width = _window_width * scale + border_width;
        i32 new_height = _window_height * scale + border_height;
        
        SetWindowPos(
            s_app.window, 
            0, 
            window_rect.left, 
            window_rect.top, 
            new_width, 
            new_height, 
            0);
    }
    
    s_app.dc = GetDC(s_app.window);
    s_app.flags |= 1 << (u32)App_Flags::is_running;
    s_app.flags |= 1 << (u32)App_Flags::is_focused;
    s_app.flags |= 1 << (u32)App_Flags::cursor_is_visible;
    s_app.flags |= 1 << (u32)App_Flags::window_has_resized;
    
    
    // Fill in bitmap info.
    {
        s_bitmap.info.bmiHeader.biSize = sizeof(s_bitmap.info.bmiHeader);
        s_bitmap.info.bmiHeader.biPlanes = 1;
        s_bitmap.info.bmiHeader.biBitCount = sizeof(i32) * 8;
        s_bitmap.info.bmiHeader.biCompression = BI_RGB;
        
        Win32_Resize_Pixel_Buffer(s_app.window_width, s_app.window_height);
    }
    
    s_app.init_done = true;
    
    // NOTE(casey): This thread can just idle for the rest of the run, forwarding
    // messages to the main thread that it thinks the main thread wants.
    for(;;)
    {
        MSG Message;
        
        GetMessageW(&Message, 0, 0, 0); //blocks untill there is a message to get.
        TranslateMessage(&Message);
        DispatchMessageW(&Message);
    }
    
    return 0;
}


static void Win32_Init(
    u32 window_width, 
    u32 window_height, 
    u32 window_pos_x, 
    u32 window_pos_y, 
    const wchar_t* window_title)
{
    GetSystemInfo(&s_app.sys_info);
    
    s_app.frame_time = 0;
    
    Window_Create_Info wci = 
    {
        window_width, 
        window_height, 
        window_pos_x, 
        window_pos_y, 
        window_title
    };
    
    
    // Try to load XInput
    { 
        HMODULE XInput_lib;
        if ((XInput_lib = LoadLibraryA("xinput1_4.dll")) || (XInput_lib = LoadLibraryA("xinput1_3.dll")))
        {
            XInputGetState = (x_input_get_state*)GetProcAddress(XInput_lib, "XInputGetState");
        }
    }
    
    
    //Init time counters
    {
        LARGE_INTEGER perf_freg;
        QueryPerformanceFrequency(&perf_freg);
        s_app.timer_counter_freg = perf_freg.QuadPart;
        QueryPerformanceCounter(&s_app.last_time_counter);
    }
    
    s_app.main_thread_id = GetCurrentThreadId();
    
    DWORD message_thread_id;
    CloseHandle(CreateThread(0, 0, Win32_Create_Message_Thread, &wci, 0, &message_thread_id));
    
    while(!s_app.init_done);
}


static void Win32_Flush_Events()
{
    s_app.typing_information = {};
    s_app.scroll_delta = 0;
    
    // This message loop resives messages we choose to send it. Using ThreadMessage and PostMessage.
    {
        MSG message;
        while(PeekMessage(&message, 0, 0, 0, PM_REMOVE))
        {
            switch(message.message)
            {
                case WM_SIZE:
                {
                    RECT client_rect;
                    GetClientRect(s_app.window, &client_rect);
                    
                    s_app.window_width = client_rect.right - client_rect.left;
                    s_app.window_height = client_rect.bottom - client_rect.top;
                    
                    s_app.flags |= (1 << (u32)App_Flags::window_has_resized);
                    
                    s_app.force_update_surface = true;
                }break;
                
                
                case WM_DESTROY:
                case WM_CLOSE:
                {
                    s_app.flags |= (1 << (u32)App_Flags::wants_to_exit);
                }break;
                
                
                case WM_CHAR:
                {
                    char c = (char)message.wParam;
                    
                    u32 lenght = Array_Lenght(s_app.typing_information.buffer);
                    if(s_app.typing_information.count < lenght)
                    {
                        s_app.typing_information.buffer[s_app.typing_information.count] = c;
                        s_app.typing_information.count += 1;
                    }
                    
                }break;
                
                
                case WM_MOUSEWHEEL:
                {
                    f32 wheel_delta = f32(GET_WHEEL_DELTA_WPARAM(message.wParam));
                    s_app.scroll_delta = wheel_delta / f32(WHEEL_DELTA);
                }break;
                
                
                case WM_ACTIVATEAPP:
                {
                    BOOL active = (BOOL)message.wParam;
                    
                    u8 n = (u8)App_Flags::is_focused;
                    s_app.flags = (s_app.flags & ~(1 << n)) | (active << n);
                    s_app.force_update_surface = true;
                }break;
            }
        }        
    }
    
    // Update controller state.
    {
        for (i32 i = 0; i < s_app.max_controllers; i++)
        {
            s_app.controller_state[i].m_prev = s_app.controller_state[i].m_curr;
            
            if (XInputGetState)
            {
                XINPUT_STATE xinput_state;
                f64 time_stamp = Win32_Get_Time_Stamp();
                if(time_stamp >= s_app.next_controler_test_time)
                {
                    //TODO: Actually figure out the types and get rid of auto.
                    auto result = XInputGetState(i, &xinput_state);
                    if (result == ERROR_SUCCESS)
                    {
                        auto& pad = xinput_state.Gamepad;
                        s_app.controller_state[i].m_curr.button_states = 0;
                        for (u16 button_idx = 0; button_idx < (u16)Button::BUTTON_COUNT; ++button_idx)
                            if (s_controller_map[button_idx] & pad.wButtons)
                                s_app.controller_state[i].m_curr.button_states = s_app.controller_state[i].m_curr.button_states | (1 << button_idx);
                        
                        static constexpr u32 negative_max_range = 32768;
                        static constexpr u32 positive_max_range = 32767;
                        
                        if (pad.sThumbLX < 0)
                            s_app.controller_state[i].m_curr.l_thumb_x = (f32)pad.sThumbLX / negative_max_range;
                        else
                            s_app.controller_state[i].m_curr.l_thumb_x = (f32)pad.sThumbLX / positive_max_range;
                        
                        if (pad.sThumbLY < 0)
                            s_app.controller_state[i].m_curr.l_thumb_y = (f32)pad.sThumbLY / negative_max_range;
                        else
                            s_app.controller_state[i].m_curr.l_thumb_y = (f32)pad.sThumbLY / positive_max_range;
                        
                        if (pad.sThumbRX < 0)
                            s_app.controller_state[i].m_curr.r_thumb_x = (f32)pad.sThumbRX / negative_max_range;
                        else
                            s_app.controller_state[i].m_curr.r_thumb_x = (f32)pad.sThumbRX / positive_max_range;
                        
                        if (pad.sThumbRY < 0)
                            s_app.controller_state[i].m_curr.r_thumb_y = (f32)pad.sThumbRY / negative_max_range;
                        else
                            s_app.controller_state[i].m_curr.r_thumb_y = (f32)pad.sThumbRY / positive_max_range;
                        
                        static constexpr u32 trigger_max_range = 255;
                        
                        s_app.controller_state[i].m_curr.l_trig = (f32)pad.bLeftTrigger / trigger_max_range;
                        s_app.controller_state[i].m_curr.r_trig = (f32)pad.bRightTrigger / trigger_max_range;
                    }
                    else if(result == ERROR_DEVICE_NOT_CONNECTED)
                    {
                        s_app.controller_state[i] = {0};
                        s_app.next_controler_test_time = time_stamp + 3;
                    }
                
                }
            }
        }
    }
}


static void Win32_Update_Surface(bool update_from_game)
{
    s_app.flags &= ~(1 << (u32)App_Flags::window_has_resized);
    
    if(s_bitmap.memory && (update_from_game || s_app.force_update_surface))
    {
        // Start_Scope_Timer(blit_surface);
        
        s_app.force_update_surface = false;
        StretchDIBits
        (
            s_app.dc,
            0, 0,
            s_app.window_width,
            s_app.window_height,
            0, 0,
            s_bitmap.width,
            s_bitmap.height,
            s_bitmap.memory,
            &s_bitmap.info,
            DIB_RGB_COLORS,
            SRCCOPY
        );
    }
    
    s_app.frame_counter += 1;
}


static void Win32_Set_Flag(App_Flags flag, bool value)
{
    u32 iflag = (u32)flag;
    
    switch(flag)
    {
        case App_Flags::is_running:
        {
            s_app.flags = (s_app.flags & ~(1 << iflag)) | (value << iflag);
        }break;
        
        
        case App_Flags::wants_to_exit:
        {
            s_app.flags = (s_app.flags & ~(1 << iflag)) | (value << iflag);
        }break;
        
        
        case App_Flags::is_focused:
        {
            //TODO: Make so this can be used to unfocus the window...
            // if that's even possible.
            Terminate;
        }break;
        
        case App_Flags::cursor_is_visible:
        {
            // TODO: Only hide the cursor in the client area of the window.
            bool cursor_visiblity = (s_app.flags & (1 << (u32)App_Flags::cursor_is_visible)) > 0;
            if(cursor_visiblity == value)
                return;
            
            s_app.flags ^= (1 << (u32)App_Flags::cursor_is_visible);
            
            ShowCursor(value);
        }break;
        
        case App_Flags::is_fullscreen:
        {
            bool is_fullscreen = (s_app.flags & (1 << iflag)) > 0;
            if(is_fullscreen == value)
            return;
            
            s_app.force_update_surface = true;
            DWORD dwStyle = GetWindowLongA(s_app.window, GWL_STYLE);
            
            // TODO: Does this send a resize message?
            
            if(value) // Set fullscreen to true.
            {
                
                if (!GetWindowPlacement(s_app.window, &s_app.window_placement))
                    Terminate;
                
                MONITORINFO mi = { sizeof(mi) };
                
                if (!GetMonitorInfoA(MonitorFromWindow(s_app.window, MONITOR_DEFAULTTOPRIMARY), &mi))
                Terminate;
                
                SetWindowLongA(s_app.window, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
                SetWindowPos
                (
                    s_app.window,
                    HWND_TOP,
                    mi.rcMonitor.left,
                    mi.rcMonitor.top,
                    mi.rcMonitor.right - mi.rcMonitor.left,
                    mi.rcMonitor.bottom - mi.rcMonitor.top,
                    SWP_NOOWNERZORDER | SWP_FRAMECHANGED
                );
            }
            
            // Set fullscreen to false.
            else
            {
                SetWindowLongA(s_app.window, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
                SetWindowPlacement(s_app.window, &s_app.window_placement);
                SetWindowPos(s_app.window, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
            }
            
            s_app.flags ^= (1 << iflag);
        }break;
        
        default:
            // Unhandeled flag
            Terminate;
    }
}

static f64 Win32_Update_Frame_Time()
{
    //Update timing info.    
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    
    i64 counter_elapsed_time = current_time.QuadPart - s_app.last_time_counter.QuadPart;
    s_app.last_time_counter = current_time;    
    
    s_app.frame_time = f64(counter_elapsed_time) / s_app.timer_counter_freg;
    
    return s_app.frame_time;
}


static u32 Win32_Get_Window_Width()
{
    return s_app.window_width;
}


static u32 Win32_Get_Window_Height()
{
    return s_app.window_height;
}


static Pixel_Buffer Win32_Get_Pixel_Buffer()
{
    Pixel_Buffer result;
    result.memory = s_bitmap.memory;
    result.width = s_bitmap.width;
    result.height = s_bitmap.height;
    
    return result;
}


static i32 Win32_Get_Pixel_Buffer_Width()
{
    return s_bitmap.width;
}


static i32 Win32_Get_Pixel_Buffer_Height()
{
    return s_bitmap.height;
}


static u32 Win32_Get_Flags()
{
    return s_app.flags;
}


static Controller_State Win32_Get_Controller_State(i32 contoller_idx)
{
    Assert(contoller_idx >= 0 && contoller_idx < s_app.max_controllers);
    return s_app.controller_state[contoller_idx]; 
}


static f64 Win32_Get_Time_Stamp()
{
    //Update timing info.    
    LARGE_INTEGER current_time;
    QueryPerformanceCounter(&current_time);
    return f64(current_time.QuadPart) / s_app.timer_counter_freg;
}


static u64 Win32_Get_CPU_Time_Stamp()
{
    return __rdtsc();
}


static v2i Win32_Get_Cursor_Position()
{
    // TODO: cashe the border values.

    POINT cursor_p;
    BOOL get_cursor_pos_result = GetCursorPos(&cursor_p);
    
    RECT window_rect;
    BOOL get_window_rect_result = GetWindowRect(s_app.window, &window_rect);
    
    RECT client_rect;
    BOOL get_client_rect_result = GetClientRect(s_app.window, &client_rect);
    
    i32 window_width = window_rect.right - window_rect.left;
    i32 window_height = window_rect.bottom - window_rect.top;
    
    i32 border_width = (window_width - client_rect.right) / 2;
    i32 border_height = (window_height - client_rect.bottom) - border_width;
    
    // x and y are now relative to the top left of the windows client area.
    i32 x = cursor_p.x - (window_rect.left + border_width);
    i32 y = cursor_p.y - (window_rect.top + border_height);
    
    // Next they are converted to be relative to the bottom left. Increasing up and rightwards.
    y = (y - client_rect.bottom) * -1;
    
    // Next convert the screenspace coordinates into pixel space coordinates.
    x = i32(x * ((f32)s_bitmap.width / (f32)client_rect.right));
    y = i32(y * ((f32)s_bitmap.height / (f32)client_rect.bottom));
    
    return v2i{x, y};

}


static bool Win32_Get_Keyboard_Key_Down(Key_Code key_code)
{
    bool result = false;
    
    if(key_code != Key_Code::NONE)
        result = (GetKeyState(s_keycode_map[(u32)key_code]) & (1 << 15)) > 0;
    
    return result;
}


static u64 Win32_Get_Frame_Count()
{
    return s_app.frame_counter;
}


static bool Win32_Write_File(const char* file_name, u8* buffer, u32 bytes_to_write)
{
    HANDLE file = CreateFileA
    (
        file_name, 
        GENERIC_WRITE,
        0,
        0,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    
    if(file == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        return false;
    }
    
    DWORD bytes_written;
    BOOL write_succeeded = WriteFile
    (
        file,
        buffer,
        bytes_to_write,
        &bytes_written,
        0
    );
    
    Assert(bytes_to_write == bytes_written);
    
    CloseHandle(file);
    return write_succeeded;
}


static bool Win32_Get_File_Size(const char* file_name, u32* out_file_size)
{
    HANDLE file = CreateFileA
    (
        file_name, 
        GENERIC_READ,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    
    if(file == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        *out_file_size = 0;
        return false;
    }
    
    *out_file_size = GetFileSize(file, 0);
    
    CloseHandle(file);
    return true;
}


static bool Win32_Read_File(const char* file_name, u8* out_buffer, u32 bytes_to_read)
{
    HANDLE file = CreateFileA
    (
        file_name, 
        GENERIC_READ,
        0,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    
    if(file == INVALID_HANDLE_VALUE)
    {
        DWORD error = GetLastError();
        return false;
    }
    
    DWORD bytes_read;
    BOOL file_read = ReadFile
    (
        file,
        out_buffer,
        bytes_to_read,
        &bytes_read,
        0
    );
    
    Assert(bytes_read == bytes_to_read);
    
    CloseHandle(file);
    return file_read;
}


static Char_Array Win32_Get_Typing_Information()
{
    return s_app.typing_information;
}


static char* Win32_Get_Clipboard_Data_As_Text()
{
    char* result = 0;
    
    if (OpenClipboard(NULL))
    {
        HANDLE h = GetClipboardData(CF_TEXT);
        result = (char*)h;
        
        CloseClipboard();        
    }
    
    return result;
}


static void Win32_Set_Clipboard_Data_As_Text(char* data, u32 lenght)
{
    // Set clipboard data
    if(OpenClipboard(NULL)) 
    {
        // Allocate string for cwd
        HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, lenght + 1);
        char* dest = (char*)GlobalLock(handle);
        dest[lenght] = 0;
        Mem_Copy(dest, data, lenght);
        
        GlobalUnlock(handle);
        
        EmptyClipboard();
        
        SetClipboardData(CF_TEXT, handle);
        
        CloseClipboard();
    }
}


static f32 Win32_Get_Scroll_Wheel_Delta()
{
    return s_app.scroll_delta;
}


static Create_Directory_Result::T Win32_Create_Directory(char* path)
{
    BOOL win_result = CreateDirectoryA(path, 0);
    Create_Directory_Result::T result;
    if(!win_result)
    {
        switch(GetLastError())
        {
            case ERROR_ALREADY_EXISTS:
            {
                result = Create_Directory_Result::file_already_exists;
            }break;
            
            case ERROR_PATH_NOT_FOUND:
            {
                result = Create_Directory_Result::path_not_found;
            }break;
        }
    }
    else
    {
        result = Create_Directory_Result::success;
    }
    
    return result;
}


static void* Win32_Allocate_Memory(u32 amount, u32* out_amount)
{
    u32 page = u32(s_app.sys_info.dwPageSize);
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


static void Win32_Free_Memory(void* memory)
{
    VirtualFree(memory, 0, MEM_RELEASE);
}


static Dynamic_Array<String>* Win32_Search_Directory_For_Maching_Names(
    char* file_name, 
    Allocator_Shell* allocator)
{
    Dynamic_Array<String>* result = Create_Dynamic_Array<String>(allocator, 4);
    
    WIN32_FIND_DATAA find_data;
    
    HANDLE search_handle = FindFirstFileA(file_name, &find_data);
    if(search_handle != INVALID_HANDLE_VALUE)
    {
        {
            String* file_name = Push(&result, allocator);
            Init_String(file_name, allocator, &find_data.cFileName[0]);
        }
        
        while(FindNextFileA(search_handle, &find_data))
        {
            String* file_name = Push(&result, allocator);
            Init_String(file_name, allocator, &find_data.cFileName[0]);
        }
        
        FindClose(search_handle);
    }
    
    return result;
}


static f64 Win32_Get_Frame_Time()
{
    return s_app.frame_time;
}


static bool Win32_Open_Select_File_Dialog(
    char* result_buffer, 
    u32 result_buffer_size,
    char* dialog_title,
    char* filter_name,
    char** filter_file_types,
    u32 filter_type_count
    )
{
    Assert(result_buffer);
    Assert(result_buffer_size);
    
    Mem_Zero(result_buffer, result_buffer_size);
    BOOL result = 0;
    
    char filter_buffer[256];
    
    u32 write_head = Null_Terminated_Buffer_Lenght(filter_name);
    
    if(write_head < Array_Lenght(filter_buffer))
    {    
        Mem_Copy(filter_buffer, filter_name, write_head);
        filter_buffer[write_head++] = 0;
        
        char* wild_prefix = "*.";
        u32 wild_leng = Null_Terminated_Buffer_Lenght(wild_prefix);
        
        char post_fix = ';';
        
        
        for(u32 i = 0; i < filter_type_count; ++i)
        {
            u32 leng = Null_Terminated_Buffer_Lenght(filter_file_types[i]);
            
            if(write_head + leng + wild_leng + 1 < Array_Lenght(filter_buffer))
            {
                Mem_Copy(filter_buffer + write_head, wild_prefix, wild_leng);
                write_head += wild_leng;
                
                Mem_Copy(filter_buffer + write_head, filter_file_types[i], leng);
                write_head += leng;
                
                filter_buffer[write_head++] = post_fix;
                
            }
        }
        
        if(write_head + 1 < Array_Lenght(filter_buffer))
        {
            filter_buffer[write_head] = 0;
            filter_buffer[write_head + 1] = 0;
            
            OPENFILENAMEA of = {};
            of.lStructSize          = sizeof(OPENFILENAME);
            of.hwndOwner            = 0;
            of.hInstance            = 0;
            of.lpstrFilter          = filter_buffer;
            of.lpstrCustomFilter    = 0;
            of.nMaxCustFilter       = 0;
            of.nFilterIndex         = 0;
            of.lpstrFile            = (char*)result_buffer;
            of.nMaxFile             = result_buffer_size;
            of.lpstrFileTitle       = 0;
            of.nMaxFileTitle        = 0;
            of.lpstrInitialDir      = 0;
            of.lpstrTitle           = dialog_title; 
            of.Flags                = 0;
            of.nFileOffset          = 0;
            of.nFileExtension       = 0;
            of.lpstrDefExt          = 0;
            of.lCustData            = 0;
            of.lpfnHook             = 0;
            of.lpTemplateName       = 0;
            //of.lpEditInfo         = 0;
            //of.lpstrPrompt        = 0;
            //of.*pvReserved        = 0;
            of.FlagsEx              = 0;
            of.dwReserved           = 0;


            LPOPENFILENAMEA dialog = &of;
            result = GetOpenFileNameA(dialog);
        }
    }
    
    return result;
}


static bool Win32_Get_Executable_Path(char* result_buffer, u32 result_buffer_size)
{
    Assert(result_buffer);
    Assert(result_buffer_size);
    
    DWORD bits_written = GetModuleFileNameA(0, result_buffer, result_buffer_size);
    
    for(char* c = result_buffer + bits_written; c >= result_buffer; --c)
    {
        if(*c == '\\')
        {
            *(c + 1) = 0;
            return true;
        }
    }
    
    result_buffer[0] = 0;
    return false;
}


static bool Win32_Delete_File(char* path)
{
    BOOL result = DeleteFile(path);
    return result;
}


static bool Win32_Move_File(char* path_old, char* path_new)
{
    BOOL result = MoveFile(
        path_old,
        path_new
    );
    
    return result;
}


static Platform_Calltable Win32_Get_Calltable()
{
    Platform_Calltable ct = {};
    ct.Get_Window_Width = Win32_Get_Window_Width;
    ct.Get_Window_Height = Win32_Get_Window_Height;
    ct.Get_Pixel_Buffer = Win32_Get_Pixel_Buffer;
    ct.Get_Pixel_Buffer_Width = Win32_Get_Pixel_Buffer_Width;
    ct.Get_Pixel_Buffer_Height = Win32_Get_Pixel_Buffer_Height;
    ct.Resize_Pixel_Buffer = Win32_Resize_Pixel_Buffer;
    ct.Get_Flags = Win32_Get_Flags;
    ct.Set_Flag = Win32_Set_Flag;
    ct.Get_Keyboard_Key_Down = Win32_Get_Keyboard_Key_Down;
    ct.Get_Controller_State = Win32_Get_Controller_State;
    ct.Get_Time_Stamp = Win32_Get_Time_Stamp;
    ct.Get_CPU_Time_Stamp = Win32_Get_CPU_Time_Stamp;
    ct.Get_Cursor_Position = Win32_Get_Cursor_Position;
    ct.Get_Frame_Count = Win32_Get_Frame_Count;
    ct.Write_File = Win32_Write_File;
    ct.Get_File_Size = Win32_Get_File_Size;
    ct.Read_File = Win32_Read_File;
    ct.Get_Typing_Information = Win32_Get_Typing_Information;
    ct.Get_Clipboard_Data_As_Text = Win32_Get_Clipboard_Data_As_Text;
    ct.Set_Clipboard_Data_As_Text = Win32_Set_Clipboard_Data_As_Text;
    ct.Get_Scroll_Wheel_Delta = Win32_Get_Scroll_Wheel_Delta;
    ct.Create_Directory = Win32_Create_Directory;
    ct.Allocate_Memory = Win32_Allocate_Memory;
    ct.Free_Memory = Win32_Free_Memory;
    ct.Search_Directory_For_Maching_Names = Win32_Search_Directory_For_Maching_Names;
    ct.Open_Select_File_Dialog = Win32_Open_Select_File_Dialog;
    ct.Get_Executable_Path = Win32_Get_Executable_Path;
    ct.Get_Frame_Time = Win32_Get_Frame_Time;
    ct.Delete_File = Win32_Delete_File;
    ct.Move_File = Win32_Move_File;
    
    return ct;
}

#ifdef INSTRUMENTATION
#include <stdio.h>

static constexpr char* __TIMING_FORMAT_STRING = 
"Counter: %s:\n\
\t-Hits:%I64u\n\
\t-Cycles:%-20I64u Lowest:%-20I64u\n\
\t-C/H:%-23I64u Lowest:%-20I64u Avg:%-20I64u\n";

static inline void Win32_Report_Timing_Results()
{
    /*
    
    This is trying to be nothing but a quick hack for displaying timing results.
    The code is not good and is not trying to be good.
    
    */
    
    bool reset_records = Win32_Get_Keyboard_Key_Down(Key_Code::RALT) && Win32_Get_Keyboard_Key_Down(Key_Code::P);
    
    static bool output_key_last_state = false;
    bool output_key_state = Win32_Get_Keyboard_Key_Down(Key_Code::RALT) && Win32_Get_Keyboard_Key_Down(Key_Code::O);
    
    static bool output_results = false;
    if(output_key_last_state == false && output_key_state == true)
    {
        output_results = !output_results;
        reset_records = true;
    }
    output_key_last_state = output_key_state;
    
    char text_buffer[256];
    
    for(u64 i = 0; i < u64(__Time_Table_Names::COUNT); ++i)
    {
        Instrumentation_Cycle_Measurement* icm = __timing_table + i;
        char* label_name = __timing_table_labels[i];
        
        if(icm->hit_count > 0)
        {
            Assert(icm->accumulated_cycle_count > 0);
            
            icm->total_cycle_count += icm->accumulated_cycle_count;
            icm->total_hit_count += icm->hit_count;
            
            u64 cycles_per_hit = icm->accumulated_cycle_count / icm->hit_count;
            u64 avg_cycles_per_hit = icm->total_cycle_count / icm->total_hit_count;
            
            if(reset_records || icm->lowest_cycle_count == 0)
            {
                icm->lowest_cycle_count = icm->accumulated_cycle_count;
                icm->lowest_cycles_per_hit = cycles_per_hit;
                icm->total_cycle_count = 0;
                icm->total_hit_count = 0;
            }
            else
            {
                if(icm->accumulated_cycle_count < icm->lowest_cycle_count)
                    icm->lowest_cycle_count = icm->accumulated_cycle_count;
                
                if(cycles_per_hit < icm->lowest_cycles_per_hit)
                    icm->lowest_cycles_per_hit = cycles_per_hit;
            }
            
            
            if(output_results)
            {
                _snprintf_s(
                    text_buffer, 
                    sizeof(text_buffer), 
                    __TIMING_FORMAT_STRING,
                    label_name, 
                    icm->hit_count, 
                    icm->accumulated_cycle_count, icm->lowest_cycle_count,
                    cycles_per_hit, icm->lowest_cycles_per_hit, avg_cycles_per_hit);
                
                OutputDebugStringA(text_buffer);
            }
        }
        
        icm->hit_count = 0;
        icm->accumulated_cycle_count = 0;
    }
    
    if(output_results)
        OutputDebugStringA("\n\n\n");
}
#endif