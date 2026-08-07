# COMP 371 — Assignment 3

//Description to do later
It uses CMake as its build system and supports macOS and Windows.

The third-party libraries are not distributed as part of the submitted project. Before configuring the project, download versions of GLFW, GLEW, and GLM that match your operating system, processor architecture, and compiler, then place their files in the `dependencies` directory described below.

## Requirements

The following tools are required:

- CMake 3.20 or newer
- A C++17-compatible compiler
- OpenGL 3.3 support
- GLEW 2.3.1 (or a compatible build with the filename expected by `CMakeLists.txt`)
- GLFW 3.4 (or a compatible build with the filename expected by `CMakeLists.txt`)
- GLM (header-only library)

Platform-specific development tools:

- macOS: Xcode Command Line Tools (Apple Clang)
- Windows: Visual Studio 2022 with the **Desktop development with C++** workload, including CMake tools for Windows
- VS Code on either platform: the **C/C++** and **CMake Tools** extensions

// To do later: Add a note about the `stb_image` library.
`stb_image` is included in the project's root `include` directory and does not need to be downloaded separately.

## Project structure
// to update later

The project must retain the following hierarchy:

```text
A2/
├── CMakeLists.txt
├── README.md
├── Team_Contribution_Report.pdf
├── SampleRun/
│   ├── SampleRun.pdf
│   ├── Camera_2026-08-04.gif
│   └── ...
├── dependencies/                 # Created and populated by you
│   ├── include/
│   │   ├── GL/
│   │   │   ├── glew.h
│   │   │   ├── glxew.h           # May be included with the GLEW package
│   │   │   └── wglew.h           # May be included with the GLEW package
│   │   ├── GLFW/
│   │   │   ├── glfw3.h
│   │   │   └── glfw3native.h
│   │   └── glm/
│   │       └── ...
│   └── lib/
│       └── platform-specific library files
├── include/                      # Project-owned headers
│   ├── AppState.h
│   ├── Shader.h
│   ├── Texture.h
│   └── stb_image.h
├── Resources/                    # Runtime assets; preserve capitalization
│   ├── cabin1.jpg
│   ├── cabin3.jpg
│   ├── shaderT.fs
│   ├── shaderT.fs
│   ├── shaderC.fs
│   └── shaderC.vs
└── src/
    ├── main.cpp
    ├── Shader.cpp
    ├── Texture.cpp
    └── stb_image.cpp
```

Do not place the project-owned headers (`AppState.h`, `Shader.h`, `Texture.h`, and `stb_image.h`) inside `dependencies/include`. CMake already searches both locations:

```cmake
"${CMAKE_SOURCE_DIR}/include"
"${CMAKE_SOURCE_DIR}/dependencies/include"
```

## Preparing the dependencies

Update the dependency directories from the project root (empty folders are provided):

Copy the downloaded library files so that the final paths match the hierarchy shown above. Copy the complete `GL`, `GLFW`, and `glm` header directories—not only individual headers—because those headers include other files from their respective packages.

The headers and compiled libraries must all target the same architecture. For example, do not mix ARM64 libraries with an x86-64 compiler or 32-bit Windows libraries with a 64-bit build.

### Required macOS library filenames

The current `CMakeLists.txt` expects:

```text
dependencies/lib/libGLEW.2.3.1.dylib
dependencies/lib/libglfw.3.4.dylib
```

Use macOS libraries built for the grader's processor architecture:

- Apple Silicon: ARM64 (`arm64`)
- Intel Mac: Intel 64-bit (`x86_64`)

If the downloaded libraries have different versioned filenames, either provide copies/symbolic links with the exact names above or update the two corresponding paths in `CMakeLists.txt`.

The required macOS OpenGL and window-system frameworks are supplied by macOS and linked by CMake; they do not belong in `dependencies/lib`.

### Required Windows library filenames

The current `CMakeLists.txt` expects:

```text
dependencies/lib/glew32.lib
dependencies/lib/glfw3.lib
```

Use libraries compiled for the same architecture and Visual C++ toolchain selected by CMake. A 64-bit Visual Studio build normally requires the 64-bit library files.

If `glew32.lib` is an import library for the dynamic version of GLEW, also copy its matching `glew32.dll` beside the generated `A2.exe` before running. `glfw3.lib` is expected to be the library corresponding to the GLFW headers placed in `dependencies/include/GLFW`.

Windows' `opengl32` system library is linked automatically by `CMakeLists.txt` and should not be copied into the project.

## Configure and build from a terminal

Run all commands from the root `A2` directory, where `CMakeLists.txt` is located.

### macOS

Configure the build:

```bash
cmake -S . -B build
```

Compile:

```bash
cmake --build build
```

Run from the project root:

```bash
./build/A2
```

