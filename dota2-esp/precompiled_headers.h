#pragma once
#pragma message("Compiling precompiled header.\n")

#define CLIENT_DLL
#define COMPILER_MSVC
#define COMPILER_MSVC64

#include "dt_recv.h"
#include "mathlib\vector.h"
#include "tier1\KeyValues.h"
#include "tier1\convar.h"
#include "basehandle.h"
#include "igameevents.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <stdint.h>

#include "..\LuaJIT\src\lua.hpp"
#include "..\LuaBridge\LuaBridge.h"

#include <memory>
#include <set>