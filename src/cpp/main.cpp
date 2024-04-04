#include "imgui.h"
#include "glm/glm.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "shading.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "gl_misc.h"


float g_clearColor[3] = { 0.2f, 0.2f, 0.8f };

glm::vec3 g_triangleColor = { 1, 1, 1 };

GLuint vertexArrayObject;
GLuint newVertexArrayObject;
GLuint shaderProgram;

void gui()
{
    ImGui::ColorEdit3("clear color", g_clearColor);
    ImGui::ColorEdit3("triangle color", &g_triangleColor.x);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate);
}

void initialize()
{
    const float positions[] = {
            //	 X      Y     Z
            0.0f,  0.5f,  1.0f, // v0
            -0.5f, -0.5f, 1.0f, // v1
            0.5f,  -0.5f, 1.0f  // v2
    };
    GLuint positionBuffer;
    glGenBuffers(1, &positionBuffer);

    // Set the newly created buffer as the current one
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    // Send the vertex position data to the current buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions,
                 GL_STATIC_DRAW);
    //////////////////////////////////////////////////////////////////////////////
    // Vertex colors
    //
    // Task 3: Change these colors to something more fun.
    //////////////////////////////////////////////////////////////////////////////
    // Define the colors for each of the three vertices of the triangle
    const float colors[] = {
            //   R     G     B
            1.0f, 1.0f, 1.0f, // White
            1.0f, 1.0f, 1.0f, // White
            1.0f, 1.0f, 1.0f  // White
    };
    // Create a handle for the vertex color buffer
    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    // Set the newly created buffer as the current one
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    // Send the vertex color data to the current buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

    //////////////////////////////////////////////////////////////////////////////
    // Create a vertex array object and connect the vertex buffer objects to it
    //
    // See OpenGL Spec §2.10
    // - http://www.cse.chalmers.se/edu/course/TDA361/glspec30.20080923.pdf#page=64
    //////////////////////////////////////////////////////////////////////////////
    glGenVertexArrays(1, &vertexArrayObject);
    // Bind the vertex array object
    // The following calls will affect this vertex array object.
    glBindVertexArray(vertexArrayObject);
    // Makes positionBuffer the current array buffer for subsequent calls.
    glBindBuffer(GL_ARRAY_BUFFER, positionBuffer);
    // Attaches positionBuffer to vertexArrayObject, in the 0th attribute location
    glVertexAttribPointer(0, 3, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/);
    // Makes colorBuffer the current array buffer for subsequent calls.
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    // Attaches colorBuffer to vertexArrayObject, in the 1st attribute location
    glVertexAttribPointer(1, 3, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/);
    glEnableVertexAttribArray(0); // Enable the vertex position attribute
    glEnableVertexAttribArray(1); // Enable the vertex color attribute

    //////////////////////////////////////////////////////////////////////////////
    // Task 4: Add two new triangles. First by creating another vertex array
    //		   object, and then by adding a triangle to an existing VAO.
    //////////////////////////////////////////////////////////////////////////////

    const float newPositions[] = {
            0.75f, 0.75f, 0.75f,
            0.8f, 0.5f, 1.f,
            0.5f, 0.5f, 0.5f
                        -0.75f, -0.75f, 0.75f,
            -0.8f, -0.5f, 1.f,
            -0.5f, -0.5f, 0.5f
    };

    const float newColors[] = {
            0.5f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 0.2f, 0.3f,
            0.1f, 0.5f, 0.5f,
            1, 0.4, 0.2
    };

    GLuint newPositionBuffer;
    glGenBuffers(1, &newPositionBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newPositionBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newPositions), newPositions,
                 GL_STATIC_DRAW);

    GLuint newColorBuffer;
    glGenBuffers(1, &newColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, newColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(newColors), newColors, GL_STATIC_DRAW);

    glGenVertexArrays(1, &newVertexArrayObject);
    glBindVertexArray(newVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, newPositionBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/);
    glBindBuffer(GL_ARRAY_BUFFER, newColorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, false /*normalized*/, 0 /*stride*/, 0 /*offset*/);
    glEnableVertexAttribArray(0); // Enable the vertex position attribute
    glEnableVertexAttribArray(1); // Enable the vertex color attribute

    ///////////////////////////////////////////////////////////////////////////
    // Create shaders
    ///////////////////////////////////////////////////////////////////////////
    shaderProgram = shading::loadShaderProgram("shaders/simple.vert", "shaders/simple.frag", false);
}


///////////////////////////////////////////////////////////////////////////////
/// This function will be called once per frame, so the code to set up
/// the scene for rendering should go here
///////////////////////////////////////////////////////////////////////////////
void display(void)
{
    glClearColor(g_clearColor[0], g_clearColor[1], g_clearColor[2], 1.0); // Set clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the color buffer and the z-buffer
    // Instead of glClear(GL_BUFFER) the call should be glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

    // We disable backface culling for this tutorial, otherwise care must be taken with the winding order
    // of the vertices. It is however a lot faster to enable culling when drawing large scenes.
    glDisable(GL_CULL_FACE);

    // Set the Shader Program to use
    glUseProgram(shaderProgram); // Set the shader program to use for this draw call

    // Task 5: Set the `triangleColor` uniform in the shader to `g_triangleColor`

    // Bind the vertex array object that contains all the vertex data.
    glBindVertexArray(vertexArrayObject);
    // Submit triangles from currently bound vertex array object.
    shading::setUniform(2, g_triangleColor);
    glDrawArrays(GL_TRIANGLES, 0, 3); // Render 1 triangle


    // Task 4: Render the second VAO
    shading::setUniform(2, glm::vec3(1, 1, 1));
    glBindVertexArray(newVertexArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Task 5: Set the `triangleColor` uniform to white

    glUseProgram(0); // "unsets" the current shader program. Not really necessary.
}


int main(void)
{
    GLFWwindow* window;


    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();
    ImGui::StyleColorsDark();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();
        display();
        gui();

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