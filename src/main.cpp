/*
    COMP371 2261 CX - Assignment 3 
    Team members:
    - Nerina An 40310293 
    - Noemie Corneillier 40284815 
    - Ryan Anthony Khireddine 40315218
    
    If changing the CMakeLists.txt file, to reconfigure the build:
    On Windows with Visula Studio:
    Project -> Delete Cache and Reconfigure
    
    On macOS:
    cmake -S . -B build  
    
 
    Build and run with VS on Windows:
	On first run: File -> Open CMake Project -> select the CMakeLists.txt file -> Build -> Run
    Latter run: Ctrl +F5

    Build and run with CMake on macOS:
    Cmake --build build && ./build/A3

	This project was made with the help of the following tutorials:
	https://learnopengl.com/Getting-started/Hello-Triangle
	https://learnopengl.com/Getting-started/Camera
    https://learnopengl.com/Model-Loading/Assimp
    https://learnopengl.com/Model-Loading/Mesh
    https://learnopengl.com/Model-Loading/Model
	https://learnopengl.com/Advanced-OpenGL/Anti-Aliasing
*/

// Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <shader.h>
#include <model.h>
#include "AppState.h"

#include <iostream>

// Function prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, AppState& state);

int main() {

    // Making accessible the variables from the struct
    AppState state;

	// Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

	// Set GLFW window hints for OpenGL version and profile
    //   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Telling GLFW to use OpenGL version 4.x for Windows
    //   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Telling GLFW to use OpenGL version 3.x for macOS
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// For macOS compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Antialisasing
	glfwWindowHint(GLFW_SAMPLES, 4);
	// z-buffer
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	// Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(state.framebufferWidth, state.framebufferHeight, "COMP371 - Assignment 3", NULL, NULL);

	// Check if the window was created successfully
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

	// Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Store a pointer to the state to retrieve in glfwSetFramebufferSizeCallback()
    glfwSetWindowUserPointer(window, &state);

	// Set the framebuffer size callback to handle window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    glEnable(GL_DEPTH_TEST); // z-buffer
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // To not render back of polygon
	glEnable(GL_MULTISAMPLE); // Enable antialiasing
    
    // inner scope so the shader destructor calls before glfwTerminate()
    {
        // Build and compile GLSL Shaders
        Shader shader("resources/shaders/model_shader.vs", "resources/shaders/model_shader.fs");

        // Load the 3D models
        Model chair("resources/objects/chair.obj");
        Model floor("resources/objects/floor.obj");
        Model backWall("resources/objects/backWall.obj");
        Model rightWall("resources/objects/rightWall.obj");
        
        // Activate the shader program
        shader.use();

        shader.setInt("texture_diffuse1", 0);
        
        // Render loop
        while (!glfwWindowShouldClose(window)) {
            
            // Calculate the delta time between frames to make camera 
            // movement and rotation smooth and frame-rate independent
            float currentFrame = glfwGetTime();
            state.deltaTime = currentFrame - state.lastFrame;
            state.lastFrame = currentFrame;

            // Input
            processInput(window, state);

            // Render
            // Clear the color buffer with a dark yellow background color
            glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
            // Clear the color and depth buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);           

            // Set the polygon mode to wireframe or fill based on the edges flag
            if (state.wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            // Initialize the matrices to their identity matrix
            glm::mat4 chairTransform = glm::mat4(1.0f);
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            // Initial adjustement: Translate, rotate and scale down the world
            model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
            model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.2f));
            
            // Translate, rotate and scale only the chair based from input
            chairTransform = glm::translate(chairTransform, glm::vec3(state.distanceX, state.distanceY, 0.0f));
            chairTransform = glm::rotate(chairTransform, glm::radians(state.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
            chairTransform = glm::scale(chairTransform, glm::vec3(state.scale));

            // Create a view matrix to simulate camera movement
            view = glm::lookAt(state.cameraPos, state.cameraPos + state.cameraFront, state.cameraUp);

            // Set the projection matrix based on the current window size and the selected projection type (perspective or orthogonal)
            float aspect = 1.0f;
            // Protect against division by zero when the window is minimized
            if (state.framebufferHeight > 0) {
                aspect =
                    static_cast<float>(state.framebufferWidth) /
                    static_cast<float>(state.framebufferHeight);
            }        
            if (state.perspective) {
                projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
            }
            else {
                projection = glm::ortho(-1.5f * aspect, 1.5f * aspect, -1.5f, 1.5f, 0.1f, 100.0f);
            }

            const glm::mat4 viewProjection = projection * view;
            shader.setMat4("mvp", viewProjection * model * chairTransform);

            chair.Draw();
            
            // Draw the environment based on input
            if (state.environment) {
                shader.setMat4("mvp", viewProjection * model);
                floor.Draw();
                backWall.Draw();
                rightWall.Draw();
            }

            // Swap buffers and poll IO events (keys pressed/released)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

	// Terminate GLFW, clearing any resources allocated by GLFW
    glfwTerminate();
    return 0;
}

// Function definitions
// Callback function to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    // Retrieving the state of framebuffer window size
    AppState* state = static_cast<AppState*>(glfwGetWindowUserPointer(window));
    if (state) {
        state->framebufferWidth = width;
        state->framebufferHeight = height;
    }
}

// Function to process input from the user
void processInput(GLFWwindow* window, AppState& state)
{
	// Check if the ESC key is pressed, and if so, set the window to close
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	// 1, 2 keys to switch between wireframe and filled modes
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) 
        state.wireframe = true;
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) 
        state.wireframe = false;

    // 3, 4 keys to toggle to see chair only or with environment
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) 
        state.environment = true;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) 
        state.environment = false;    

	// O key to switch between perspective and orthographic projections
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) 
        state.perspective = false;   
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_RELEASE) 
        state.perspective = true;

	// W, A, S, D keys to move the chair in the scene
    float movementSpeed = 3.5f * state.deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        state.distanceY += movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        state.distanceY -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        state.distanceX -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        state.distanceX += movementSpeed;

	// Q, E keys to rotate the object in the scene
    float rotationSpeed = 3.5f * state.deltaTime;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        state.rotation += 15.0f * rotationSpeed;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        state.rotation -= 15.0f * rotationSpeed;

	// R, F keys to scale the object in the scene
    float scaleSpeed = 0.6f * state.deltaTime;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
        state.scale += scaleSpeed;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        state.scale -= scaleSpeed;
        if (state.scale <= 0.05f)
            state.scale = 0.05f; // minimum treshold 
    }

	// Arrow keys to move the camera in the scene
    float cameraSpeed = 2.0f * state.deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        state.cameraPos += cameraSpeed * state.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        state.cameraPos -= cameraSpeed * state.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        state.cameraPos += glm::normalize(glm::cross(state.cameraFront, state.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        state.cameraPos -= glm::normalize(glm::cross(state.cameraFront, state.cameraUp)) * cameraSpeed;
}
