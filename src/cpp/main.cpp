#include <GL/glew.h>
#include <chrono>
#include <iostream>

#include <imgui.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <glm/ext.hpp>

#include "sky/sky_physics.h"

using namespace glm;

#include "textures/texture.h"
#include "shader.h"
#include "camera.h"

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
Texture environmentMap;

Camera camera;

void loadShaders()
{
    background = Shader("./shaders/fullscreenQuad.vert", "./shaders/background.frag");
    simpleShader = Shader("./shaders/simple.vert", "./shaders/simple.frag");
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
            -100.0f, 0.0f, -100.0f,
            -100.0f, 0.0f,  100.0f,
            100.0f, 0.0f,  100.0f,
            100.0f, 0.0f, -100.0f,
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
    loadShaders();
    loadTextures();
    generateBackground();
    generateQuad();
    glEnable(GL_DEPTH_TEST); // enable Z-buffering
    glEnable(GL_CULL_FACE);  // enables backface culling



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

    ///////////////////////////////////////////////////////////////////////////
    // setup matrices
    ///////////////////////////////////////////////////////////////////////////
    mat4 projMatrix = perspective(radians(camera.Fov()), float(windowWidth) / float(windowHeight), 5.0f, 2000.0f);
    environmentMap.Bind(GL_TEXTURE6);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, windowWidth, windowHeight);
    glClearColor(0.2f, 0.2f, 0.8f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    drawBackground(camera.ViewMatrix(), projMatrix);
    drawQuad(camera.ViewMatrix(), projMatrix);
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
        camera.HandleMouseInput(xPos, yPos, deltaTime);
}



int main()
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    /* Make the window's context current */
    glfwMakeContextCurrent(window);

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
        std::chrono::duration<float> timeSinceStart = std::chrono::system_clock::now() - startTime;
        previousTime = currentTime;
        currentTime = timeSinceStart.count();
        deltaTime = currentTime - previousTime;

        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glfwPollEvents();
        camera.Update(deltaTime);
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        display();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}
