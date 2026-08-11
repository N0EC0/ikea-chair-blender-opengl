#ifndef APP_STATE_H
#define APP_STATE_H

#include <glm/glm.hpp>

struct AppState
{
    // Framebuffer window dimension for accurate aspect ratio
    int framebufferWidth = 1000;
    int framebufferHeight = 1000;

    // camera position and orientation
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // flag to display different projections
    bool perspective = true;
    
    // flag to display wireframe of filled polygon
    bool wireframe = false;

    // stores the initial moving distance 
    float distanceX = 0.0f;
    float distanceY = 0.0f;

    // stores the initial rotation angle 
    float rotation = 0.0f;

    // stores the initial scaling factor 
    float depthScale = 1.0f;

    // time between current frame and last frame
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif