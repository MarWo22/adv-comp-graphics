#include <GL/glew.h>
#include <chrono>
#include <iostream>

#include <imgui.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/ext.hpp>

using namespace glm;

#include "textures/texture.h"
#include "shader.h"
#include "camera.h"
#include "terrain/file.h"
#include "terrain/grid.h"
#include "terrain/new/chunkManager.h"
#include "guiVariables.h"
#include "terrain/new/terrainManager.h"

///////////////////////////////////////////////////////////////////////////////
// Various globals
///////////////////////////////////////////////////////////////////////////////
float currentTime = 0.0f;
float previousTime = 0.0f;
float deltaTime = 0.0f;
int windowWidth, windowHeight;

GLuint backgroundVAO;
GLuint quadVAO;

float environment_multiplier = 1.5f;
Shader background;
Shader simpleShader;
Shader densityShader;
Shader voxelShader;
Shader prefixSumFirstPassShader;
Shader prefixSumSecondPassShader;
Shader prefixSumThirdPassShader;
Shader voxelGeometryShader;
Shader terrainShader;
Texture environmentMap;

TerrainManager terrainManager;

Camera camera;
Camera stationaryCamera;
void loadShaders()
{
    background = Shader("./shaders/fullscreenQuad.vert", "./shaders/background.frag");
    simpleShader = Shader("./shaders/simple.vert", "./shaders/simple.frag");
    densityShader = Shader("./shaders/density.comp", GL_COMPUTE_SHADER);
    voxelShader = Shader("./shaders/blockPolygons.comp", GL_COMPUTE_SHADER);
    prefixSumFirstPassShader = Shader("./shaders/prefixSumFirst.comp", GL_COMPUTE_SHADER);
    prefixSumSecondPassShader = Shader("./shaders/prefixSumSecond.comp", GL_COMPUTE_SHADER);
    prefixSumThirdPassShader = Shader("./shaders/prefixSumThird.comp", GL_COMPUTE_SHADER);
    voxelGeometryShader = Shader("./shaders/voxelGeometry.comp", GL_COMPUTE_SHADER);
    terrainShader = Shader("./shaders/terrain.vert", "./shaders/terrain.frag");
}

void loadTextures()
{
    environmentMap = Texture("./assets/lighting-maps/001.hdr");
}

void generateQuad()
{
    // Define vertex data for a quad (two triangles)
    GLfloat vertices[] = {
            // Positions
            -50.0f, 0.0f, -50.0f,
            -50.0f, 0.0f,  50.0f,
            50.0f, 0.0f,  50.0f,
            50.0f, 0.0f, -50.0f,
    };

    // Define indices for the quad
    GLuint indices[] = {
            0, 1, 2,
            0, 2, 3,
    };
    glGenVertexArrays(1, &quadVAO);
    GLuint positionBuffer, indexBuffer;

    glGenBuffers(1, &positionBuffer);
    glGenBuffers(1, &indexBuffer);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(0);

}

void generateBackground()
{
    glGenVertexArrays(1, &backgroundVAO);
    static const glm::vec2 positions[] = { { -1.0f, -1.0f }, { 1.0f, -1.0f }, { 1.0f, 1.0f },
                                           { -1.0f, -1.0f }, { 1.0f, 1.0f },  { -1.0f, 1.0f } };
    GLuint buffer = 0;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    // Now attach buffer to vertex array object.
    glBindVertexArray(backgroundVAO);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, nullptr);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


///////////////////////////////////////////////////////////////////////////////
/// This function is called once at the start of the program and never again
///////////////////////////////////////////////////////////////////////////////
void initialize()
{
    camera = Camera(vec3(-70.0f, 50.0f, 70.0f), 45.f, 25.f, 5.f, 0.4f);
    stationaryCamera = Camera(vec3(-70.0f, 50.0f, 70.0f), 45.f, 25.f, 5.f, 0.4f);

    loadShaders();
    loadTextures();
    generateBackground();
    generateQuad();
    glEnable(GL_DEPTH_TEST); // enable Z-buffering
    glEnable(GL_CULL_FACE);  // enables backface culling

    terrainManager = TerrainManager{&densityShader,
                                    &voxelShader,
                                    &prefixSumFirstPassShader,
                                    &prefixSumSecondPassShader,
                                    &prefixSumThirdPassShader,
                                    &voxelGeometryShader,
                                    &terrainShader};

}

void drawQuad(mat4 viewMatrix, mat4 projectionMatrix)
{
    simpleShader.Bind();
    simpleShader.SetUniform("modelViewProjectionMatrix", projectionMatrix * viewMatrix);
    glBindVertexArray(quadVAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}


void drawBackground(const mat4& viewMatrix, const mat4& projectionMatrix)
{

    background.Bind();
    background.SetUniform("environment_multiplier", environment_multiplier);
    background.SetUniform("inv_PV", inverse(projectionMatrix * viewMatrix));
    background.SetUniform("camera_pos", camera.Pos());

    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(backgroundVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

///////////////////////////////////////////////////////////////////////////////
/// This function will be called once per frame, so the code to set up
/// the scene for rendering should go here
///////////////////////////////////////////////////////////////////////////////
void display()
{
    environmentMap.Bind(GL_TEXTURE6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    drawBackground(camera.ViewMatrix(), camera.ProjectionMatrix());
//    drawQuad(camera.ViewMatrix(), camera.ProjectionMatrix());
    stationaryCamera.DrawFrustum(simpleShader, camera);


    if (GuiVariables::s_ChunkDrawMode != GuiVariables::ChunkDrawMode::None)
        terrainManager.RenderExtras(simpleShader, camera.ProjectionMatrix() * camera.ViewMatrix());

    terrainManager.RenderTerrain(camera);

}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_NORMAL)
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        else
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    camera.HandleKeyInput(key, action);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (glfwGetInputMode(window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED)
        camera.HandleMouseInput((float)xPos, (float)yPos, deltaTime);
}

static float frameCount = 0;
static float avgFramerate = 0;
void gui()
{
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);

    avgFramerate = (avgFramerate * frameCount + ImGui::GetIO().Framerate) / (frameCount + 1);
    frameCount++;




    ImGui::Text("Average %.1f FPS", avgFramerate);

    if (ImGui::CollapsingHeader("Chunking"))
    {
        ImGui::Text("Draw outlines:");
        ImGui::RadioButton("None", &GuiVariables::s_ChunkDrawMode, GuiVariables::ChunkDrawMode::None);
        ImGui::RadioButton("Chunks", &GuiVariables::s_ChunkDrawMode, GuiVariables::ChunkDrawMode::Chunks);
        ImGui::RadioButton("Blocks", &GuiVariables::s_ChunkDrawMode, GuiVariables::ChunkDrawMode::Blocks);

        ImGui::Text("LOD outlines:");
        ImGui::Checkbox("LOD 0", &GuiVariables::s_DrawLODs[0]);
        ImGui::Checkbox("LOD 1", &GuiVariables::s_DrawLODs[1]);
        ImGui::Checkbox("LOD 2", &GuiVariables::s_DrawLODs[2]);
        ImGui::Checkbox("LOD 3", &GuiVariables::s_DrawLODs[3]);
        ImGui::Checkbox("LOD 4", &GuiVariables::s_DrawLODs[4]);

        ImGui::Text("Settings:");
        ImGui::SliderFloat("LOD level distance", &GuiVariables::s_LODdistance, 5, 25);
        ImGui::SliderInt("Render Distance (Chunks)", &GuiVariables::s_RenderDistance, 5, 25);
    }

    if (ImGui::CollapsingHeader("Terrain"))
    {
        ImGui::Text("Draw mode:");
        ImGui::RadioButton("Disabled", &GuiVariables::s_TerrainDrawMode, GuiVariables::TerrainDrawMode::Disabled);
        ImGui::RadioButton("Fill", &GuiVariables::s_TerrainDrawMode, GuiVariables::TerrainDrawMode::Fill);
        ImGui::RadioButton("Wireframe", &GuiVariables::s_TerrainDrawMode, GuiVariables::TerrainDrawMode::Wireframe);

        ImGui::Text("Settings:");
        ImGui::Checkbox("Display Normals", &GuiVariables::s_DisplayNormals);
        ImGui::Checkbox("Cache SSBOs", &GuiVariables::s_EnableSSBOCaching);
        ImGui::Checkbox("Cache Empty Blocks", &GuiVariables::s_EnableEmptyBlockCaching);
        ImGui::Checkbox("Fixed Camera Terrain", &GuiVariables::s_StationaryCam);
    }

    if (ImGui::CollapsingHeader("Noise Settings"))
    {
        ImGui::Checkbox("Octave lvl 1", &GuiVariables::s_Octaves[0]);
        ImGui::Checkbox("Octave lvl 2", &GuiVariables::s_Octaves[1]);
        ImGui::Checkbox("Octave lvl 3", &GuiVariables::s_Octaves[2]);
        ImGui::Checkbox("Octave lvl 4", &GuiVariables::s_Octaves[3]);
        ImGui::Checkbox("Octave lvl 5", &GuiVariables::s_Octaves[4]);
        ImGui::Checkbox("Octave lvl 6", &GuiVariables::s_Octaves[5]);
    }

    if (ImGui::Button("Flush caches"))
    {
        terrainManager.FlushBuffer();
    }
}


int main()
{

    auto numPolys = file::ReadNumPolys("assets/tables/NumPolys.txt");
    auto trisTable = file::ReadTrisTable("assets/tables/TrisTable.txt");

    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW\n";
        glfwTerminate();
        return -1;
    }

    initialize();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    auto startTime = std::chrono::system_clock::now();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
            // Measure time since last frame
        std::chrono::duration<float> timeSinceStart = std::chrono::system_clock::now() - startTime;
        previousTime = currentTime;
        currentTime = timeSinceStart.count();
        deltaTime = currentTime - previousTime;

            // Do GLFW stuff
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glfwPollEvents();

            // Update the camera based of user input
        camera.Update(deltaTime, (float)windowWidth / (float)windowHeight);
            // Start rendering cycle: Clear the color buffer
        glViewport(0, 0, windowWidth, windowHeight);
        glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Do required ImGUI stuff
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

            // Render all tris
        display();
        gui();

            // Render the GUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap front and back buffer
        glfwSwapBuffers(window);

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
