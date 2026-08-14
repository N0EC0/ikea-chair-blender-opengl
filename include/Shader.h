/*
	Reference: https://learnopengl.com/Getting-started/Shaders
   
    Move the file inside dependencies/include
    CMake will find the file among the other header files
*/

#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <string>

class Shader {
public:
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Destructor
    ~Shader();
    
    // Prevent accidental copying
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // use/activte the shader
    void use();
    
    // utility uniform functions
    void setInt(const std::string& name, int value) const;
	
    // Set a 4x4 matrix
    void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
    // the program ID
    unsigned int ID = 0;
    
    // Helper function for checking shader and program compilation/link errors
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif