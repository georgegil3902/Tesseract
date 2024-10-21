#include <glad/glad.h>
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
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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


    // Initialize GLAD and check if initialised succesfully
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // vertex positions
        float positions[] = {
            0.0f, 0.0f, 0.0f, 0.0f,   // 0
            1280.0f, 0.0f, 1.0f, 0.0f,   // 1
            1280.0f, 677.0f, 1.0f, 1.0f,   // 2
            0.0f, 677.0f, 0.0f, 1.0f    // 3
        };

        // indices to draw triangles
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        // Generating Vertex Array
        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Generating Index Buffer
        IndexBuffer ib(indices, 6);

        glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 677.0f, -1.0f, 1.0f);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -300, 0));

        glm::mat4 mvp = proj * view * model;

        // Generating shader
        Shader shader("res/shader/Basic.shader");
        shader.Bind();

        shader.SetUniform4f("u_Color", 0.2f, 0.8f, 0.3f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        // rgb values from 0.0f to 1.0f 
        float r = 0.0f;
        float g = 0.0f;
        float b = 0.0f;
        // rgb increment values
        float increment_r = 0.03f;
        float increment_g = 0.05f;
        float increment_b = 0.07f;

		int h = 0;
		int h_i = 1;
		int v = 0;
		int v_i = 1;

        Texture texture("res/images/image.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        va.Unbind();
        vb.Bind();
        ib.Unbind();
		shader.Unbind();

        Renderer renderer;

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		//ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

		// Our state
		bool show_demo_window = true;
		bool show_another_window = false;
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

            shader.Bind();
            shader.SetUniform4f("u_Color", r, g, b, 1.0f);

            if (h > 300)
                h_i = -1;
            else if (h < -300)
                h_i = 1;
			if (v > 300)
				v_i = -1;
			else if (v < -300)
				v_i = 1;
			h += h_i;
			v += v_i;
			model = glm::translate(glm::mat4(1.0f), glm::vec3(v, h, 0));
			mvp = proj * view * model;

			shader.SetUniformMat4f("u_MVP", mvp);

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


			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
				ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
				ImGui::Checkbox("Another Window", &show_another_window);

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}


            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}