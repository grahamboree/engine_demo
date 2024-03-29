#include "gfx.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "core.h"
#include "util/io.h"
#include "mesh.h"

namespace {
ankerl::unordered_dense::map<std::string, TextureHandle> textureHandles;

void LogShaderCompileErrors(unsigned int shaderID) {
    int success;
    char infolog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success); glCheckErrors();
    if (!success) {
        glGetShaderInfoLog(shaderID, 512, NULL, infolog); glCheckErrors();
        printf("%s\n", infolog);
    }
}

void LogProgramLinkErrors(unsigned int programID) {
    int success;
    char infolog[512];
    glGetProgramiv(programID, GL_LINK_STATUS, &success); glCheckErrors();
    if (!success) {
        glGetProgramInfoLog(programID, 512, NULL, infolog); glCheckErrors();
        printf("%s\n", infolog);
    }
}

uint32_t CompileShaderProgram(const char* vertexShaderCode, const char* fragmentShaderCode) {
	// vertex shader
    auto vs = glCreateShader(GL_VERTEX_SHADER); glCheckErrors();
    glShaderSource(vs, 1, &vertexShaderCode, NULL); glCheckErrors();
    glCompileShader(vs); glCheckErrors();
    LogShaderCompileErrors(vs);

    // fragment shader
    auto fs = glCreateShader(GL_FRAGMENT_SHADER); glCheckErrors();
    glShaderSource(fs, 1, &fragmentShaderCode, NULL); glCheckErrors();
    glCompileShader(fs); glCheckErrors();
    LogShaderCompileErrors(fs);

    // compile and link program
    ProgramHandle program = glCreateProgram(); glCheckErrors();
    glAttachShader(program, vs); glCheckErrors();
    glAttachShader(program, fs); glCheckErrors();
    glLinkProgram(program); glCheckErrors();
    LogProgramLinkErrors(program);

    // Don't need shader info anymore
    glDeleteShader(vs); glCheckErrors();
    glDeleteShader(fs); glCheckErrors();
    
    return program;
}
}

void glCheckErrors() {
#ifdef DEBUG
    GLenum err;
    while((err = glGetError()) != GL_NO_ERROR) {
        switch (err) {
        case GL_INVALID_ENUM: log(LogCategory::GFX, "OpenGL Error: GL_INVALID_ENUM\n"); break;
        case GL_INVALID_VALUE: log(LogCategory::GFX, "OpenGL Error: GL_INVALID_VALUE\n"); break;
        case GL_INVALID_OPERATION: log(LogCategory::GFX, "OpenGL Error: GL_INVALID_OPERATION\n"); break;
        case GL_STACK_OVERFLOW: log(LogCategory::GFX, "OpenGL Error: GL_STACK_OVERFLOW\n"); break;
        case GL_STACK_UNDERFLOW: log(LogCategory::GFX, "OpenGL Error: GL_STACK_UNDERFLOW\n"); break;
        case GL_OUT_OF_MEMORY: log(LogCategory::GFX, "OpenGL Error: GL_OUT_OF_MEMORY\n"); break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: log(LogCategory::GFX, "OpenGL Error: GL_INVALID_FRAMEBUFFER_OPERATION\n"); break;
        case GL_TABLE_TOO_LARGE: log(LogCategory::GFX, "OpenGL Error: GL_TABLE_TOO_LARGE\n"); break;
        default: log(LogCategory::GFX, "OpenGL Error: Unknown error 0x%08x\n", err); break;
        }
    }
#endif
}

ProgramHandle LoadProgram(const char* vertPath, const char* fragPath) {
    char* vertexShaderCode = IO::ReadFile(vertPath);
    char* fragmentShaderCode = IO::ReadFile(fragPath);
    ProgramHandle programID = CompileShaderProgram(vertexShaderCode, fragmentShaderCode);
    delete[] vertexShaderCode;
    delete[] fragmentShaderCode;
    return programID;
}

