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
    // the program ID
    unsigned int ID = 0;

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
    void setBool(const std::string& name, bool value) const;
    
    void setInt(const std::string& name, int value) const;
    
    void setFloat(const std::string& name, float value) const;
    
    // Set a 2D vector
    void setVec2(const std::string& name, const glm::vec2& value) const;
    void setVec2(const std::string& name, float x, float y) const;
    
    // Set a 3D vector
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    
    // Set a 4D vector
    void setVec4(const std::string& name, const glm::vec4& value) const;   
    void setVec4(const std::string& name, float x, float y, float z, float w) const;
    
    // Set a 2x2 matrix
    void setMat2(const std::string& name, const glm::mat2& mat) const;
	
    // Set a 3x3 matrix
    void setMat3(const std::string& name, const glm::mat3& mat) const;
	
    // Set a 4x4 matrix
    void setMat4(const std::string& name, const glm::mat4& mat) const;
private:
    // Helper function for checking shader and program compilation/link errors
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif