#pragma once

struct lua_State;

struct LuaRuntime {
    lua_State* L;
    
    // Init the lua scripting runtime.  Registers all interop functions.
    void Init();
    void Shutdown();

    void CallUpdate(float dt);
    void RunFile(const char* path);
};
