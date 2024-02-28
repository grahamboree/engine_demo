#include "lua_api.h"

#include "lua/lua.hpp"

#include <algorithm>

#include "util/logging.h"
#include "rendering/gfx.h"
#include "entity.h"
#include "input.h"

#include "engine.h"

extern EngineState engine;

namespace {
extern "C" {
    int l_log(lua_State *L) {
        size_t messageLen;
        const char* message = luaL_checklstring(L, 1, &messageLen);
        log(LogCategory::LUA, message);
        return 0;
    }

    int l_load_texture(lua_State *L) {
        size_t textureNameLen;
        std::string textureName = DATA_PATH;
        textureName.append(luaL_checklstring(L, 1, &textureNameLen));
        const TextureHandle handle = LoadTexture(textureName);
        lua_pushnumber(L, handle);
        return 1;
    }

    int l_create_entity(lua_State *L) {
        const EntityID entity = engine.entities.CreateEntity();
        engine.entities.drawInfo[entity] = DrawData::Default;
        lua_pushnumber(L, entity);
        return 1;
    }

    int l_get_pos(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));

        if (entity == EntityID_Invalid) {
            luaL_error(L, "Invalid Entity ID.");
        }

        const glm::vec2 pos = engine.entities.positions[entity];
        lua_newtable(L);

        lua_pushstring(L, "x");
        lua_pushnumber(L, pos.x);
        lua_settable(L, -3);

        lua_pushstring(L, "y");
        lua_pushnumber(L, pos.y);
        lua_settable(L, -3);

        return 1;
    }

    int l_set_pos(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));

        lua_getfield(L, 2, "x");
        float x = static_cast<float>(lua_tonumber(L, -1));

        lua_getfield(L, 2, "y");
        float y = static_cast<float>(lua_tonumber(L, -1));

        float z = engine.entities.positions[entity].z;
        engine.entities.positions[entity] = {x, y, z};

        return 0;
    }

    int l_set_zpos(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));
        engine.entities.positions[entity].z = static_cast<float>(lua_tonumber(L, 2));
        return 0;
    }

    int l_get_scale(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));

        const glm::vec2 scale = engine.entities.scales[entity];
        lua_newtable(L);

        lua_pushstring(L, "x");
        lua_pushnumber(L, scale.x);
        lua_settable(L, -3);

        lua_pushstring(L, "y");
        lua_pushnumber(L, scale.y);
        lua_settable(L, -3);

        return 1;
    }

    int l_set_scale(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));

        lua_getfield(L, 2, "x");
        float x = static_cast<float>(lua_tonumber(L, -1));

        lua_getfield(L, 2, "y");
        float y = static_cast<float>(lua_tonumber(L, -1));

        engine.entities.scales[entity] = {x, y};

        return 0;
    }

    int l_get_pivot(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));

        glm::vec2 pivot = engine.entities.pivots[entity];
        lua_newtable(L);

        lua_pushstring(L, "x");
        lua_pushnumber(L, pivot.x);
        lua_settable(L, -3);

        lua_pushstring(L, "y");
        lua_pushnumber(L, pivot.y);
        lua_settable(L, -3);

        return 1;
    }

    int l_set_pivot(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));

        lua_getfield(L, 2, "x");
        float x = static_cast<float>(lua_tonumber(L, -1));

        lua_getfield(L, 2, "y");
        float y = static_cast<float>(lua_tonumber(L, -1));

        engine.entities.pivots[entity] = {x, y};

        return 0;
    }

    int l_set_texture(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));
        const TextureHandle texture = static_cast<TextureHandle>(lua_tonumber(L, 2));
        engine.entities.drawInfo[entity].texture = texture;
        return 0;
    }

    int l_add_to_drawlist(lua_State *L) {
        const EntityID entity = static_cast<EntityID>(lua_tonumber(L, 1));
        engine.entities.drawList.push_back(entity);
        return 0;
    }

    int l_get_key(lua_State *L) {
        size_t keyLen;
        std::string key = luaL_checklstring(L, 1, &keyLen);
        std::transform(key.begin(), key.end(), key.begin(), [](unsigned char c){ return std::tolower(c); });

        bool result = false;
        if (key == "up") { result = engine.input.up; }
        else if (key == "down") { result = engine.input.down; }
        else if (key == "left") { result = engine.input.left; }
        else if (key == "right") { result = engine.input.right; }
        else if (key == "mouseleft") { result = engine.input.mouseLeft; }
        else if (key == "mouseright") { result = engine.input.mouseRight; }
        else { luaL_error(L, "Key '%s' not found."); }

        lua_pushboolean(L, result);
        return 1;
    }
}

void lua_check(lua_State* L, int result) {
    if (result == LUA_ERRSYNTAX) { // syntax error
        log(LogCategory::LUA, "Syntax error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    } else if (result == LUA_ERRMEM) { // memory allocation error
        log(LogCategory::LUA, "Memory allocation error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    } else if (result == LUA_ERRFILE) { // file read error
        log(LogCategory::LUA, "File reading error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    } else if (result) {
        log(LogCategory::LUA, "Error: %s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }
}

// Dump the lua stack (for debugging lua interop)
void DumpStack(lua_State* L) {
    int top = lua_gettop(L);
    for (int i = 1; i <= top; i++) {  /* repeat for each level */
        const int type = lua_type(L, i);

        switch (type) {
        case LUA_TSTRING:  /* strings */
            printf("`%s'", lua_tostring(L, i));
            break;

        case LUA_TBOOLEAN:  /* booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;

        case LUA_TNUMBER:  /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;

        default:  /* other values */
            printf("%s", lua_typename(L, type));
            break;
        }
        printf("  ");
    }
    printf("\n");
}
}

void LuaRuntime::Init() {
    L = luaL_newstate();
    luaL_openlibs(L); // Load standard libs

    // Set the lua search path to include the data/Scripts directory
    lua_getglobal(L, "package");
    lua_getfield(L, -1, "path");
    std::string luaPath = lua_tostring(L, -1);
    luaPath.append(";" DATA_PATH "Scripts/?/?.lua");
    luaPath.append(";" DATA_PATH "Scripts/?.lua");
    lua_pop(L, 1);
    lua_pushstring(L, luaPath.c_str());
    lua_setfield(L, -2, "path");
    lua_pop(L, 1);
    
#define REGISTER_LUA_FUNC(func) lua_pushcfunction(L, l_##func); lua_setglobal(L, #func);
    REGISTER_LUA_FUNC(log)
    REGISTER_LUA_FUNC(create_entity)
    REGISTER_LUA_FUNC(get_pos)
    REGISTER_LUA_FUNC(set_pos)
    REGISTER_LUA_FUNC(set_zpos)
    REGISTER_LUA_FUNC(get_scale)
    REGISTER_LUA_FUNC(set_scale)
    REGISTER_LUA_FUNC(get_pivot)
    REGISTER_LUA_FUNC(set_pivot)
    REGISTER_LUA_FUNC(set_texture)
#undef REGISTER_LUA_FUNC
    
#define REGISTER_LUA_METHOD(method) lua_pushcfunction(L, l_##method); lua_setfield(L, -2, #method);
    // gfx
    lua_newtable(L);
    REGISTER_LUA_METHOD(load_texture)
    REGISTER_LUA_METHOD(add_to_drawlist)
    lua_setglobal(L, "gfx");

    // io
    lua_newtable(L);
    REGISTER_LUA_METHOD(get_key)
    lua_setglobal(L, "io");
#undef REGISTER_LUA_METHOD
}

void LuaRuntime::Shutdown() {
    // Release the Lua enviroment
    lua_close(L);
}

void LuaRuntime::CallUpdate(float dt) {
    // Call lua update, passing the delta time in seconds.
    lua_getglobal(L, "update");
    lua_pushnumber(L, dt);
    lua_check(L, lua_pcall(L, 1, 0, 0));
}

void LuaRuntime::RunFile(const char* path) {
    lua_check(L, luaL_dofile(L, path));
}
