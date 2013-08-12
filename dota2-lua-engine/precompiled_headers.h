// Copyright 2013 Karl Skomski - GPL v3
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

#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4005) // macro redefinition
#include <stdint.h>
#pragma warning (push)
#pragma warning (default : 4005)
#endif

#include "LuaJIT/src/lua.hpp"
#include "LuaBridge/LuaBridge.h"

#include <memory>
#include <set>
#include <functional>
#include <algorithm>
