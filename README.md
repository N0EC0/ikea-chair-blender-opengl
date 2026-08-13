# COMP 371 — Assignment 3

This project displays a textured 3D chair in OpenGL. The user can move, rotate, and scale the chair; move the camera; switch between perspective and orthographic projection; switch between wireframe and filled rendering; and show or hide the floor and walls.

Models are loaded with Assimp. The rendering pipeline uses vertex positions, texture coordinates, and one diffuse texture per mesh. Lighting, normal mapping, specular mapping, and skeletal animation are not implemented.

## Requirements

- CMake 3.20 or newer
- A C++17-compatible compiler
- OpenGL 3.3 support
- GLEW
- GLFW
- GLM
- Assimp

`stb_image` is included in `include/` and does not need to be downloaded separately.

Platform-specific development tools:

- macOS: Xcode Command Line Tools (Apple Clang)
- Windows: Visual Studio 2022 with the **Desktop development with C++** workload and CMake support
- VS Code: the **C/C++** and **CMake Tools** extensions

## Project structure

```text
A3/
├── CMakeLists.txt
├── README.md
├── dependencies/                 # Third-party headers and libraries
│   ├── include/
│   │   ├── GL/
│   │   ├── GLFW/
│   │   ├── assimp/
│   │   └── glm/
│   └── lib/
├── include/
│   ├── AppState.h
│   ├── mesh.h
│   ├── model.h
│   ├── shader.h
│   └── stb_image.h
├── resources/
│   ├── objects/
│   │   ├── backWall.obj
│   │   ├── backWall.mtl
│   │   ├── chair.obj
│   │   ├── chair.mtl
│   │   ├── floor.obj
│   │   ├── floor.mtl
│   │   ├── rightWall.obj
│   │   ├── rightWall.mtl
│   │   ├── chair_diffuse.jpg
│   │   ├── floor_diffuse.jpg
│   │   └── wall_diffuse.jpg
│   └── shaders/
│       ├── model_shader.fs
│       └── model_shader.vs
└── src/
    ├── main.cpp
    ├── mesh.cpp
    ├── model.cpp
    ├── shader.cpp
    └── stb_image.cpp
```

Do not place project-owned headers inside `dependencies/include`. CMake searches both `include/` and `dependencies/include/`.

## Preparing dependencies

The third-party headers and libraries must match the operating system, processor architecture, and compiler used for the project. Copy the complete GLEW, GLFW, GLM, and Assimp header directories into `dependencies/include/`.

### macOS library filenames

The current `CMakeLists.txt` expects:

```text
dependencies/lib/libGLEW.2.3.1.dylib
dependencies/lib/libglfw.3.4.dylib
dependencies/lib/libassimp.6.0.5.dylib
```

Use libraries built for the correct architecture:

- Apple Silicon: `arm64`
- Intel Mac: `x86_64`

If the downloaded files have different names or versions, update the corresponding paths in `CMakeLists.txt`.

### Windows library filenames

The current `CMakeLists.txt` expects:

```text
dependencies/lib/glew32.lib
dependencies/lib/glfw3.lib
dependencies/lib/assimp-vc145-mtd.lib
```

Use libraries built for the same architecture and Visual C++ toolchain selected by CMake. If a library is dynamically linked, place its matching DLL beside `A3.exe` before running the program.

## Configure, build, and run

Run the following commands from the project root, where `CMakeLists.txt` is located.

### macOS

```bash
cmake -S . -B build
cmake --build build
./build/A3
```

### Windows

From PowerShell or Developer PowerShell:

```powershell
cmake -S . -B build -A x64
cmake --build build --config Debug
.\build\Debug\A3.exe
```

For a Release build:

```powershell
cmake --build build --config Release
.\build\Release\A3.exe
```

The executable location can vary with the selected CMake generator. A single-configuration generator may produce `build/A3.exe`.

## Visual Studio Code

1. Install the **C/C++** and **CMake Tools** extensions.
2. Open the root `A3` directory.
3. Make sure the dependency files described above are present.
4. Run **CMake: Select a Kit** and select Apple Clang on macOS or an appropriate Visual Studio x64 kit on Windows.
5. Run **CMake: Configure**.
6. Run **CMake: Build**.
7. Select `A3` as the launch target and run it without debugging.

If the project was configured with the wrong compiler or architecture, run **CMake: Delete Cache and Reconfigure**.

## Visual Studio on Windows

1. Install Visual Studio 2022 with the **Desktop development with C++** workload and CMake support.
2. Select **Open a local folder** and open the root `A3` directory, or open `CMakeLists.txt` directly.
3. Confirm that the selected configuration matches the architecture of the libraries in `dependencies/lib/`.
4. Select `A3.exe` as the startup item.
5. Select **Build → Build All**.
6. Run with **Debug → Start Without Debugging** (`Ctrl+F5`).

## Runtime resources

The program loads models, textures, and shaders through paths relative to the working directory, for example:

```text
resources/objects/chair.obj
resources/shaders/model_shader.vs
```

Run the executable from the project root as shown above. CMake also copies `resources/` into the build directory when the project is configured. If the copied resources become stale, configure the project again:

```bash
cmake -S . -B build
```

## Controls

| Key | Action |
|---|---|
| `Esc` | Close the application |
| `1` | Enable wireframe rendering |
| `2` | Enable filled rendering |
| `3` | Show the floor and walls |
| `4` | Hide the floor and walls |
| Hold `O` | Use orthographic projection; releasing it restores perspective projection |
| `W` / `S` | Move the chair up/down |
| `A` / `D` | Move the chair left/right |
| `Q` / `E` | Rotate the chair |
| `R` / `F` | Increase/decrease the chair scale |
| Up/Down arrows | Move the camera forward/backward |
| Left/Right arrows | Move the camera left/right |

The application starts in wireframe mode with the environment hidden.

## Rendering design

- Assimp imports OBJ geometry and material data.
- Each vertex contains only a position and one set of texture coordinates.
- Each mesh uploads its vertices and indices to a VAO, VBO, and EBO, then retains only the OpenGL handles, diffuse texture ID, and index count.
- A model caches diffuse textures so that meshes using the same image do not load it more than once.
- The vertex shader receives one combined model-view-projection (`mvp`) matrix.
- The fragment shader samples one diffuse texture from `texture_diffuse1`.

## Troubleshooting

### A header cannot be found

Confirm that these paths exist:

```text
dependencies/include/GL/glew.h
dependencies/include/GLFW/glfw3.h
dependencies/include/glm/glm.hpp
dependencies/include/assimp/scene.h
include/AppState.h
include/mesh.h
include/model.h
include/shader.h
include/stb_image.h
```

### A library cannot be found

Check that the filenames in `dependencies/lib/` exactly match those referenced by `CMakeLists.txt`. The current build configuration links the libraries using direct paths rather than searching the operating system.

### Undefined symbols or incompatible architecture

Make sure GLEW, GLFW, Assimp, the selected compiler, and the CMake target all use the same architecture. On Windows, ensure the `.lib` files were built for the selected Visual C++ toolchain.

### A DLL is missing on Windows

Copy the matching runtime DLL for each dynamically linked dependency into the same directory as `A3.exe`.

### Shaders, models, or textures fail to load

Run the executable with the project root as the working directory. Confirm that the lowercase `resources/` directory and all referenced filenames are present.