For a multi-configuration generator, the executable may instead be located in a configuration subdirectory, such as `build/Debug/A2`.

### Windows

Open a PowerShell or Developer PowerShell terminal in the project root, then configure a 64-bit Visual Studio build:

```powershell
cmake -S . -B build -A x64
```

Compile the Debug configuration:

```powershell
cmake --build build --config Debug
```

Run from the project root:

```powershell
.\build\Debug\A2.exe
```

For a Release build, replace `Debug` with `Release` in both commands/paths:

```powershell
cmake --build build --config Release
.\build\Release\A2.exe
```

The exact executable location depends on the selected CMake generator. Single-configuration generators may produce `build/A2.exe` instead.

## Build and run with VS Code

These instructions apply to VS Code on both macOS and Windows.

1. Install the **C/C++** and **CMake Tools** extensions.
2. Open the root `A2` folder in VS Code. Do not open only `src` or `build`.
3. Ensure the dependency folders and files described above are present.
4. Open the Command Palette (`Ctrl+Shift+P` on Windows or `Command+Shift+P` on macOS).
5. Select **CMake: Select a Kit** and choose the appropriate compiler:
   - Apple Clang on macOS.
   - A Visual Studio x64 kit on Windows.
6. Select **CMake: Configure**.
7. Select **CMake: Build**, or use the default build task with `Ctrl+Shift+B` (`Command+Shift+B` on macOS).
8. Select `A2` as the active CMake launch target.
9. Use **CMake: Run Without Debugging** to launch the program.

If VS Code previously configured the project with the wrong compiler or architecture, delete the `build` directory or run **CMake: Delete Cache and Reconfigure**, then select the correct kit.

The repository's `.vscode/tasks.json` defines a CMake build task, but the CMake Tools extension must first configure the project and select a valid compiler kit.

## Build and run with Visual Studio on Windows

Visual Studio can open the project directly as a CMake project; no `.sln` file is required.

1. Install Visual Studio 2022 with the **Desktop development with C++** workload and CMake support.
2. Start Visual Studio.
3. Select **Open a local folder** and choose the root `A2` directory, or select **File → Open → CMake** and choose `CMakeLists.txt`.
4. Wait for Visual Studio to finish its CMake configuration.
5. Confirm that the selected configuration uses an x64 compiler when the dependency libraries are 64-bit.
6. Select `A2.exe` as the startup item from the target dropdown.
7. Build with **Build → Build All**.
8. Run with **Debug → Start Without Debugging** (`Ctrl+F5`).

If the CMake cache was created before the dependencies were installed, or with the wrong architecture, use **Project → Delete Cache and Reconfigure** and build again.

## Runtime resources and working directory

The program loads its shaders and images through relative paths such as:

```text
Resources/shader.vs
Resources/shader.fs
Resources/cabin1.jpg
Resources/cabin3.jpg
```

CMake copies the `Resources` directory into the build directory during configuration. Nevertheless, the safest manual launch method is to run the executable while the terminal's working directory is the project root, as shown in the commands above. If the program prints a shader or texture file-loading error, first verify the working directory and confirm that the `Resources` folder exists there or beside the configured runtime location.

If a resource is added or changed and its build-directory copy is stale, run CMake configuration again:

```bash
cmake -S . -B build
```

## Controls

| Key | Action |
|---|---|
| `Esc` | Close the application |
| `1` / `2` | Increase/decrease the blend between the two textures |
| `3` / `4` | Enable wireframe/filled rendering |
| `P` / `O` | Select perspective/orthographic projection |
| `W`, `A`, `S`, `D` | Move the pyramid |
| `Q` / `E` | Rotate the pyramid by 30 degrees |
| `R` / `F` | Increase/decrease the pyramid's depth scale |
| Arrow keys | Move the camera |

## Troubleshooting

### A header cannot be found

Confirm these paths exist:

```text
dependencies/include/GL/glew.h
dependencies/include/GLFW/glfw3.h
dependencies/include/glm/glm.hpp
include/Shader.h
include/Texture.h
include/AppState.h
include/stb_image.h
```

### A library cannot be found while configuring

Check that the filenames in `dependencies/lib` exactly match the platform-specific names referenced by `CMakeLists.txt`. CMake currently uses direct file paths rather than searching the operating system for installed packages.

### Undefined symbols or an incompatible architecture error

Ensure GLEW, GLFW, the selected compiler, and the CMake target all use the same architecture. On Windows, also ensure the `.lib` files were compiled for the Visual C++ toolchain rather than an incompatible compiler.

### Windows reports that `glew32.dll` is missing

Copy the matching `glew32.dll` from the downloaded GLEW binary package into the same directory as `A2.exe`, then run the program again.

### Shaders or textures fail to load

Launch the executable with the project root as the working directory and verify that the capitalization and filenames under `Resources` have not changed.

