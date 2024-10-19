#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


// struct to store or return multiple shader code strings
struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

// Read shader code from external file and parse it into string
static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                // Set Mode to vertex
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                // Set Mode to fragment
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(), ss[1].str() };
}

// Compile shader code string in the gpu
static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ?  "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }
    return id;
}

// Create the vertex shader and fragment shader and link to GLProgram
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
{
    unsigned int program = glCreateProgram();   // Create a OpenGL program
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);    // Attach vertex shader to program
    glAttachShader(program, fs);    // Attach fragment shader to program
    glLinkProgram(program);         // link program to OpenGL
    glValidateProgram(program);     // Validate the linked program
    
    glDeleteShader(vs);     // Delete Shader later
    glDeleteShader(fs);     // Delete Shader later

    return program;
}

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
        ShaderProgramSource source = ParseShader("res/shader/Basic.shader");    // read the Basic.shader file into a string and save it into source[ShaderProgramSource struct]
        unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource); // create shader program using the vertex and fragment shader
        GLCall(glUseProgram(shader));   // Tell OpenGL to use our shader program

        // Retrieve location of the uniform within from shader program
        int location = glGetUniformLocation(shader, "u_Color");
        ASSERT(location != -1); // Make sure that location is retrieved
        glUniform4f(location, 0.2f, 0.8f, 0.3f, 1.0f);  // set desired value to uniform as an initialization

        // rgb values from 0.0f to 1.0f 
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        // rgb increment values
        float increment_r = 0.03f;
        float increment_g = 0.05f;
        float increment_b = 0.07f;

        GLCall(glBindVertexArray(0));   // Unbind program
        GLCall(glUseProgram(0));   // Unbind program
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));  // Unbind ARRAY_BUFFER
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); // Unbind ELEMENT_ARRAY_BUFFER
        

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);   // Clear the screen

            GLCall(glUseProgram(shader));   // Unbind program
            glUniform4f(location, r, g, b, 1.0f);   // Set the uniform values with r, g, b using uniform location

            // GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));  // Unbind ARRAY_BUFFER
            // GLCall(glEnableVertexAttribArray(0));   // Exact usage forgot... do check it up online later
            // GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));  // Check it up online later

            va.Bind();
            ib.Bind();

            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));  // Draw call that uses the binded buffers to draw triangles

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

        glDeleteProgram(shader);
    }

    glfwTerminate();
    return 0;
}