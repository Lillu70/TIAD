
#pragma once


// This link explains what Windowses weird typedefs mean. I don't use them, but it's an usefull resource.
// https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types


#define AZEWIN_DEFAULT_COMPILE  1
#define AZEWIN_COMPILE_KERNEL32 0
#define AZEWIN_COMPILE_USER32   0
#define AZEWIN_COMPILE_GDI32    0
#define AZEWIN_COMPILE_OPENGL32 0


typedef void* WHandle;
typedef void* WMenuHandle;
typedef void* WIconHandle;
typedef void* WBrushHandle;
typedef void* WWindowHandle;
typedef void* WModuleHandle;
typedef void* WCursorHandle;
typedef void* WDeviceHandle;
typedef void* WGlobalHandle;
typedef void* WMonitorHandle;
typedef void* WInstanceHandle;
typedef void* WGLRenderContextHandle;


typedef s32 (__stdcall *WProcedure)();
typedef u32 (__stdcall *WThreadRoutine)(void* thread_parameter);
typedef s64 (__stdcall *WWindowProcedure)(WWindowHandle window, u32 message, u64 wparam, s64 lparam); // https://devblogs.microsoft.com/oldnewthing/20031125-00/?p=41713


#if AZEWIN_DEFAULT_COMPILE || 0
static inline char* WMakeResource(u16 idc) { return (char*)idc; }
#endif


// https://learn.microsoft.com/en-us/previous-versions/windows/desktop/legacy/aa379560(v=vs.85)
struct WSecurityAttributes
{
    u32 length;
    void* security_descriptor;
    b32 inherit_handle;
};


// https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/ns-sysinfoapi-system_info
struct WSystemInfo
{
    union 
    {
        u32 oem_id;
        struct
        {
            u16 processor_architecture;
            u16 reserved;
        };
    };
    u32 page_size;
    void* minimum_application_address;
    void* maximum_application_address;
    u32* active_processor_mask;
    u32 number_of_processors;
    u32 obsolete;
    u32 allocation_granularity;
    u16 processor_level;
    u16 processor_revision;
};


// https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-rect
struct WRect
{
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;
};


// https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-point
struct WPoint
{
    s32 x;
    s32 y;
};


// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-msg
struct WMessage
{
    WWindowHandle window_handle;
    u32 message;
    u64 wparam;
    s64 lparam;
    u32 time;
    WPoint point;
    u32 is_private;
};


// https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassa
struct WWindowClass
{
    u32 style;
    WWindowProcedure window_procedure;
    s32 cls_extra;
    s32 window_extra;
    WInstanceHandle instance;
    WIconHandle icon;
    WCursorHandle cursor;
    WBrushHandle background_brush;
    char* menu_name;
    char* class_name;
};


// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-pixelformatdescriptor
struct WPixel_Format_Descriptor
{
  u16  size;
  u16  version;
  u32 flags;
  u8  pixel_type;
  u8  color_bits;
  u8  red_bitts;
  u8  red_shift;
  u8  green_bits;
  u8  green_shift;
  u8  blue_bits;
  u8  blue_shift;
  u8  alpha_bits;
  u8  alpha_shift;
  u8  accum_bits;
  u8  accum_red_bits;
  u8  accum_green_bits;
  u8  accum_blue_bits;
  u8  accum_alpha_bits;
  u8  depth_bits;
  u8  stencil_bits;
  u8  aux_buffers;
  u8  layer_type;
  u8  reserved;
  u32 layer_mask;
  u32 visible_mask;
  u32 damage_mask;
};


struct WMonitorInfo 
{
    u32     size;
    WRect   monitor;
    WRect   work;
    u32     flags;
};


struct WWindowPlacement
{
    u32 length;
    u32 flags;
    u32 show_command;
    WPoint min_position;
    WPoint max_position;
    WRect normal_position;
    WRect device;
};


// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-rgbquad
struct WRGBQuad
{
    u8 blue;
    u8 green;
    u8 red;
    u8 reserved;
};


// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
struct WBitMapInfoHeader
{
    u32 size;
    s32 width;
    s32 height;
    u16 planes;
    u16 bit_count;
    u32 compression;
    u32 size_image;
    s32 pixels_per_meter_x;
    s32 pixels_per_meter_y;
    u32 colors_used;
    u32 colors_important;
};


// https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfo
struct WBitmapInfo
{
    WBitMapInfoHeader header;
    WRGBQuad colors[1];
};



 #if AZEWIN_DEFAULT_COMPILE || 0
// https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-get_wheel_delta_wparam
static inline s16 WGetWheelDeltaWParam(u64 wparam) { return (s16)((u16)((((u64)(wparam)) >> 16) & 0xffff)); }
#endif
 
 

// --- WinMain signature
/*
    The user-provided entry point for a graphical Windows-based application.
    WinMain is the conventional name used for the application entry point.
    
    int __clrcall WinMain(
        [in]           HINSTANCE hInstance,
        [in, optional] HINSTANCE hPrevInstance,
        [in]           LPSTR     lpCmdLine,
        [in]           int       nShowCmd
    );
        
    https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
*/
// s32 WinMain(WInstanceHandle instance, WInstanceHandle pre_instance, char* cmd, s32 show_command)



// --- GetLastError import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the calling thread's last-error code value. 
    The last-error code is maintained on a per-thread basis. 
    Multiple threads do not overwrite each other's last-error code.
    
    _Post_equals_last_error_ DWORD GetLastError();
    
    https://learn.microsoft.com/en-us/windows/win32/api/errhandlingapi/nf-errhandlingapi-getlasterror
*/
extern "C" __declspec(dllimport) u32 __stdcall GetLastError(void);
#endif



