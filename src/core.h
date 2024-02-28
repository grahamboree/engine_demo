#pragma once

#include <cmath>

#if _WIN32
#   include <GL/glew.h>
#else
#   include <OpenGL/gl3.h>
#   include <OpenGL/gl3ext.h>
#endif

#include <SDL.h>
#include <SDL_opengl.h>

#include <lua/lua.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/implot.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <unordered_dense.h>

#include "util/logging.h"

constexpr float PI = 3.1415926545f;
constexpr float HALF_PI = 1.57079633f;

template<typename T>
T clamp(T val, T minVal, T maxVal) {
    return val > maxVal ? maxVal
        : val < minVal ? minVal
        : val;
}

inline float clamp01(float val) {
    return clamp(val, 0.0f, 1.0f);
}

using TextureHandle = GLuint;
using ProgramHandle = GLuint;
using EntityID = uint32_t;

constexpr TextureHandle TextureHandle_Invalid = 0;
constexpr ProgramHandle ProgramHandle_Invalid = 0;
constexpr EntityID EntityID_Invalid = 0;

// Location of the game data relative to the exe.
#define DATA_PATH "../data/"
