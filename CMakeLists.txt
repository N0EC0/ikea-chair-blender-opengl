cmake_minimum_required(VERSION 3.20)

project(A3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(A3
    src/main.cpp
    src/Shader.cpp
    src/stb_image.cpp
)

target_include_directories(A3 PRIVATE
    # Contains GL/glew.h, GLFW/glfw3.h, Shader.h, stb_image.h, Texture.h
    "${CMAKE_SOURCE_DIR}/include" #for portability
    "${CMAKE_SOURCE_DIR}/dependencies/include" #for local repo
)

file(COPY ${CMAKE_SOURCE_DIR}/resources
     DESTINATION ${CMAKE_BINARY_DIR})

# macOS configurations -----------------------------------------------------------
if (APPLE)
    target_link_libraries(A3 PRIVATE
        "${CMAKE_SOURCE_DIR}/dependencies/lib/libGLEW.2.3.1.dylib"
        "${CMAKE_SOURCE_DIR}/dependencies/lib/libglfw.3.4.dylib"
        "${CMAKE_SOURCE_DIR}/dependencies/lib/libassimp.6.0.5.dylib"
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework CoreFoundation"
    )

    set_target_properties(A3 PROPERTIES
        BUILD_RPATH "${CMAKE_SOURCE_DIR}/dependencies/lib"
    )
endif()

# macOS configurations End--------------------------------------------------------

# Windows configurations ---------------------------------------------------------
if (WIN32)
    target_link_libraries(A3 PRIVATE
        "${CMAKE_SOURCE_DIR}/dependencies/lib/glew32.lib"
        "${CMAKE_SOURCE_DIR}/dependencies/lib/glfw3.lib"
        opengl32
    )
endif()
# Windows configurations End------------------------------------------------------
