
#pragma once


#include <Windows.h>
#include <Xinput.h>

#include "Utility\Primitives.h"
#include "Utility\Intrinsics.h"
#include "Utility\Instrumentation.h"
#include "Utility\Assert.h"
#include "Utility\Utility.h"
#include "Utility\Allocator_Shell.h"
#include "Utility\Dynamic_Array.h"
#include "Utility\String.h"
#include "Platform\Input.h"
#include "Platform\Platform_Interface.h"

#include "Platform\Win32.cpp"

#include "App\Main_App.h"

static const wchar_t* APP_TITLE = L"TIAD";

Declare_Timing_Tables();

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{    
	Win32_Init(32 * 12, 32 * 22 + 32 * 6, CW_USEDEFAULT, CW_USEDEFAULT, APP_TITLE);
	
	Platform_Calltable calltable = Win32_Get_Calltable();
	Init_App(calltable);
	
	while(Is_Flag_Set(s_app.flags, (u32)App_Flags::is_running))
    {
		Begin_Timing_Block(run_time);
		
		Win32_Flush_Events();
		
		Update_App();
		
		Win32_Update_Surface(true);
		
		Win32_Update_Frame_Time();
		
		End_Timing_Block(run_time);
		
		Instrumentation_Call(Win32_Report_Timing_Results());
    }
	
	return 0;
}