uint32_t UploadMesh(const Mesh& mesh) {
    size_t posDataSize = sizeof(glm::vec3); // position
    size_t uvDataSize = sizeof(glm::vec2); // uv
    size_t normDataSize = sizeof(glm::vec3); // normals
    size_t stride = sizeof(struct Mesh::VertexData);
    
    // create a vertex array object
    uint32_t vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // create a vertex buffer object
    uint32_t vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // upload vertex data
    glBufferData(GL_ARRAY_BUFFER, mesh.vertexData.size() * stride, mesh.vertexData.data(), GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), nullptr);
    glEnableVertexAttribArray(0);

    // uv
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void*)(long)posDataSize);
    glEnableVertexAttribArray(1);

    // normal
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, static_cast<GLsizei>(stride), (void*)(long)(posDataSize + uvDataSize));
    glEnableVertexAttribArray(2);

    // color
    glVertexAttribPointer(3, 4, GL_UNSIGNED_BYTE, GL_TRUE, static_cast<GLsizei>(stride), (void*)(long)(posDataSize + uvDataSize + normDataSize));
    glEnableVertexAttribArray(3);

    return vao;
}

void InitRendering(Renderer& state, int windowWidth, int windowHeight, int& viewportWidth, int& viewportHeight) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Could not initialize SDL.\nError: %s\n", SDL_GetError());
        exit(1);
    }

    // Set gl version to 3.2
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Enable depth buffer.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // 4x MSAA.
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    // init an opengl window
    state.window = SDL_CreateWindow(
        "engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        windowWidth,
        windowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    if (!state.window) {
        fprintf(stderr, "Could not create window.\nError: %s\n", SDL_GetError());
        exit(1);
    }
    state.mainWindowID = SDL_GetWindowID(state.window);

    state.glContext = SDL_GL_CreateContext(state.window);
    if (!state.glContext) {
        fprintf(stderr, "Could not initialize SDL opengl context\nError: %s\n", SDL_GetError());
        exit(1);
    }

    // set the viewport
    SDL_GL_GetDrawableSize(state.window, &viewportWidth, &viewportHeight);
    glViewport(0, 0, viewportWidth, viewportHeight); glCheckErrors();
    glDisable(GL_CULL_FACE); glCheckErrors();
    glEnable(GL_MULTISAMPLE); glCheckErrors();
    glEnable(GL_DEPTH_TEST); glCheckErrors();
    glDepthFunc(GL_LEQUAL); glCheckErrors();

    glClearColor(0.3f, 0.3f, 0.32f, 1.0f); glCheckErrors();

#if _WIN32
    glewInit();
#endif
}

TextureHandle LoadTexture(const std::string& filename) {
    const auto foundData = textureHandles.find(filename);
    if (foundData != textureHandles.end()) {
        return foundData->second;
    }

    int imageWidth;
    int imageHeight;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename.c_str(), &imageWidth, &imageHeight, nullptr, 4);
    if (!data)
    {
        log(LogCategory::GFX, "Could not load texture at path %s", filename.c_str());
        return TextureHandle_Invalid;
    }

    TextureHandle textureHandle;
    glGenTextures(1, &textureHandle); glCheckErrors();
    glBindTexture(GL_TEXTURE_2D, textureHandle); glCheckErrors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); glCheckErrors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); glCheckErrors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glCheckErrors();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); glCheckErrors();
    
    // bool isPowerOfTwoTexture =
    //     ((imageWidth & (imageWidth - 1)) == 0) &&
    //     ((imageHeight & (imageHeight - 1)) == 0);

    // if (!isPowerOfTwoTexture) {
    //     // This is required on WebGL for non power-of-two textures
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); glCheckErrors();
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); glCheckErrors();
    // }
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data); glCheckErrors();
    
    stbi_image_free(data);

    textureHandles[filename] = textureHandle;
    return textureHandle;
}

void UnloadTexture(TextureHandle textureHandle) {
    glDeleteTextures(1, &textureHandle); glCheckErrors();
}
