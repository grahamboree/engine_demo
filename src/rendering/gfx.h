#pragma once

#include "core.h"

class Mesh;

struct Renderer {
    SDL_Window* window;
    uint32_t mainWindowID;
    SDL_Renderer* renderer;
    SDL_GLContext glContext;
};

void glCheckErrors();

// Init
void InitRendering(Renderer& state, int windowWidth, int windowHeight, int& viewportWidth, int& viewportHeight);

// Shaders
ProgramHandle LoadProgram(const char* vertPath, const char* fragPath);

// Textures
TextureHandle LoadTexture(const std::string& filename);
void UnloadTexture(TextureHandle textureHandle);

// Meshes
uint32_t UploadMesh(const Mesh& mesh);
