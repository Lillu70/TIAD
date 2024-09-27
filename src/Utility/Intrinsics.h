

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#ifdef _MSC_VER
#include <intrin.h>

#define Get_CPU_Cycle_Count __rdtsc

#else

static u64 __UNIMPLEMENTED(){ return 0; }

#define Get_CPU_Cycle_Count __UNIMPLEMENTED

#endif
