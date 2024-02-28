#include "engine.h"

bool EngineState::UpdateAndRender()
{
    // calculate dt
    const auto frameStart = std::chrono::high_resolution_clock::now();
    const auto deltaTime = frameStart - lastFrameStart;
    lastFrameStart = frameStart;

    // Process SDL Events
    bool shouldExit = false;
    for (SDL_Event event; SDL_PollEvent(&event);) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        input.ProcessSDLEvent(&event);

        shouldExit |= event.type == SDL_QUIT;
        shouldExit |= event.type == SDL_WINDOWEVENT &&
                event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == renderer.mainWindowID;
    }

    glm::mat4 viewProjection = camera.CalculateViewProjectionMatrix();
    
    // Update
    {
        input.readInput();

        float dt_seconds = std::chrono::duration_cast<std::chrono::duration<float>>(deltaTime).count();

        // Camera controls
        // camera.move(dt_seconds, input, renderer.window, windowHeight);

        // Call lua update, passing the delta time in seconds.
        lua_getglobal(luaState, "update");
        lua_pushnumber(luaState, dt_seconds);
        lua_check(lua_pcall(luaState, 1, 0, 0));

        // Compute MVP matricesv
        for (const auto& entity : entities.drawList) {
            entities.UpdateLocalTransformMatrix(entity);
        }
        for (const auto& entity : entities.drawList) {
            entities.UpdateMvpMatrix(entity, viewProjection);
        }
    }

    // Render
    const auto renderStart = std::chrono::high_resolution_clock::now();
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); glCheckErrors();

        // Draw entities
        for (const auto& entity : entities.drawList) {
            // DrawEntity(entity);
            const auto& drawData = entities.drawInfo[entity];
            glBindVertexArray(drawData.vao); glCheckErrors();
            glUseProgram(drawData.program); glCheckErrors();
            glActiveTexture(GL_TEXTURE0); glCheckErrors();
            glBindTexture(GL_TEXTURE_2D, drawData.texture); glCheckErrors();
            glUniformMatrix4fv(glGetUniformLocation(drawData.program, "MVP"), 1, GL_FALSE, glm::value_ptr(entities.MVPMatrix[entity])); glCheckErrors();
            glDrawArrays(GL_TRIANGLES, 0, drawData.vertCount); glCheckErrors();
        }

        ui.UpdateAndRender(renderer, stats, camera);
    }
    SDL_GL_SwapWindow(renderer.window);

    // Record frame perf stats
    const auto frameEnd = std::chrono::high_resolution_clock::now();
    stats.RecordFrameTime(frameEnd - frameStart);
    stats.RecordUpdateTime(renderStart - frameStart);
    stats.RecordRenderTime(frameEnd - renderStart);

    return shouldExit;
}

void EngineState::Init() {
    // Init subsystems
    InitRendering(renderer, windowWidth, windowHeight, camera.viewportWidth, camera.viewportHeight);

    ui.Init(renderer);
    luaState = InitLua();

    // Create a camera
    camera.pos = {0, 0, 10};
    camera.euler = {0, 0};
    camera.SetOrtho(40);

    // Load the default entity draw data.
    const Mesh defaultQuad = Mesh::DefaultQuad();
    DrawData::Default.vao = UploadMesh(defaultQuad);
    DrawData::Default.vertCount = static_cast<uint32_t>(defaultQuad.vertexData.size());
    DrawData::Default.texture = LoadTexture(DATA_PATH "Sprites/player.png");
    DrawData::Default.program = LoadProgram(DATA_PATH "Shaders/vert.glsl", DATA_PATH "Shaders/frag.glsl");

    // Run lua setup
    lua_check(luaL_dofile(luaState, DATA_PATH "Scripts/luamain.lua"));

    gameStartTime = lastFrameStart = std::chrono::high_resolution_clock::now();
}

void EngineState::Shutdown() {
    // Destroy the imgui context
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImPlot::DestroyContext();
    ImGui::DestroyContext();

    // Release the Lua enviroment
    lua_close(luaState);

    // Destroy the SDL context and quit
    SDL_GL_DeleteContext(renderer.glContext);
    SDL_DestroyWindow(renderer.window);
    SDL_Quit();
}
