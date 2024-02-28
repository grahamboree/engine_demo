#pragma once

struct lua_State;

struct LuaRuntime {
    lua_State* L;
    
    // Init the lua scripting runtime.  Registers all interop functions.
    void Init();
    void Shutdown();

    // Check for and report errors in lua execution
    void lua_check(int result);
    
    // Dump the lua stack (for debugging lua interop)
    void dump_stack();

    void call_update(float dt);

    void run_file(const char* path);
};