// --- QueryPerformanceCounter import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/* 
    Retrieves the current value of the performance counter,
    which is a high resolution (<1us) time stamp that can be used for time-interval measurements.
    
    BOOL QueryPerformanceCounter(
        [out] LARGE_INTEGER *lpPerformanceCount
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
*/
extern "C" __declspec(dllimport) b32  __stdcall QueryPerformanceCounter(u64* out_result);
#endif



// --- QueryPerformanceFrequency import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the frequency of the performance counter. 
    The frequency of the performance counter is fixed at system boot and is consistent across all processors. 
    Therefore, the frequency need only be queried upon application initialization, and the result can be cached.

    BOOL QueryPerformanceFrequency(
        [out] LARGE_INTEGER *lpFrequency
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
*/
extern "C" __declspec(dllimport) b32 __stdcall QueryPerformanceFrequency(u64* out_result);
#endif



// --- OutputDebugStringA import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Sends a string to the debugger for display.
    
    void OutputDebugStringA(
        [in, optional] LPCSTR lpOutputString
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/debugapi/nf-debugapi-outputdebugstringa
*/
extern "C" __declspec(dllexport) void __stdcall OutputDebugStringA(char* string);
#endif



// --- LoadLibraryA import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0 
/*
    Loads the specified module into the address space of the calling process. 
    The specified module may cause other modules to be loaded.

    HMODULE LoadLibraryA(
        [in] LPCSTR lpLibFileName
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibrarya
*/
extern "C" __declspec(dllimport) WModuleHandle __stdcall LoadLibraryA(char* library_name);
#endif



// --- GetProcAddress import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the address of an exported function (also known as a procedure) 
    or variable from the specified dynamic-link library (DLL).
    
    FARPROC GetProcAddress(
        [in] HMODULE hModule,
        [in] LPCSTR  lpProcName
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
*/
extern "C" __declspec(dllimport) WProcedure __stdcall GetProcAddress(WModuleHandle module_handle, char* procedure_name);
#endif



// --- GetModuleFileNameA import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the fully qualified path for the file that contains the specified module. 
    The module must have been loaded by the current process.
    
    DWORD GetModuleFileNameA(
        [in, optional] HMODULE hModule,
        [out]          LPSTR   lpFilename,
        [in]           DWORD   nSize
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulefilenamea
*/
extern "C" __declspec(dllimport) u32 __stdcall GetModuleFileNameA(WModuleHandle module, char* out_file_name_buffer, u32 file_name_buffer_size);
#endif



// --- CreateThread import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Creates a thread to execute within the virtual address space of the calling process.
    
    HANDLE CreateThread(
        [in, optional]  LPSECURITY_ATTRIBUTES   lpThreadAttributes,
        [in]            SIZE_T                  dwStackSize,
        [in]            LPTHREAD_START_ROUTINE  lpStartAddress,
        [in, optional]  __drv_aliasesMem LPVOID lpParameter,
        [in]            DWORD                   dwCreationFlags,
        [out, optional] LPDWORD                 lpThreadId
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createthread
*/
extern "C" __declspec(dllimport) WHandle __stdcall CreateThread(WSecurityAttributes* thread_attributes, u64 stack_size, WThreadRoutine routine, void* parameter, u32 flags, u32* out_thread_id);
#endif



// --- GetCurrentThreadId import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the thread identifier of the calling thread.
    
    DWORD GetCurrentThreadId();
    
    https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getcurrentthreadid
*/
extern "C" __declspec(dllimport) u32 __stdcall GetCurrentThreadId(void);
#endif



// --- CloseHandle import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Closes an open object handle.
    
    BOOL CloseHandle(
        [in] HANDLE hObject
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/handleapi/nf-handleapi-closehandle
*/
extern "C" __declspec(dllimport) b32 __stdcall CloseHandle(WHandle handle);
#endif



// --- GetSystemInfo import
/// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves information about the current system.
    
    void GetSystemInfo(
        [out] LPSYSTEM_INFO lpSystemInfo
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-getsysteminfo
*/
extern "C" __declspec(dllimport) void __stdcall GetSystemInfo(WSystemInfo* out_system_info);
#endif



// --- GetModuleHandleA import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves a module handle for the specified module.
    The module must have been loaded by the calling process.
    
    HMODULE GetModuleHandleA(
        [in, optional] LPCSTR lpModuleName
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlea
*/
extern "C" __declspec(dllimport) WModuleHandle __stdcall GetModuleHandleA(char* module_name);
#endif



// --- VirtualAlloc import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Reserves, commits, or changes the state of a region of pages in the virtual address space of the calling process. 
    Memory allocated by this function is automatically initialized to zero.
    
    LPVOID VirtualAlloc(
        [in, optional] LPVOID lpAddress,
        [in]           SIZE_T dwSize,
        [in]           DWORD  flAllocationType,
        [in]           DWORD  flProtect
    );

    https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
*/
extern "C" __declspec(dllimport) void* __stdcall VirtualAlloc(void* address, u64 size, u32 type, u32 protect);
#endif



// --- VirtualFree import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Releases, decommits, or releases and decommits a region of pages within the virtual address space of the calling process.

    BOOL VirtualFree(
        [in] LPVOID lpAddress,
        [in] SIZE_T dwSize,
        [in] DWORD  dwFreeType
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualfree
*/
extern "C" __declspec(dllimport) b32 __stdcall VirtualFree(void* address, u64 size, u32 type);
#endif



// --- GlobalAlloc import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    DECLSPEC_ALLOCATOR HGLOBAL GlobalAlloc(
        [in] UINT   uFlags,
        [in] SIZE_T dwBytes
    )
*/
extern "C" __declspec(dllimport) WGlobalHandle __stdcall GlobalAlloc(s32 flags, u64 bytes);
#endif



// --- GlobalLock import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Locks a global memory object and returns a pointer to the first byte of the object's memory block.

    LPVOID GlobalLock(
        [in] HGLOBAL hMem
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-globallock
*/
extern "C" __declspec(dllimport) void* __stdcall GlobalLock(WGlobalHandle memory);
#endif



// --- GlobalUnlock import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Decrements the lock count associated with a memory object that was allocated with GMEM_MOVEABLE. 
    This function has no effect on memory objects allocated with GMEM_FIXED.
    
    BOOL GlobalUnlock(
        [in] HGLOBAL hMem
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-globalunlock
*/
extern "C" __declspec(dllimport) b32 __stdcall GlobalUnlock(WGlobalHandle memory);
#endif



// --- CreateFileA import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Creates or opens a file or I/O device.
    The most commonly used I/O devices are as follows: file, file stream, directory, physical disk, volume, console buffer, tape drive, communications resource, mailslot, and pipe. 
    The function returns a handle that can be used to access the file or device for various types of I/O depending on the file or device and the flags and attributes specified.

    To perform this operation as a transacted operation, which results in a handle that can be used for transacted I/O, use the CreateFileTransacted function.

    HANDLE CreateFileA(
        [in]           LPCSTR                lpFileName,
        [in]           DWORD                 dwDesiredAccess,
        [in]           DWORD                 dwShareMode,
        [in, optional] LPSECURITY_ATTRIBUTES lpSecurityAttributes,
        [in]           DWORD                 dwCreationDisposition,
        [in]           DWORD                 dwFlagsAndAttributes,
        [in, optional] HANDLE                hTemplateFile
    );

    https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
*/
extern "C" __declspec(dllimport) WHandle __stdcall CreateFileA(char* file_name, u32 desired_access, u32 share_mode, WSecurityAttributes* security_attribute, u32 creation_disposition, u32 flags_and_attributes, WHandle template_file);
#endif



// --- GetFileSize import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the size of the specified file, in bytes.

    DWORD GetFileSize(
      [in]            HANDLE  hFile,
      [out, optional] LPDWORD lpFileSizeHigh
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfilesize
*/
extern "C" __declspec(dllimport) u32 __stdcall GetFileSize(WHandle file, u32* file_size_high_bits);
#endif



// --- ReadFile import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Reads data from the specified file or input/output (I/O) device. 
    Reads occur at the position specified by the file pointer if supported by the device.

    BOOL ReadFile(
        [in]                HANDLE       hFile,
        [out]               LPVOID       lpBuffer,
        [in]                DWORD        nNumberOfBytesToRead,
        [out, optional]     LPDWORD      lpNumberOfBytesRead,
        [in, out, optional] LPOVERLAPPED lpOverlapped
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-readfile
*/
extern "C" __declspec(dllimport) b32 __stdcall ReadFile(WHandle file, void* buffer, u32 number_of_bytes_to_read, u32* number_of_bytes_read, void*);
#endif



// --- WriteFile import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Writes data to the specified file or input/output (I/O) device.

    BOOL WriteFile(
        [in]                HANDLE       hFile,
        [in]                LPCVOID      lpBuffer,
        [in]                DWORD        nNumberOfBytesToWrite,
        [out, optional]     LPDWORD      lpNumberOfBytesWritten,
        [in, out, optional] LPOVERLAPPED lpOverlapped
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile
*/
extern "C" __declspec(dllimport) b32 __stdcall WriteFile(WHandle file, void* buffer, u32 number_of_bytes_to_read, u32* number_of_bytes_read, void*);
#endif



// --- ReleaseSemaphore import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Increases the count of the specified semaphore object by a specified amount.

    BOOL ReleaseSemaphore(
        [in]            HANDLE hSemaphore,
        [in]            LONG   lReleaseCount,
        [out, optional] LPLONG lpPreviousCount
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-releasesemaphore
*/
extern "C" __declspec(dllimport) b32 __stdcall ReleaseSemaphore(WHandle semaphore, s32 release_count, s32* prev_release_count);
#endif


// --- CreateSemaphoreExA import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Creates or opens a named or unnamed semaphore object and returns a handle to the object.

    HANDLE CreateSemaphoreExA(
    [in, optional] LPSECURITY_ATTRIBUTES lpSemaphoreAttributes,
    [in]           LONG                  lInitialCount,
    [in]           LONG                  lMaximumCount,
    [in, optional] LPCSTR                lpName,
                   DWORD                 dwFlags,
    [in]           DWORD                 dwDesiredAccess
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-createsemaphoreexa
*/
extern "C" __declspec(dllimport) WHandle __stdcall CreateSemaphoreExA(WSecurityAttributes* security_attributes, s32 initial_count, s32 max_count, char* name, u32 flags, u32 desired_access);
#endif



// --- WaitForSingleObjectEx import
// Source: Kernel32
#if AZEWIN_COMPILE_KERNEL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    DWORD WaitForSingleObjectEx(
        [in] HANDLE hHandle,
        [in] DWORD  dwMilliseconds,
        [in] BOOL   bAlertable
    );
*/
extern "C" __declspec(dllimport) u32 __stdcall WaitForSingleObjectEx(WHandle handle, u32 milliseconds, b32 allertable);
#endif



// --- GetDC import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The GetDC function retrieves a handle to a device context (DC) for,
    the client area of a specified window or for the entire screen.
    You can use the returned handle in subsequent GDI functions to draw in the DC. 
    The device context is an opaque data structure, whose values are used internally by GDI.
    
    HDC GetDC(
        [in] HWND hWnd
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getdc
*/
extern "C" __declspec(dllimport) WDeviceHandle __stdcall GetDC(WWindowHandle handle);
#endif



// --- PeekMessageA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Dispatches incoming nonqueued messages, checks the thread message queue for a posted message, 
    and retrieves the message (if any exist).
    Unlike GetMessage, the PeekMessage function does not wait for a message to be posted before returning.
    
    BOOL PeekMessageA(
        [out]          LPMSG lpMsg,
        [in, optional] HWND  hWnd,
        [in]           UINT  wMsgFilterMin,
        [in]           UINT  wMsgFilterMax,
        [in]           UINT  wRemoveMsg
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea
*/
extern "C" __declspec(dllimport) b32 __stdcall PeekMessageA(WMessage* out_message, WWindowHandle window_handle, u32 filter_min, u32 filter_max, u32 remove_message);
#endif



// --- DispatchMessageA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Dispatches a message to a window procedure. 
    It is typically used to dispatch a message retrieved by the GetMessage function.
    
    LRESULT DispatchMessageA(
        [in] const MSG *lpMsg
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-dispatchmessagea
*/
extern "C" __declspec(dllimport) s64 __stdcall DispatchMessageA(WMessage* message);
#endif



// --- GetMessageA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    
    Retrieves a message from the calling thread's message queue. 
    The function dispatches incoming sent messages until a posted message is available for retrieval.
    
    BOOL GetMessageA(
        [out]          LPMSG lpMsg,
        [in, optional] HWND  hWnd,
        [in]           UINT  wMsgFilterMin,
        [in]           UINT  wMsgFilterMax
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmessagea
*/
extern "C" __declspec(dllimport) b32 __stdcall GetMessageA(WMessage* message, WWindowHandle window_handle, u32 filter_min, u32 filter_max);
#endif



// --- LoadCursorA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Loads the specified cursor resource from the executable (.EXE) file associated with an application instance.
    
    HCURSOR LoadCursorA(
        [in, optional] HINSTANCE hInstance,
        [in]           LPCSTR    lpCursorName
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-loadcursora
*/
extern "C" __declspec(dllimport) WCursorHandle __stdcall LoadCursorA(WInstanceHandle instance, char* cursor_name);
#endif



// --- DefWindowProcA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Calls the default window procedure to provide default processing for any window messages that an application does not process.
    This function ensures that every message is processed. 
    DefWindowProc is called with the same parameters received by the window procedure.
    
    LRESULT DefWindowProcA(
        [in] HWND   hWnd,
        [in] UINT   Msg,
        [in] WPARAM wParam,
        [in] LPARAM lParam
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-defwindowproca
*/
extern "C" __declspec(dllimport) s64 __stdcall DefWindowProcA(WWindowHandle handle, u32 message, u64 wparam, s64 lparam);
#endif



// --- RegisterClassA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Registers a window class for subsequent use in calls to the CreateWindow or CreateWindowEx function.
    
    ATOM RegisterClassA(
        [in] const WNDCLASSA *lpWndClass
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassa
*/
extern "C" __declspec(dllimport) s16 __stdcall RegisterClassA(WWindowClass* window_class);
#endif



// --- CreateWindowExA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Creates an overlapped, pop-up, or child window with an extended window style; otherwise, this function is identical to the CreateWindow function.
    For more information about creating a window and for full descriptions of the other parameters of CreateWindowEx, see CreateWindow.
    
    HWND CreateWindowExA(
        [in]           DWORD     dwExStyle,
        [in, optional] LPCSTR    lpClassName,
        [in, optional] LPCSTR    lpWindowName,
        [in]           DWORD     dwStyle,
        [in]           int       X,
        [in]           int       Y,
        [in]           int       nWidth,
        [in]           int       nHeight,
        [in, optional] HWND      hWndParent,
        [in, optional] HMENU     hMenu,
        [in, optional] HINSTANCE hInstance,
        [in, optional] LPVOID    lpParam
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa
*/
extern "C" __declspec(dllimport) WWindowHandle __stdcall CreateWindowExA(u32 ex_style, char* class_name, char* window_name, u32 style, s32 x, s32 y, s32 width, s32 height, WWindowHandle parent, WMenuHandle menu, WInstanceHandle instance, void* param);
#endif



// --- TranslateMessage import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Translates virtual-key messages into character messages.
    The character messages are posted to the calling thread's message queue,
    to be read the next time the thread calls the GetMessage or PeekMessage function.

    BOOL TranslateMessage(
        [in] const MSG *lpMsg
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-translatemessage
*/
extern "C" __declspec(dllimport) b32 __stdcall TranslateMessage(WMessage* message);
#endif



// --- PostThreadMessageA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*    
    Posts a message to the message queue of the specified thread. 
    It returns without waiting for the thread to process the message.
    
    BOOL PostThreadMessageA(
        [in] DWORD  idThread,
        [in] UINT   Msg,
        [in] WPARAM wParam,
        [in] LPARAM lParam
    );

    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-postthreadmessagea
*/
extern "C" __declspec(dllimport) b32 __stdcall PostThreadMessageA(u32 thread_id, u32 message, u64 wparam, s64 lparam);
#endif



// --- GetClientRect import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the coordinates of a window's client area.
    The client coordinates specify the upper-left and lower-right corners of the client area.
    Because client coordinates are relative to the upper-left corner of a window's client area, 
    the coordinates of the upper-left corner are (0,0).

    BOOL GetClientRect(
        [in]  HWND   hWnd,
        [out] LPRECT lpRect
    );

    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclientrect
*/
extern "C" __declspec(dllimport) b32 __stdcall GetClientRect(WWindowHandle window, WRect* out_rect);
#endif



// --- GetWindowRect import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the dimensions of the bounding rectangle of the specified window. 
    The dimensions are given in screen coordinates that are relative to the upper-left corner of the screen.
    
    BOOL GetWindowRect(
        [in]  HWND   hWnd,
        [out] LPRECT lpRect
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowrect
*/
extern "C" __declspec(dllimport) b32 __stdcall GetWindowRect(WWindowHandle window, WRect* out_rect);
#endif



// --- GetCursorPos import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the position of the mouse cursor, in screen coordinates.

    BOOL GetCursorPos(
        [out] LPPOINT lpPoint
    );

    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getcursorpos
*/
extern "C" __declspec(dllimport) b32 __stdcall GetCursorPos(WPoint* point);
#endif



// --- GetKeyState import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the status of the specified virtual key. 
    The status specifies whether the key is up, down, or toggled (on, off—alternating each time the key is pressed).
    
    SHORT GetKeyState(
        [in] int nVirtKey
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getkeystate
*/
extern "C" __declspec(dllimport) s16 __stdcall GetKeyState(s32 virtual_key);
#endif



// --- OpenClipboard import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Opens the clipboard for examination and prevents other applications from modifying the clipboard content.
    
    BOOL OpenClipboard(
        [in, optional] HWND hWndNewOwner
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-openclipboard
*/
extern "C" __declspec(dllimport) b32 __stdcall OpenClipboard(WWindowHandle window);
#endif



// -- CloseClipboard import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Closes the clipboard.

    BOOL CloseClipboard();
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-closeclipboard
*/
extern "C" __declspec(dllimport) b32 __stdcall CloseClipboard();
#endif



// --- EmptyClipboard import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Empties the clipboard and frees handles to data in the clipboard. 
    The function then assigns ownership of the clipboard to the window that currently has the clipboard open.

    BOOL EmptyClipboard();
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-emptyclipboard
*/
extern "C" __declspec(dllimport) b32 __stdcall EmptyClipboard();
#endif



// --- SetClipboardData import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Places data on the clipboard in a specified clipboard format. 
    The window must be the current clipboard owner, 
    and the application must have called the OpenClipboard function. 
    (When responding to the WM_RENDERFORMAT message,
    the clipboard owner must not call OpenClipboard before calling SetClipboardData.)

    HANDLE SetClipboardData(
        [in]           UINT   uFormat,
        [in, optional] HANDLE hMem
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setclipboarddata
*/
extern "C" __declspec(dllimport) WHandle __stdcall SetClipboardData(u32 format, WHandle mem);
#endif



// --- GetClipboardData import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves data from the clipboard in a specified format. 
    The clipboard must have been opened previously.
    
    HANDLE GetClipboardData(
        [in] UINT uFormat
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getclipboarddata
*/
extern "C" __declspec(dllimport) WHandle __stdcall GetClipboardData(u32 format);
#endif



// --- SetWindowPos import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Changes the size, position, and Z order of a child, pop-up, or top-level window. 
    These windows are ordered according to their appearance on the screen. 
    The topmost window receives the highest rank and is the first window in the Z order.
    
    BOOL SetWindowPos(
        [in]           HWND hWnd,
        [in, optional] HWND hWndInsertAfter,
        [in]           int  X,
        [in]           int  Y,
        [in]           int  cx,
        [in]           int  cy,
        [in]           UINT uFlags
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowpos
*/
extern "C" __declspec(dllimport) b32 __stdcall SetWindowPos(WWindowHandle window, WWindowHandle inser_after, s32 x, s32 y, s32 cx, s32 cy, u32 flags);
#endif



// --- SetWindowPlacement import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Sets the show state and the restored, minimized, and maximized positions of the specified window.

    BOOL SetWindowPlacement(
        [in] HWND                  hWnd,
        [in] const WINDOWPLACEMENT *lpwndpl
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowplacement
*/
extern "C" __declspec(dllimport) b32 __stdcall SetWindowPlacement(WWindowHandle window, WWindowPlacement* window_placement);
#endif



// --- SetWindowLongA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The GetMonitorInfo function retrieves information about a display monitor.
    
    LONG SetWindowLongA(
        [in] HWND hWnd,
        [in] int  nIndex,
        [in] LONG dwNewLong
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getmonitorinfoa
*/
extern "C" __declspec(dllimport) s32 __stdcall SetWindowLongA(WWindowHandle window, s32 index, s32 new_long);
#endif



// --- GetWindowLongA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    LONG GetWindowLongA(
        [in] HWND hWnd,
        [in] int  nIndex
    );
*/
extern "C" __declspec(dllimport) s32 __stdcall GetWindowLongA(WWindowHandle window, s32 index);
#endif



// --- GetMonitorInfoA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Changes an attribute of the specified window. 
    The function also sets the 32-bit (long) value at the specified offset into the extra window memory.
    
    BOOL GetMonitorInfoA(
        [in]  HMONITOR      hMonitor,
        [out] LPMONITORINFO lpmi
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-setwindowlonga
*/
extern "C" __declspec(dllimport) b32 __stdcall GetMonitorInfoA(WMonitorHandle monitor, WMonitorInfo* out_monitor_info);
#endif



// --- MonitorFromWindow import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The MonitorFromWindow function retrieves a handle to the display monitor that has the largest area of intersection with the bounding rectangle of a specified window.
    
    HMONITOR MonitorFromWindow(
        [in] HWND  hwnd,
        [in] DWORD dwFlags
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-monitorfromwindow
*/
extern "C" __declspec(dllimport) WMonitorHandle __stdcall MonitorFromWindow(WWindowHandle window, u32 flags);
#endif



// --- GetWindowPlacement import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    Retrieves the show state and the restored, minimized, and maximized positions of the specified window.

    BOOL GetWindowPlacement(
        [in]      HWND            hWnd,
        [in, out] WINDOWPLACEMENT *lpwndpl
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getwindowplacement
*/
extern "C" __declspec(dllimport) b32 __stdcall GetWindowPlacement(WWindowHandle window, WWindowPlacement* out_window_placement);
#endif



// --- LoadIconA import
// Source: User32
#if AZEWIN_COMPILE_USER32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    HICON LoadIconA(
      [in, optional] HINSTANCE hInstance,
      [in]           LPCSTR    lpIconName
    );
*/
extern "C" __declspec(dllimport) WIconHandle __stdcall LoadIconA(WInstanceHandle instance, char* icon_name);
#endif



// --- ChoosePixelFormat  import
// Source: Gdi32
#if AZEWIN_COMPILE_GDI32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The ChoosePixelFormat function attempts to match an appropriate pixel format supported by a device context to a given pixel format specification.

    int ChoosePixelFormat(
        HDC                         hdc,
        const PIXELFORMATDESCRIPTOR *ppfd
    );

    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-choosepixelformat
*/
extern "C" __declspec(dllimport) s32 __stdcall ChoosePixelFormat(WDeviceHandle device, WPixel_Format_Descriptor* format);
#endif



// --- DescribePixelFormat import
// Source: Gdi32
#if AZEWIN_COMPILE_GDI32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The DescribePixelFormat function obtains information about the pixel format identified by iPixelFormat of the device associated with hdc.
    The function sets the members of the PIXELFORMATDESCRIPTOR structure pointed to by ppfd with that pixel format data.

    int DescribePixelFormat(
        HDC                     hdc,
        int                     iPixelFormat,
        UINT                    nBytes,
        LPPIXELFORMATDESCRIPTOR ppfd
    );

    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-describepixelformat
*/
extern "C" __declspec(dllimport) s32 __stdcall DescribePixelFormat(WDeviceHandle device, s32 pixel_format, u32 bytes, WPixel_Format_Descriptor* out_pixel_format_description);
#endif



// --- SetPixelFormat import
// Source: Gdi32
#if AZEWIN_COMPILE_GDI32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The SetPixelFormat function sets the pixel format of the specified device context to the format specified by the iPixelFormat index.
    
    BOOL SetPixelFormat(
        HDC                         hdc,
        int                         format,
        const PIXELFORMATDESCRIPTOR *ppfd
    );
    
    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-setpixelformat
*/
extern "C" __declspec(dllimport) b32 __stdcall SetPixelFormat(WDeviceHandle device, s32 format_index, WPixel_Format_Descriptor* format);
#endif



// --- SwapBuffers import
// Source: Gdi32
#if AZEWIN_COMPILE_GDI32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The SwapBuffers function exchanges the front and back buffers if the current pixel format for the window referenced by the specified device context includes a back buffer.

    BOOL SwapBuffers(
        HDC unnamedParam1
    );

    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-swapbuffers
*/
extern "C" __declspec(dllimport) b32 __stdcall SwapBuffers(WDeviceHandle device);
#endif



// --- StretchDIBits import
// Source: Gdi32
#if AZEWIN_COMPILE_GDI32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The StretchDIBits function copies the color data for a rectangle of pixels in a DIB, JPEG, or PNG image to the specified destination rectangle. 
    If the destination rectangle is larger than the source rectangle, 
    this function stretches the rows and columns of color data to fit the destination rectangle. 
    If the destination rectangle is smaller than the source rectangle, 
    this function compresses the rows and columns by using the specified raster operation.

    int StretchDIBits(
        [in] HDC              hdc,
        [in] int              xDest,
        [in] int              yDest,
        [in] int              DestWidth,
        [in] int              DestHeight,
        [in] int              xSrc,
        [in] int              ySrc,
        [in] int              SrcWidth,
        [in] int              SrcHeight,
        [in] const VOID       *lpBits,
        [in] const BITMAPINFO *lpbmi,
        [in] UINT             iUsage,
        [in] DWORD            rop
    );
    
    // https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-stretchdibits
*/
extern "C" __declspec(dllimport) s32 __stdcall StretchDIBits(WDeviceHandle device, s32 dest_x, s32 dest_y, s32 dest_width, s32 dest_height, s32 src_x, s32 src_y, s32 src_width, s32 src_height, void* bits, WBitmapInfo* bitmap_info, u32 usage, u32 raster_op);
#endif



// --- wglCreateContext import
// Source: Opengl32
#if AZEWIN_COMPILE_OPENGL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The wglCreateContext function creates a new OpenGL rendering context, 
    which is suitable for drawing on the device referenced by hdc.
    The rendering context has the same pixel format as the device context.
    
    HGLRC wglCreateContext(
        HDC unnamedParam1
    );

    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wglcreatecontext
*/
extern "C" __declspec(dllimport) WGLRenderContextHandle __stdcall wglCreateContext(WDeviceHandle device);
#endif



// --- wglDeleteContext import
// Source: Opengl32
#if AZEWIN_DEFAULT_COMPILE || AZEWIN_COMPILE_OPENGL32 || 0
/*
    The wglDeleteContext function deletes a specified OpenGL rendering context.
    
    BOOL wglDeleteContext(
        HGLRC unnamedParam1
    );

    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wgldeletecontext
*/
extern "C" __declspec(dllimport) b32 __stdcall wglDeleteContext(WGLRenderContextHandle render_context);
#endif



// --- wglMakeCurrent import
// Source: Opengl32
#if AZEWIN_COMPILE_OPENGL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    The wglMakeCurrent function makes a specified OpenGL rendering context the calling thread's current rendering context. 
    All subsequent OpenGL calls made by the thread are drawn on the device identified by hdc. 
    You can also use wglMakeCurrent to change the calling thread's current rendering context so it's no longer current.

    BOOL wglMakeCurrent(
        HDC   unnamedParam1,
        HGLRC unnamedParam2
    );

    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wglmakecurrent
*/
extern "C" __declspec(dllimport) b32 __stdcall wglMakeCurrent(WDeviceHandle device, WGLRenderContextHandle render_context);
#endif



// --- wglGetProcAddress import
// Source: Opengl32
#if AZEWIN_DEFAULT_COMPILE || AZEWIN_COMPILE_OPENGL32 || 0
/*    
    The wglGetProcAddress function returns the address of an OpenGL extension function for use with the current OpenGL rendering context.
    
    PROC wglGetProcAddress(
        LPCSTR unnamedParam1
    );

    https://learn.microsoft.com/en-us/windows/win32/api/wingdi/nf-wingdi-wglgetprocaddress
*/
extern "C" __declspec(dllimport) WProcedure __stdcall wglGetProcAddress(char* procedure_name);
#endif



// --- wglCreateContextAttribsARB signature
// Source: UNKNOWN
#if AZEWIN_COMPILE_OPENGL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    https://registry.khronos.org/OpenGL/extensions/ARB/WGL_ARB_create_context.txt
*/
typedef WGLRenderContextHandle __stdcall wglCreateContextAttribsARB(WDeviceHandle device, WGLRenderContextHandle shared_context, s32* attribute_list);
#endif



// --- wglSwapIntervalEXT signature
// Source: UNKNOWN
#if AZEWIN_COMPILE_OPENGL32 || AZEWIN_DEFAULT_COMPILE || 0
/*
    https://registry.khronos.org/OpenGL/extensions/EXT/WGL_EXT_swap_control.txt
*/
typedef bool __stdcall wglSwapIntervalEXT(s32 interval);
#endif




#if AZEWIN_DEFAULT_COMPILE || 0
    /*
        XInput related functionality, if you don't care about Xinput set the #if to 0.
        This is not intented to be a full implementation of the interface, but just a 
        minimal set of the commonly used items.
    */
    
    constexpr char* WModuleMamesXInput[] = 
    {
        "xinput1_4.dll",
        "xinput1_3.dll",
        "xinput9_1_0.dll"
    };
    
    /* Load module example:
    
        WModuleHandle xinput = {};
        for(u64 i = 0; i < Array_Lenght(WModuleMamesXInput[i]) && !xinput; ++i)
        {
            xinput = LoadLibraryA(WModuleMamesXInput[i]);
        }
    */


    // https://learn.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
    struct XInputGamepad
    {
        u16 buttons;
        u8 left_trigger;
        u8 right_trigger;
        s16 thumb_left_x;
        s16 thumb_left_y;
        s16 thumb_right_x;
        s16 thumb_right_y;
    };



    // https://learn.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_state
    struct XInputState
    {
        s32 packet_number;
        XInputGamepad gamepad;
    };
    static_assert(sizeof(XInputState) == 16);
    

    // --- XInputGetState signature
    // Source: Xinput1_4.dll, Xinput9_1_0.dll, Xinputuap.dll
    #if AZEWIN_DEFAULT_COMPILE || 0
    /*
        "Retrieves the current state of the specified controller."
        
        DWORD XInputGetState(
            [in]  DWORD        dwUserIndex,
            [out] XINPUT_STATE *pState
        );
        
        https://learn.microsoft.com/en-us/windows/win32/api/xinput/nf-xinput-xinputgetstate
    */
    constexpr char* XInputGetStateProcName = "XInputGetState";
    typedef u32 __stdcall XInputGetState(u32 user_idx, XInputState* out_state);
    #endif

#endif



#if AZEWIN_DEFAULT_COMPILE || 0
    constexpr u64 DIRECTSOUND_VERSION = 0x0900;
    
    
    enum WDSSCL
    {
        DSSCL_NORMAL                = 0x00000001,
        DSSCL_PRIORITY              = 0x00000002,
        DSSCL_EXCLUSIVE             = 0x00000003,
        DSSCL_WRITEPRIMARY          = 0x00000004        
    };
    
    
    enum WDSBCAPS
    {
        DSBCAPS_PRIMARYBUFFER       = 0x00000001,
        DSBCAPS_STATIC              = 0x00000002,
        DSBCAPS_LOCHARDWARE         = 0x00000004,
        DSBCAPS_LOCSOFTWARE         = 0x00000008,
        DSBCAPS_CTRL3D              = 0x00000010,
        DSBCAPS_CTRLFREQUENCY       = 0x00000020,
        DSBCAPS_CTRLPAN             = 0x00000040,
        DSBCAPS_CTRLVOLUME          = 0x00000080,
        DSBCAPS_CTRLPOSITIONNOTIFY  = 0x00000100,
        DSBCAPS_CTRLFX              = 0x00000200,
        DSBCAPS_STICKYFOCUS         = 0x00004000,
        DSBCAPS_GLOBALFOCUS         = 0x00008000,
        DSBCAPS_GETCURRENTPOSITION2 = 0x00010000,
        DSBCAPS_MUTE3DATMAXDISTANCE = 0x00020000,
        DSBCAPS_LOCDEFER            = 0x00040000,
        DSBCAPS_TRUEPLAYPOSITION    = 0x00080000        
    };

    
    
    enum WDSBPLAY
    {
        DSBPLAY_LOOPING                 = 0x00000001,
        DSBPLAY_LOCHARDWARE             = 0x00000002,
        DSBPLAY_LOCSOFTWARE             = 0x00000004,
        DSBPLAY_TERMINATEBY_TIME        = 0x00000008,
        DSBPLAY_TERMINATEBY_DISTANCE    = 0x000000010,
        DSBPLAY_TERMINATEBY_PRIORITY    = 0x000000020        
    };

    
    struct WDirectSoundBuffer;
    struct WCOMObjectDirectSound;
    struct WCOMObjectDirectSoundBuffer;
    
    
    // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416818(v=vs.85)
    struct WDirectSoundCapabilities
    {
        u32 size;
        u32 flags;
        u32 buffer_bytes;
        u32 unlock_transfer_rate;
        u32 play_cpu_overhead;
    };
    
    
    
    // https://learn.microsoft.com/en-us/windows/win32/api/guiddef/ns-guiddef-guid
    struct WGUID
    {
        u32 data1;
        u16 data2;
        u16 data3;
        unsigned char data4[8];
    };
    static_assert(sizeof(WGUID) == 128 / 8);
    
    
    
    // https://learn.microsoft.com/en-us/windows/win32/api/mmeapi/ns-mmeapi-waveformatex
    struct WWaveFormatEx
    {
        u16 format;
        u16 channels;
        u32 samples_per_second;
        u32 avarage_bytes_per_second;
        u16 block_alling;
        u16 bits_per_sample;
        u16 size;
    };
    
    
    
    struct WDirectSoundBufferDescription
    {
        u32 size;
        u32 flags;
        u32 buffer_bytes;
        u32 reserved;
        WWaveFormatEx* fx_format;
        WGUID guid_3D_algorithm;
    };
    
    
    
    // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ee416821(v=vs.85)
    struct DirectSoundCapabilities
    {
        u32 size;
        u32 flags;
        u32 min_secondary_sample_rate;
        u32 max_secondary_sample_rate;
        u32 primary_buffers;
        u32 max_hw_mixing_all_buffers;
        u32 max_hw_mixing_static_buffers;
        u32 max_hw_mixing_streaming_buffers;
        u32 free_hw_mixing_all_buffers;
        u32 free_hw_mixing_static_buffers;
        u32 free_hw_mixing_streaming_buffers;
        u32 max_hw_3d_all_buffers;
        u32 max_hw_3d_static_buffers;
        u32 max_hw_3d_streaming_buffers;
        u32 free_hw_3d_all_buffers;
        u32 free_hw_3d_static_buffers;
        u32 free_hw_3d_streaming_buffers;
        u32 total_hw_mem_bytes;
        u32 free_hw_mem_bytes;
        u32 max_contig_free_hw_mem_bytes;
        u32 unlock_transferrate_hw_buffers;
        u32 play_cpu_overhead_sw_buffers;
        u32 reserved1;
        u32 reserved2;
    };
    
    
    // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/mt708922(v=vs.85)
    struct WDirectSound
    {
        // IUnknown methods
        s32 (__stdcall *QueryInterface)(WCOMObjectDirectSound* com, WGUID* guid, void**);
        
        u32 (__stdcall *AddRef)(WCOMObjectDirectSound* com);
        
        u32 (__stdcall *Release)(WCOMObjectDirectSound* com);


        // IDirectSound methods
        s32 (__stdcall *CreateSoundBuffer)(WCOMObjectDirectSound* com, WDirectSoundBufferDescription* buffer_description, WCOMObjectDirectSoundBuffer** buffer, void*);
        
        s32 (__stdcall *GetCaps)(WCOMObjectDirectSound* com, DirectSoundCapabilities* direct_sound_capabilities);
        
        s32 (__stdcall *DuplicateSoundBuffer)(WCOMObjectDirectSound* com, WDirectSoundBuffer* buffer_original, WDirectSoundBuffer** buffer_dublicate);
        
        s32 (__stdcall *SetCooperativeLevel)(WCOMObjectDirectSound* com, WWindowHandle window, u32 lvl);
        
        s32 (__stdcall *Compact)(WCOMObjectDirectSound* com);
        
        s32 (__stdcall *GetSpeakerConfig)(WCOMObjectDirectSound* com, u32* speaker_config);
        
        s32 (__stdcall *SetSpeakerConfig)(WCOMObjectDirectSound* com, u32 speaker_config);
        
        s32 (__stdcall *Initialize)(WCOMObjectDirectSound* com, WGUID* guid_device);
    };
    
    
    struct WCOMObjectDirectSound
    {
        WDirectSound* dsound;
    };
    
    
    // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/mt708923(v=vs.85)
    struct WDirectSoundBuffer
    {
        // IUnknown methods
        s32 (__stdcall *QueryInterface)(WGUID* guid, void**);
        
        u32 (__stdcall *AddRef)();
        
        u32 (__stdcall *Release)();


        // IDirectSoundBuffer methods
        s32 (__stdcall *GetCaps)(WCOMObjectDirectSoundBuffer* dsp, WDirectSoundCapabilities* buffer_capabilities);
        
        s32 (__stdcall *GetCurrentPosition)(WCOMObjectDirectSoundBuffer* dsp, u32* current_play_cursor, u32* current_write_cursor);
        
        s32 (__stdcall *GetFormat)(WCOMObjectDirectSoundBuffer* dsp, WWaveFormatEx* fx_format, u32 size_allocated, u32* size_written);
        
        s32 (__stdcall *GetVolume)(WCOMObjectDirectSoundBuffer* dsp, s32* out_volume);
        
        s32 (__stdcall *GetPan)(WCOMObjectDirectSoundBuffer* dsp, s32* out_pan);
        
        s32 (__stdcall *GetFrequency)(WCOMObjectDirectSoundBuffer* dsp, u32* out_frequency);
        
        s32 (__stdcall *GetStatus)(WCOMObjectDirectSoundBuffer* dsp, u32* out_status);
        
        s32 (__stdcall *Initialize)(WCOMObjectDirectSoundBuffer* dsp, WDirectSound* direct_sound, WDirectSoundBufferDescription* buffer_description);
        
        s32 (__stdcall *Lock)(WCOMObjectDirectSoundBuffer* dsp, u32 offset, u32 bytes, void** audio_ptr1, u32* audio_bytes1, void** audio_ptr2, u32* audio_bytes2, u32 flags);
        
        s32 (__stdcall *Play)(WCOMObjectDirectSoundBuffer* dsp, u32 reserved, u32 priority, u32 flags);
        
        s32 (__stdcall *SetCurrentPosition)(WCOMObjectDirectSoundBuffer* dsp, u32 new_position);
        
        s32 (__stdcall *SetFormat)(WCOMObjectDirectSoundBuffer* dsp, WWaveFormatEx* format);
        
        s32 (__stdcall *SetVolume)(WCOMObjectDirectSoundBuffer* dsp, s32 volume);
        
        s32 (__stdcall *SetPan)(WCOMObjectDirectSoundBuffer* dsp, s32 pan);
        
        s32 (__stdcall *SetFrequency)(WCOMObjectDirectSoundBuffer* dsp, u32 frequency);
        
        s32 (__stdcall *Stop)(WCOMObjectDirectSoundBuffer* dsp);
        
        s32 (__stdcall *Unlock)(WCOMObjectDirectSoundBuffer* dsp, void* audio_ptr1, u32 audio_bytes1, void* audio_ptr2, u32 audio_bytes2);
        
        s32 (__stdcall *Restore)(WCOMObjectDirectSoundBuffer* dsp);
    };
    
    
    struct WCOMObjectDirectSoundBuffer
    {
        WDirectSoundBuffer* dsound_buffer;
    };
    
    
    constexpr char* DirectSoundModuleName = "dsound.dll";
    constexpr char* DirectSoundCreateProcName = "DirectSoundCreate";
    
    // https://learn.microsoft.com/en-us/previous-versions/windows/desktop/mt708921(v=vs.85)
    typedef s32 __stdcall DirectSoundCreate(WGUID* guid_device, WCOMObjectDirectSound** out_direct_sound, void*);
    
    
    // https://learn.microsoft.com/en-us/windows/win32/api/winerror/nf-winerror-succeeded
    static inline bool SUCCEEDED(s32 hr) { return (((s32)(hr)) >= 0); }
#endif



#undef AZEWIN_DEFAULT_COMPILE
#undef AZEWIN_COMPILE_KERNEL32
#undef AZEWIN_COMPILE_USER32
#undef AZEWIN_COMPILE_GDI32
#undef AZEWIN_COMPILE_OPENGL32


// Error codes. Not a complite list of course and you don't want that.
// The winerror.h file that contains these definations; is over 65k lines long...
// https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes--0-499-
enum WError : u32
{
    ERROR_SUCCESS                   = 0,
    ERROR_INSUFICENT_BUFFER         = 122,
    ERROR_DEVICE_NOT_CONNECTED      = 1167 
};


enum WPeekMessageRemove: u32
{
    PM_NOREMOVE                     = 0x0000,  // Messages are not removed from the queue after processing by PeekMessage.
    PM_REMOVE                       = 0x0001,  // Messages are removed from the queue after processing by PeekMessage.
    PM_NOYIELD                      = 0x0002   // Prevents the system from releasing any thread that is waiting for the caller to go idle. Combine with 'yes' or 'no'.
};


enum WWindowMessage : s64
{
    WM_NULL                         = 0x0000, 
    WM_CREATE                       = 0x0001, 
    WM_DESTROY                      = 0x0002,
    WM_MOVE                         = 0x0003, 
    WM_SIZE                         = 0x0005, 
    WM_SET_FOCUS                    = 0x0007,
    WM_KILL_FOCUS                   = 0x0008,
    WM_ENABLE                       = 0x000A,
    WM_SET_REDRAW                   = 0x000B,
    WM_SET_TEXT                     = 0x000C,
    WM_GET_TEXT                     = 0x000D,
    WM_GET_TEXT_LENGHT              = 0x000E,
    WM_PAINT                        = 0x000F,
    WM_CLOSE                        = 0x0010,
    WM_KEY_DOWN                     = 0x0100,
    WM_KEY_UP                       = 0x0101,
    WM_CHAR                         = 0x0102,
    WM_ACTIVATEAPP                  = 0x001C,

    WM_MOUSEWHEEL                   = 0x020A,
    WM_MOUSE_MOVE                   = 0x0200,
    WM_MOUSE_LEFT_BUTTON_DOWN       = 0x0201,
    WM_MOUSE_LEFT_BUTTON_UP         = 0x0202,
    WM_MOUSE_RIGHT_BUTTON_DOWN      = 0x0204,
    WM_MOUSE_RIGHT_BUTTON_UP        = 0x0205,
    WM_MOUSE_MIDDLE_BUTTON_DOWN     = 0x0207,
    WM_MOUSE_MIDDLE_BUTTON          = 0x0208,
};  


// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
enum WWindowStyle : u32
{
    WS_BORDER                       = 0x00800000L,
    WS_CAPTION                      = 0x00C00000L,	
    WS_CHILD                        = 0x40000000L,
    WS_CHILDWINDOW                  = 0x40000000L,
    WS_CLIPCHILDREN                 = 0x02000000L,
    WS_CLIPSIBLINGS                 = 0x04000000L,
    WS_DISABLED                     = 0x08000000L,
    WS_DLGFRAME	                    = 0x00400000L,
    WS_GROUP                        = 0x00020000L,
    WS_HSCROLL                      = 0x00100000L,
    WS_ICONIC                       = 0x20000000L,
    WS_MAXIMIZE                     = 0x01000000L,
    WS_MAXIMIZEBOX                  = 0x00010000L,
    WS_MINIMIZE                     = 0x20000000L,
    WS_MINIMIZEBOX                  = 0x00020000L,
    WS_OVERLAPPED                   = 0x00000000L,
    WS_POPUP                        = 0x80000000L,
    WS_SIZEBOX                      = 0x00040000L,
    WS_SYSMENU                      = 0x00080000L,
    WS_TABSTOP                      = 0x00010000L,
    WS_THICKFRAME                   = 0x00040000L,
    WS_TILED                        = 0x00000000L,
    WS_VISIBLE                      = 0x10000000L,
    WS_VSCROLL                      = 0x00200000L
};

constexpr u32 WS_OVERLAPPEDWINDOW = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
constexpr u32 WS_POPUPWINDOW = (WS_POPUP | WS_BORDER | WS_SYSMENU);
constexpr u32 WS_TILEDWINDOW = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
constexpr u32 CW_DEFAULT = ((s32)0x80000000);


// https://learn.microsoft.com/en-us/windows/win32/winmsg/window-class-styles
enum WClassStyle : u32
{
    CS_BYTEALIGNCLIENT              = 0x1000,
    CS_BYTEALIGNWINDOW              = 0x2000,
    CS_CLASSDC                      = 0x0040,
    CS_DBLCLKS                      = 0x0008,
    CS_DROPSHADOW                   = 0x00020000,
    CS_GLOBALCLASS                  = 0x4000,
    CS_HREDRAW                      = 0x0002,
    CS_NOCLOSE                      = 0x0200,
    CS_OWNDC                        = 0x0020,
    CS_PARENTDC                     = 0x0080,
    CS_SAVEBITS                     = 0x0800,
    CS_VREDRAW                      = 0x0001
};


enum WProcessorArchitecture : u16
{
    WPA_INTEL                       = 0,
    WPA_ARM                         = 5,
    WPA_IA64                        = 6,
    WPA_AMD64                       = 9,
    WPA_ARM64                       = 12,
    WPA_UNKNOWN                     = 0xffff
};


// https://learn.microsoft.com/en-us/windows/win32/menurc/about-cursors
enum WIDC : u16
{
    IDC_ARROW                       = 32512,
    IDC_BEAM                        = 32513,
    IDC_WAIT                        = 32514,
    IDC_CROSS                       = 32515,
    IDC_UPARROW                     = 32516,
    IDC_SIZENWSE                    = 32642,
    IDC_SIZENESW                    = 32643,
    IDC_SIZEWE                      = 32644,
    IDC_SIZENS                      = 32645,
    IDC_SIZEALL                     = 32646,
    IDC_NO                          = 32648,
    IDC_HAND                        = 32649,
    IDC_APPSTARTING                 = 32650,
    IDC_HELP                        = 32651,
    IDC_PIN                         = 32671,
    IDC_PERSON                      = 32672,
    IDC_PEN                         = 32631,
    IDC_CD                          = 32663
};


constexpr u32 PFD_TYPE_RGBA        = 0;
constexpr u32 PFD_TYPE_COLORINDEX  = 1;
constexpr u32 PFD_MAIN_PLANE       = 0;
constexpr u32 PFD_OVERLAY_PLANE    = 1;
constexpr u32 PFD_UNDERLAY_PLANE   = -1;


enum WGL_Context : s32
{
    WGL_CONTEXT_MAJOR_VERSION_ARB             = 0x2091,
    WGL_CONTEXT_MINOR_VERSION_ARB             = 0x2092,
    WGL_CONTEXT_LAYER_PLANE_ARB               = 0x2093,
    WGL_CONTEXT_FLAGS_ARB                     = 0x2094,
    WGL_CONTEXT_PROFILE_MASK_ARB              = 0x9126,
    WGL_CONTEXT_DEBUG_BIT_ARB                 = 0x0001,
    WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB    = 0x0002,
    WGL_CONTEXT_CORE_PROFILE_BIT_ARB          = 0x00000001,
    WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB = 0x00000002
};



enum WMem
{
    MEM_COMMIT              = 0x00001000,
    MEM_RESERVE             = 0x00002000,
    MEM_RESET               = 0x00080000,
    MEM_RESET_UNDO          = 0x1000000,
    MEM_DECOMMIT            = 0x00004000,
    MEM_RELEASE             = 0x00008000
};


// https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
enum WPage
{
    PAGE_EXECUTE            = 0x10,
    PAGE_EXECUTE_READ       = 0x20,
    PAGE_EXECUTE_READWRITE  = 0x40,
    PAGE_EXECUTE_WRITECOPY  = 0x80,
    PAGE_NOACCESS           = 0x01,
    PAGE_READONLY           = 0x02,
    PAGE_READWRITE          = 0x04,
    PAGE_WRITECOPY          = 0x08,
    PAGE_TARGETS_INVALID    = 0x40000000,
    PAGE_TARGETS_NO_UPDATE  = 0x40000000
};


// https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
enum WVK
{
    VK_LBUTTON              = 0x01,
    VK_RBUTTON              = 0x02,
    VK_CANCEL               = 0x03,
    VK_MBUTTON              = 0x04,
    VK_XBUTTON1             = 0x05,
    VK_XBUTTON2             = 0x06,
    VK_BACK                 = 0x08,
    VK_TAB                  = 0x09,
    VK_CLEAR                = 0x0C,
    VK_RETURN               = 0x0D,
    VK_SHIFT                = 0x10,
    VK_CONTROL              = 0x11,
    VK_MENU                 = 0x12,
    VK_PAUSE                = 0x13,
    VK_CAPITAL              = 0x14,
    VK_KANA                 = 0x15,
    VK_HANGUL               = 0x15,
    VK_IME_ON               = 0x16,
    VK_JUNJA                = 0x17,
    VK_FINAL                = 0x18,
    VK_HANJA                = 0x19,
    VK_KANJI                = 0x19,
    VK_IME_OFF              = 0x1A,
    VK_ESCAPE               = 0x1B,
    VK_CONVERT              = 0x1C,
    VK_NONCONVERT           = 0x1D,
    VK_ACCEPT               = 0x1E,
    VK_MODECHANGE           = 0x1F,
    VK_SPACE                = 0x20,
    VK_PRIOR                = 0x21,
    VK_NEXT                 = 0x22,
    VK_END                  = 0x23,
    VK_HOME                 = 0x24,
    VK_LEFT                 = 0x25,
    VK_UP                   = 0x26,
    VK_RIGHT                = 0x27,
    VK_DOWN                 = 0x28,
    VK_SELECT               = 0x29,
    VK_PRINT                = 0x2A,
    VK_EXECUTE              = 0x2B,
    VK_SNAPSHOT             = 0x2C,
    VK_INSERT               = 0x2D,
    VK_DELETE               = 0x2E,
    VK_HELP                 = 0x2F,
    VK_LWIN                 = 0x5B,
    VK_RWIN                 = 0x5C,
    VK_APPS                 = 0x5D,
    VK_SLEEP                = 0x5F,
    VK_NUMPAD0              = 0x60,
    VK_NUMPAD1              = 0x61,
    VK_NUMPAD2              = 0x62,
    VK_NUMPAD3              = 0x63,
    VK_NUMPAD4              = 0x64,
    VK_NUMPAD5              = 0x65,
    VK_NUMPAD6              = 0x66,
    VK_NUMPAD7              = 0x67,
    VK_NUMPAD8              = 0x68,
    VK_NUMPAD9              = 0x69,
    VK_MULTIPLY             = 0x6A,
    VK_ADD                  = 0x6B,
    VK_SEPARATOR            = 0x6C,
    VK_SUBTRACT             = 0x6D,
    VK_DECIMAL              = 0x6E,
    VK_DIVIDE               = 0x6F,
    VK_F1                   = 0x70,
    VK_F2                   = 0x71,
    VK_F3                   = 0x72,
    VK_F4                   = 0x73,
    VK_F5                   = 0x74,
    VK_F6                   = 0x75,
    VK_F7                   = 0x76,
    VK_F8                   = 0x77,
    VK_F9                   = 0x78,
    VK_F10                  = 0x79,
    VK_F11                  = 0x7A,
    VK_F12                  = 0x7B,
    VK_F13                  = 0x7C,
    VK_F14                  = 0x7D,
    VK_F15                  = 0x7E,
    VK_F16                  = 0x7F,
    VK_F17                  = 0x80,
    VK_F18                  = 0x81,
    VK_F19                  = 0x82,
    VK_F20                  = 0x83,
    VK_F21                  = 0x84,
    VK_F22                  = 0x85,
    VK_F23                  = 0x86,
    VK_F24                  = 0x87,
    VK_NUMLOCK              = 0x90,
    VK_SCROLL               = 0x91,
    VK_LSHIFT               = 0xA0,
    VK_RSHIFT               = 0xA1,
    VK_LCONTROL             = 0xA2,
    VK_RCONTROL             = 0xA3,
    VK_LMENU                = 0xA4,
    VK_RMENU                = 0xA5,
    VK_BROWSER_HOME         = 0xAC,
    VK_VOLUME_MUTE          = 0xAD,
    VK_VOLUME_DOWN          = 0xAE,
    VK_VOLUME_UP            = 0xAF,
    VK_MEDIA_STOP           = 0xB2,
    VK_LAUNCH_MAIL          = 0xB4,
    VK_LAUNCH_APP1          = 0xB6,
    VK_LAUNCH_APP2          = 0xB7,
    VK_OEM_1                = 0xBA,
    VK_OEM_PLUS             = 0xBB,
    VK_OEM_COMMA            = 0xBC,
    VK_OEM_MINUS            = 0xBD,
    VK_OEM_PERIOD           = 0xBE,
    VK_OEM_2                = 0xBF,
    VK_OEM_3                = 0xC0,
    VK_OEM_4                = 0xDB,
    VK_OEM_5                = 0xDC,
    VK_OEM_6                = 0xDD,
    VK_OEM_7                = 0xDE,
    VK_OEM_8                = 0xDF,
    VK_OEM_102              = 0xE2,
    VK_PROCESSKEY           = 0xE5,
    VK_PACKET               = 0xE7,
    VK_ATTN                 = 0xF6,
    VK_CRSEL                = 0xF7,
    VK_EXSEL                = 0xF8,
    VK_EREOF                = 0xF9,
    VK_PLAY                 = 0xFA,
    VK_ZOOM                 = 0xFB,
    VK_NONAME               = 0xFC,
    VK_PA1                  = 0xFD,
    VK_OEM_CLEAR            = 0xFE,
    VK_MEDIA_PLAY_PAUSE     = 0xB3,
    VK_LAUNCH_MEDIA_SELECT  = 0xB5,
    VK_MEDIA_NEXT_TRACK     = 0xB0,
    VK_MEDIA_PREV_TRACK     = 0xB1,
    VK_BROWSER_BACK         = 0xA6,
    VK_BROWSER_FORWARD      = 0xA7,
    VK_BROWSER_REFRESH      = 0xA8,
    VK_BROWSER_STOP         = 0xA9,
    VK_BROWSER_SEARCH       = 0xAA,
    VK_BROWSER_FAVORITES    = 0xAB    
};


enum WGMEM
{
    GMEM_FIXED      = 0x0000,
    GMEM_MOVEABLE   = 0x0002,
    GMEM_ZEROINIT   = 0x0040,
    GHND            = 0x0042,
    GPTR            = 0x0040    
};
 
 
// https://learn.microsoft.com/en-us/windows/win32/dataxchg/standard-clipboard-formats
enum WCF
{
    CF_TEXT             = 1,
    CF_BITMAP           = 2,
    CF_DIB              = 8,
    CF_DIBV5            = 17,
    CF_DIF              = 5,
    CF_DSPBITMAP        = 0x0082,
    CF_DSPENHMETAFILE   = 0x008E,
    CF_DSPMETAFILEPICT  = 0x0083,
    CF_DSPTEXT          = 0x0081,
    CF_ENHMETAFILE      = 14,
    CF_GDIOBJFIRST      = 0x0300,
    CF_GDIOBJLAST       = 0x03FF,
    CF_HDROP            = 15,
    CF_LOCALE           = 16,
    CF_METAFILEPICT     = 3,
    CF_OEMTEXT          = 7,
    CF_OWNERDISPLAY     = 0x0080,
    CF_PALETTE          = 9,
    CF_PENDATA          = 10,
    CF_PRIVATEFIRST     = 0x0200,
    CF_PRIVATELAST      = 0x02FF,
    CF_RIFF             = 11,
    CF_SYLK             = 4,
    CF_TIFF             = 6,
    CF_UNICODETEXT      = 13,
    CF_WAVE             = 12    
};

const WHandle INVALID_HANDLE_VALUE = (WHandle)(u64)(-1);


constexpr u64 STANDARD_RIGHTS_REQUIRED  = 0x000F0000L;
constexpr u64 SYNCHRONIZE               = 0x00100000L;
constexpr u64 SEMAPHORE_ALL_ACCESS      = (STANDARD_RIGHTS_REQUIRED | SYNCHRONIZE | 0x3);
constexpr u64 INFINITE                  = 0xFFFFFFFF;


enum WFileAcces
{
    GENERIC_READ    = 0x80000000L,
    GENERIC_WRITE   = 0x40000000L,
    GENERIC_EXECUTE = 0x20000000L,
    GENERIC_ALL     = 0x10000000L    
};


enum WFileOpenMode
{
    CREATE_NEW          = 1,
    CREATE_ALWAYS       = 2,
    OPEN_EXISTING       = 3,
    OPEN_ALWAYS         = 4,
    TRUNCATE_EXISTING   = 5    
};


enum WFileAttribute
{
    FILE_ATTRIBUTE_READONLY              = 0x00000001,  
    FILE_ATTRIBUTE_HIDDEN                = 0x00000002,  
    FILE_ATTRIBUTE_SYSTEM                = 0x00000004,  
    FILE_ATTRIBUTE_DIRECTORY             = 0x00000010,  
    FILE_ATTRIBUTE_ARCHIVE               = 0x00000020,  
    FILE_ATTRIBUTE_DEVICE                = 0x00000040,  
    FILE_ATTRIBUTE_NORMAL                = 0x00000080,  
    FILE_ATTRIBUTE_TEMPORARY             = 0x00000100,  
    FILE_ATTRIBUTE_SPARSE_FILE           = 0x00000200,  
    FILE_ATTRIBUTE_REPARSE_POINT         = 0x00000400,  
    FILE_ATTRIBUTE_COMPRESSED            = 0x00000800,  
    FILE_ATTRIBUTE_OFFLINE               = 0x00001000,  
    FILE_ATTRIBUTE_NOT_CONTENT_INDEXED   = 0x00002000,  
    FILE_ATTRIBUTE_ENCRYPTED             = 0x00004000,  
    FILE_ATTRIBUTE_INTEGRITY_STREAM      = 0x00008000,  
    FILE_ATTRIBUTE_VIRTUAL               = 0x00010000,  
    FILE_ATTRIBUTE_NO_SCRUB_DATA         = 0x00020000,  
    FILE_ATTRIBUTE_EA                    = 0x00040000,  
    FILE_ATTRIBUTE_PINNED                = 0x00080000,  
    FILE_ATTRIBUTE_UNPINNED              = 0x00100000,  
    FILE_ATTRIBUTE_RECALL_ON_OPEN        = 0x00040000,  
    FILE_ATTRIBUTE_RECALL_ON_DATA_ACCESS = 0x00400000
};


enum WPixel_Formats : u32
{
    PFD_DOUBLEBUFFER            = 0x00000001,
    PFD_STEREO                  = 0x00000002,
    PFD_DRAW_TO_WINDOW          = 0x00000004,
    PFD_DRAW_TO_BITMAP          = 0x00000008,
    PFD_SUPPORT_GDI             = 0x00000010,
    PFD_SUPPORT_OPENGL          = 0x00000020,
    PFD_GENERIC_FORMAT          = 0x00000040,
    PFD_NEED_PALETTE            = 0x00000080,
    PFD_NEED_SYSTEM_PALETTE     = 0x00000100,
    PFD_SWAP_EXCHANGE           = 0x00000200,
    PFD_SWAP_COPY               = 0x00000400,
    PFD_SWAP_LAYER_BUFFERS      = 0x00000800,
    PFD_GENERIC_ACCELERATED     = 0x00001000,
    PFD_SUPPORT_DIRECTDRAW      = 0x00002000,
    PFD_DIRECT3D_ACCELERATED    = 0x00004000,
    PFD_SUPPORT_COMPOSITION     = 0x00008000,
    PFD_DEPTH_DONTCARE          = 0x20000000,
    PFD_DOUBLEBUFFER_DONTCARE   = 0x40000000,
    PFD_STEREO_DONTCARE         = 0x80000000
};


constexpr u8 WHEEL_DELTA = 120; // https://devblogs.microsoft.com/oldnewthing/20130123-00/?p=5473
const WWindowHandle HWND_TOP        = WWindowHandle(0);
const WWindowHandle HWND_BOTTOM     = WWindowHandle(1);
const WWindowHandle HWND_TOPMOST    = WWindowHandle(-1);
const WWindowHandle HWND_NOTOPMOST  = WWindowHandle(-2);


enum WSWP
{
    SWP_NOSIZE                  = 0x0001,
    SWP_NOMOVE                  = 0x0002,
    SWP_NOZORDER                = 0x0004,
    SWP_NOREDRAW                = 0x0008,
    SWP_NOACTIVATE              = 0x0010,
    SWP_FRAMECHANGED            = 0x0020,
    SWP_SHOWWINDOW              = 0x0040,
    SWP_HIDEWINDOW              = 0x0080,
    SWP_NOCOPYBITS              = 0x0100,
    SWP_NOOWNERZORDER           = 0x0200,
    SWP_NOSENDCHANGING          = 0x0400    
};


enum WGWL
{
    GWL_EXSTYLE                 = -20,
    GWL_HINSTANCE               = -6,
    GWL_ID                      = -12,
    GWL_STYLE                   = -16,
    GWL_USERDATA                = -21,
    GWL_WNDPROC                 = -4
};


enum WMONITOR
{
    MONITOR_DEFAULTTONULL       = 0x00000000,
    MONITOR_DEFAULTTOPRIMARY    = 0x00000001,
    MONITOR_DEFAULTTONEAREST    = 0x00000002    
};


enum WSRC : u32
{
    SRCCOPY    = 0x00CC0020, /* dest = source                   */
    SRCPAINT   = 0x00EE0086, /* dest = source OR dest           */
    SRCAND     = 0x008800C6, /* dest = source AND dest          */
    SRCINVERT  = 0x00660046, /* dest = source XOR dest          */
    SRCERASE   = 0x00440328, /* dest = source AND (NOT dest )   */
    NOTSRCCOPY = 0x00330008, /* dest = (NOT source)             */
    NOTSRCERASE= 0x001100A6, /* dest = (NOT src) AND (NOT dest) */
    MERGECOPY  = 0x00C000CA, /* dest = (source AND pattern)     */
    MERGEPAINT = 0x00BB0226, /* dest = (NOT source) OR dest     */
    PATCOPY    = 0x00F00021, /* dest = pattern                  */
    PATPAINT   = 0x00FB0A09, /* dest = DPSnoo                   */
    PATINVERT  = 0x005A0049, /* dest = pattern XOR dest         */
    DSTINVERT  = 0x00550009, /* dest = (NOT dest)               */
    BLACKNESS  = 0x00000042, /* dest = BLACK                    */
    WHITENESS  = 0x00FF0062 /* dest = WHITE                    */
};
