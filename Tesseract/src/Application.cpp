#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


// Main program or entry point of application
int main(void)
{
    GLFWwindow* window; // Initialize GLFW Window instance

    /* Initialize the GLFW library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Tesseract", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);


    // Initialize glew and check if it is GLEW_OK
    if (glewInit() != GLEW_OK) {
        std::cout << "GLEW Not Okay !!!" << std::endl;
    }

    {
        // vertex positions
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f ,
            -0.5f ,  0.5f,
        };

        // indices to draw triangles
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Generating Vertex Array
        VertexArray va;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Generating Index Buffer
        IndexBuffer ib(indices, 6);

        // Generating shader
        Shader shader("res/shader/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.2f, 0.8f, 0.3f, 1.0f);

        // rgb values from 0.0f to 1.0f 
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        // rgb increment values
        float increment_r = 0.03f;
        float increment_g = 0.05f;
        float increment_b = 0.07f;

        va.Unbind();
        vb.Bind();
        ib.Unbind();
		shader.Unbind();

        Renderer renderer;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, g, b, 1.0f);

            renderer.Draw(va, ib, shader);

            // Change color values in every loop
            if (r > 1.0f)
                increment_r = -0.03f;
            else if (r < 0.0f)
                increment_r = 0.03f;
            r += increment_r;

            if (g > 1.0f)
                increment_g = -0.05f;
            else if (g < 0.0f)
                increment_g = 0.05f;
            g += increment_g;

            if (b > 1.0f)
                increment_b = -0.07f;
            else if (b < 0.0f)
                increment_b = 0.07f;
            b += increment_b;


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }

    glfwTerminate();
    return 0;
}