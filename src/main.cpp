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
	https://learnopengl.com/Getting-started/Textures
	https://learnopengl.com/Getting-started/Camera
*/

// Libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Shader.h>
#include <stb_image.h>
#include "Texture.h"
#include "AppState.h"
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
     glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); // Telling GLFW to use OpenGL version 4.x for Windows
     glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Telling GLFW to use OpenGL version 3.x for macOS
 //   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// For macOS compatibility
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

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

    // configure global opengl state
    glEnable(GL_DEPTH_TEST); // z-buffer
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK); // To not render back of polygon

    // Inner Scope because texture objects must be destroyed before glfwTerminate
    {
        // Build and compile GLSL Shaders
        // Colour
        // Shader shader("Resources/shaderC.vs", "Resources/shaderC.fs");
        // Textures
        Shader shader("Resources/shaderT.vs", "Resources/shaderT.fs");

        // Initialize vertex data and configure vertex attributes
        float pyramid[] = {
            // positions           // texture coords attributes
             0.0f,  0.5f,  0.0f,   0.5f, 1.0f, // 0 top
            -0.9f, -0.5f, -0.9f,   0.0f, 0.0f, // 1 bottom left - back
             0.0f, -0.5f,  0.9f,   1.0f, 0.0f, // 2 bottom front
             0.9f, -0.5f, -0.9f,   0.0f, 0.0f, // 3 bottom right - back
        };

        // For just the coloured pyramid use this one
        // float pyramid[] = {
        //     // positions
        //      0.0f,  0.5f,  0.0f,  // 0 top
        //     -0.9f, -0.5f, -0.9f,  // 1 bottom left - back
        //      0.0f, -0.5f,  0.9f,  // 2 bottom front
        //      0.9f, -0.5f, -0.9f   // 3 bottom right - back
        // };

        unsigned int indices[] = {
            0, 1, 2, // Normal of face 1
            2, 3, 0, // Normal of face 2
            0, 3, 1, // Normal of face 3
            1, 3, 2  // Normal of face 4
        };

        // Initialize the Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO)
        unsigned int VAO, VBO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Bind the VAO
        glBindVertexArray(VAO);

        // Bind and set the vertex buffer data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(pyramid), pyramid, GL_STATIC_DRAW);

        // Bind and set the element buffer data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        // Configure the vertex attributes
        // Position attribute for the colour pyramid 
        // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        // glEnableVertexAttribArray(0);
        
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // Texture coordinate attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        
        // tell stb_image.h to flip loaded texture's on the y-axis.
        stbi_set_flip_vertically_on_load(true);
        // Load and create textures
        Texture texture1("Resources/cabin1.jpg");
        Texture texture2("Resources/cabin3.jpg");

        // Activate the shader program
        shader.use();

        // Configure shader sampler uniforms
        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);
        
        // Render loop
        while (!glfwWindowShouldClose(window)) {
            
            // Calculate the delta time between frames to make camera 
            // movement and rotation smooth and frame-rate independent
            float currentFrame = glfwGetTime();
            state.deltaTime = currentFrame - state.lastFrame;
            state.lastFrame = currentFrame;

            // Rotation control for every key press/release will be exactly 30 degrees
            float rotationStep = state.rotationSpeed * state.deltaTime;
            if (state.rotation < state.targetRotation) {
                state.rotation += rotationStep;
                if (state.rotation > state.targetRotation)
                    state.rotation = state.targetRotation;           
            }
            else if (state.rotation > state.targetRotation) {
                state.rotation -= rotationStep;
                if (state.rotation < state.targetRotation)
                    state.rotation = state.targetRotation;           
            }

            // Input
            processInput(window, state);

            // Render
            // Clear the color buffer with a dark yellow background color
            glClearColor(0.1f, 0.1f, 0.0f, 1.0f);
            // Clear the color and depth buffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Bind the textures
            texture1.bind(0);
            texture2.bind(1);
            
            // Activate the shader program
            shader.use();

            // Set the texture mix uniform value
            shader.setFloat("mixValue", state.mixValue);

            // Set the polygon mode to wireframe or fill based on the edges flag
            if (state.wireframe) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
            else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }

            // Initialize the matrices to their identity matrix
            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            // Rotate and scale down the world scene by 2 degrees on x axis to see the 3D effect of the pyramid
            model = glm::rotate(model, glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
            // Translate, rotate and scale the model based from input
            model = glm::translate(model, glm::vec3(state.distanceX, state.distanceY, 0.0f));
            model = glm::rotate(model, glm::radians(state.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(1.0f, 1.0f, state.depthScale));

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
                model = glm::rotate(model, glm::radians(10.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            }

            // Retrieve and pass the uniform locations for the transformation, view, and projection matrices
            // Using the function from Shader.h
            shader.setMat4("model", model);
            shader.setMat4("view", view);
            shader.setMat4("projection", projection);
            
            // Draw the objects
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

            // Swap buffers and poll IO events (keys pressed/released)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // De-allocate all resources once they've outlived their purpose
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
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

	// 1, 2 keys to adjust the mix value of the two textures
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        state.mixValue += 0.03f;
        if (state.mixValue >= 1.0f)
            state.mixValue = 1.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        state.mixValue -= 0.03f;
        if (state.mixValue <= 0.0f)
            state.mixValue = 0.0f;
    }

	// 3, 4 keys to switch between wireframe and fill modes
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) 
        state.wireframe = true;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) 
        state.wireframe = false;

	// P and O keys to switch between perspective and orthogonal projections
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) 
        state.perspective = true;   
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) 
        state.perspective = false;

	// W, A, S, D keys to move the object in the scene
    float movementSpeed = 1.5f * state.deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        state.distanceY += movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        state.distanceY -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        state.distanceX -= movementSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        state.distanceX += movementSpeed;

	// Q, E keys to rotate the object in the scene
    bool qIsPressed = glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS;
    bool eIsPressed = glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS;
    // Only change the target when Q transitions from released to pressed
    if (qIsPressed && !state.qWasPressed) 
        state.targetRotation += 30.0f;
    // Only change the target when E transitions from released to pressed
    if (eIsPressed && !state.eWasPressed) 
        state.targetRotation -= 30.0f;
    state.qWasPressed = qIsPressed;
    state.eWasPressed = eIsPressed;

	// R, F keys to scale the object in the scene
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
        state.depthScale += 0.05f;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        state.depthScale -= 0.05f;
        if (state.depthScale <= 0.5f)
            state.depthScale = 0.5f; // So it does not flatten in 2D
    }
	// Arrow keys to move the camera in the scene
    float cameraSpeed = 2.5f * state.deltaTime;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        state.cameraPos += cameraSpeed * state.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        state.cameraPos -= cameraSpeed * state.cameraFront;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        state.cameraPos -= glm::normalize(glm::cross(state.cameraFront, state.cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        state.cameraPos += glm::normalize(glm::cross(state.cameraFront, state.cameraUp)) * cameraSpeed;
}

