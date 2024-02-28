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
void InitFramebuffers(uint32_t* framebuffers, uint32_t* framebufferTextures, int viewportWidth, int viewportHeight);
uint32_t InitFullscreenQuad();

// Shaders
ProgramHandle CompileShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode);
ProgramHandle LoadProgram(const char* vertPath, const char* fragPath);
ProgramHandle LoadPostprocessingProgram(const char* vertPath, const char* fragPath);

// Textures
TextureHandle LoadTexture(const std::string& filename);
TextureHandle LoadCheckerboardTexture();
void UnloadTexture(TextureHandle textureHandle);

// Meshes
uint32_t UploadMesh(const Mesh& mesh);
uint32_t DefaultQuad();
