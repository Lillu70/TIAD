

// ===================================
// Copyright (c) 2024 by Valtteri Kois
// All rights reserved.
// ===================================

#pragma once

#define CNB *((volatile int*)0) = 666

#ifdef _DB

#define Assert(stament) if(!(stament)) CNB



#define DB_Call(X) X
#define Release_Call(X)

#else

#define Assert(stament)
#define DB_Call(X)
#define Release_Call(X) X

#endif // _DB

#define Terminate CNB