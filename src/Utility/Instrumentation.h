

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

/*
This is just my first attempt at building a simple timing harness.
It is not thread safe at all.
*/

#ifdef INSTRUMENTATION

#define Instrumentation_Call(X) X

struct Instrumentation_Cycle_Measurement
{
    u64 accumulated_cycle_count = 0;
    u64 hit_count = 0;
    u64 total_hit_count = 0;
    u64 total_cycle_count = 0;
    u64 lowest_cycle_count = 0;
    u64 lowest_cycles_per_hit = 0;
};


enum class __Time_Table_Names : u64
{
    run_time,
    internal_run_time,
    render_glyph,
    clear_canvas,
    scale_pixel,
    set_sub_canvas,
    img2_pixel,
    COUNT
};


#define Declare_Timing_Tables() \
char* __timing_table_labels[u64(__Time_Table_Names::COUNT)] = {};\
Instrumentation_Cycle_Measurement __timing_table[u64(__Time_Table_Names::COUNT)] = {};


extern char* __timing_table_labels[u64(__Time_Table_Names::COUNT)];
extern Instrumentation_Cycle_Measurement __timing_table[u64(__Time_Table_Names::COUNT)];


static inline void __RECORD_MEASUREMENT(
    u64 elapsed_cycles, 
    u64 idx, 
    char* human_readable_label)
{
    __timing_table_labels[idx] = human_readable_label;
    
    Instrumentation_Cycle_Measurement* icm = __timing_table + idx;
    icm->accumulated_cycle_count += elapsed_cycles;
    icm->hit_count += 1;
}


struct __Scoped_Timer 
{
    __Scoped_Timer(char* _label, u64 _block_idx) : 
        label(_label), 
        block_idx(_block_idx), 
        start_time(Get_CPU_Cycle_Count()) 
    {
        
    }
    
    ~__Scoped_Timer() // The first and only destructor in the project! Much wow.
    {
        __RECORD_MEASUREMENT(Get_CPU_Cycle_Count() - start_time, block_idx, label);
    }
    
    char* label = 0;
    u64 start_time;
    u64 block_idx;
};


#define Begin_Timing_Block(NAME) u64 INSTRUMENTATION_BEGIN_CYCLE_COUNT_##NAME = Get_CPU_Cycle_Count();
#define End_Timing_Block(NAME) __RECORD_MEASUREMENT(Get_CPU_Cycle_Count() - INSTRUMENTATION_BEGIN_CYCLE_COUNT_##NAME, u64(__Time_Table_Names::##NAME), #NAME)
#define Start_Scope_Timer(NAME) __Scoped_Timer INSTRUMENTATION_SCOPED_TIMER_##NAME##(#NAME, u64(__Time_Table_Names::##NAME))

#else

#define Declare_Timing_Tables()
#define Begin_Timing_Block(NAME)
#define End_Timing_Block(NAME)
#define Start_Scope_Timer(NAME)
#define Instrumentation_Call(X)

#endif