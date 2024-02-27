#pragma once

#include "lua/lua.hpp"

// Init the lua scripting runtime.  Registers all interop functions.
lua_State* InitLua();

// Check for and report errors in lua execution
void lua_check(int result);

// Dump the lua stack (for debugging lua interop)
void dump_stack();