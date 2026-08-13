# Assignment 3 Demo Script

Estimated presentation time: 6–8 minutes, followed by questions.

The assignment gives 25% for properly importing the OBJ file and 25% for displaying and transforming the chair in OpenGL. The demo should therefore spend most of its time on those two parts. Texture loading is optional and can be presented briefly.

## Before the demo

Open these files in separate editor tabs so they can be shown quickly:

1. `resources/objects/chair.obj`
2. `src/model.cpp`
3. `include/mesh.h`
4. `src/mesh.cpp`
5. `resources/shaders/model_shader.vs`
6. `src/main.cpp`

Build the program before the meeting and start the demo from the project root:

```bash
cmake --build build
./build/A3
```

Start with the application in wireframe mode. This directly demonstrates the minimum rendering requirement in the assignment.

## 1. Introduction and assignment requirements

**Say:**

> Our project starts with a detailed chair created in Blender. The main OpenGL requirement was to export the chair as an OBJ file, import its geometry into C++, render it, and apply interactive translation, rotation, and scaling. We use GLFW to create the window and read keyboard input, GLEW to access OpenGL functions, GLM for matrices, and Assimp to parse the OBJ file.
>
> The assignment only requires a wireframe in OpenGL. We implemented that requirement and also added optional diffuse textures and a simple floor-and-wall environment. We did not add lighting, normal mapping, or specular mapping because they are not required for the OpenGL portion.

**Show:** the running application in its initial wireframe state.

## 2. From Blender to OBJ

**Show:** `resources/objects/chair.obj`.

Point to examples of lines beginning with `v`, `vt`, and `f`.

**Say:**

> We exported the completed Blender chair as an OBJ model. An OBJ file is a text representation of the model. Lines beginning with `v` contain vertex positions, `vt` lines contain UV texture coordinates, and `f` lines describe faces using indices into those arrays. The OBJ also references an MTL material file, which contains the optional diffuse texture path.
>
> We do not manually parse this text. We use Assimp because it supports model formats and converts the file into a consistent scene structure containing nodes, meshes, vertices, faces, and materials.

If asked about normals, add:

> The OBJ contains normal data from Blender, but our OpenGL shader has no lighting calculations, so normals are not copied into our simplified vertex structure.

## 3. Importing the OBJ with Assimp

**Show:** `Model::loadModel()` in `src/model.cpp`.

**Say:**

> Constructing the chair model calls `loadModel()` with `resources/objects/chair.obj`. Inside this function, an `Assimp::Importer` reads the file.

Point to:

```cpp
importer.ReadFile(
    path,
    aiProcess_Triangulate | aiProcess_FlipUVs
);
```

**Continue:**

> `aiProcess_Triangulate` converts imported faces to triangles because our OpenGL draw call uses `GL_TRIANGLES`. The current code also requests `aiProcess_FlipUVs`, which reverses the vertical texture coordinate. That flag is related only to optional texture orientation, not to importing or rendering the chair's geometry.
>
> We then validate the returned `aiScene`. If the scene is missing, incomplete, or has no root node, we report the Assimp error and stop loading that model. Otherwise, we save the model directory so relative material and texture paths can be resolved.

**Show:** `Model::processNode()`.

**Say:**

> An Assimp scene is hierarchical. Starting at the root node, `processNode()` processes every mesh referenced by that node and then recursively visits all child nodes. This ensures that all parts of the chair are imported, not only the first mesh. The chair was exported as OBJ, where the geometry positions are already suitable for this project, so we do not need to apply separate node transformations during import.

## 4. Extracting vertex and index data

**Show:** `Model::processMesh()` in `src/model.cpp`, then the `Vertex` structure in `include/mesh.h`.

**Say:**

> For each Assimp mesh, we create temporary CPU-side vectors for vertices and indices. Our `Vertex` contains only a three-component position and a two-component texture coordinate.

Point to:

```cpp
struct Vertex {
    glm::vec3 Position;
    glm::vec2 TexCoords;
};
```

**Continue:**

> We loop through `mesh->mVertices` and convert each Assimp position into a GLM vector. If the mesh has a first UV channel, we copy its x and y components. If it has no UV coordinates, we use zero so the vertex is still fully initialized.
>
> Next, we visit every Assimp face and copy its indices. Because triangulation was requested during import, each face describes a triangle. Indices let OpenGL reuse vertices instead of storing a separate complete vertex for every triangle corner.
>
> Finally, `processMesh()` constructs one of our `Mesh` objects with the vertex array, index array, and optional diffuse texture ID.

## 5. Uploading the mesh to OpenGL

**Show:** `Mesh::setupMesh()` in `src/mesh.cpp`.

**Say:**

> At this point the data is still in normal C++ vectors in CPU memory. `setupMesh()` creates three OpenGL objects: a VAO, a VBO, and an EBO.

- **VAO:** remembers the vertex input configuration and the associated element buffer.
- **VBO:** stores the actual vertex attributes—positions followed by texture coordinates for each vertex.
- **EBO:** stores the indices that define the triangles.

Point to the two `glBufferData` calls.

**Continue:**

> `glBufferData` transfers the temporary vectors into GPU-managed buffer storage. The data is marked `GL_STATIC_DRAW` because the chair geometry does not change after loading. We transform the chair with a matrix; we do not rewrite its vertices every frame.

Point to the attribute declarations:

```cpp
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                      sizeof(Vertex), nullptr);

glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
                      sizeof(Vertex),
                      (void*)offsetof(Vertex, TexCoords));
```

**Continue:**

> Attribute location zero reads three floats for the position. Attribute location one reads two floats for the UV coordinates. The stride is `sizeof(Vertex)`, so OpenGL advances by one complete vertex each time. `offsetof` gives the byte position of `TexCoords` inside the structure instead of assuming a hard-coded offset.
>
> After the upload, the mesh does not retain the CPU vertex and index vectors. It only stores the VAO, VBO, EBO, texture ID, and number of indices. This keeps the runtime representation small.

## 6. Rendering the imported vertices

**Show:** `Mesh::Draw()` in `src/mesh.cpp`.

**Say:**

> To render a mesh, we bind its VAO and call `glDrawElements`.

Point to:

```cpp
glDrawElements(
    GL_TRIANGLES,
    indexCount,
    GL_UNSIGNED_INT,
    nullptr
);
```

**Continue:**

> `GL_TRIANGLES` tells OpenGL to interpret each group of three indices as a triangle. `indexCount` tells it how many indices to read from the EBO, and `GL_UNSIGNED_INT` matches the C++ index type. The null final argument means reading starts at offset zero in the currently bound EBO.
>
> OpenGL retrieves each referenced vertex from the VBO using the layout stored in the VAO. The vertex shader then runs once for each processed vertex.

**Show:** `resources/shaders/model_shader.vs`.

**Say:**

> The shader locations match the VAO: location zero is the position and location one is the UV. The vertex position is multiplied by the combined model-view-projection matrix:

```glsl
gl_Position = mvp * vec4(aPos, 1.0);
```

> This converts the Blender model-space position into clip space. OpenGL then assembles the vertices into triangles, clips them, performs perspective division and viewport mapping, and rasterizes the visible primitives. Depth testing makes closer surfaces hide farther surfaces. In wireframe mode, `glPolygonMode` rasterizes triangle edges instead of filled interiors.

## 7. Translation, rotation, and scaling

**Show:** the transformation section of `src/main.cpp`.

**Say:**

> Keyboard input updates translation, rotation, and scale values stored in `AppState`. We multiply movement speeds by delta time so their rate is not tied to the frame rate.
>
> Every frame, GLM builds the chair transformation from those values. The effective order applied to a vertex is scale first, rotation second, and translation last, because matrix operations act on the vertex from right to left.

Point to:

```cpp
chairTransform = glm::translate(chairTransform, ...);
chairTransform = glm::rotate(chairTransform, ...);
chairTransform = glm::scale(chairTransform, ...);
```

**Continue:**

> This order is important for the requirement that the chair rotate while staying in place. Rotation occurs around the chair's local origin before its translation is applied. If translation were rotated as well, the chair could orbit around the world origin instead of spinning in place.
>
> We then create the final matrix as projection times view times model times the chair transformation and send it to the vertex shader as the `mvp` uniform. The original GPU vertex data stays unchanged; only the transformation matrix changes.

Point to:

```cpp
const glm::mat4 viewProjection = projection * view;
shader.setMat4("mvp", viewProjection * model * chairTransform);
```

## 8. Live transformation demonstration

Perform these actions slowly enough for the TA to see each requirement:

1. Press `2` to show the filled, optionally textured chair.
2. Press `1` to return to the required wireframe rendering.
3. Hold `A` and `D`, then `W` and `S`, to demonstrate translation.
4. Hold `Q` or `E` to rotate the chair. Point out that it spins around itself and remains at its translated location.
5. Hold `R` and `F` to demonstrate uniform scaling.
6. Hold `O` briefly to show orthographic projection, then release it to return to perspective.
7. Use the arrow keys to demonstrate that camera movement is separate from object transformation.
8. Press `3` to show the optional environment and `4` to hide it.

**Say while demonstrating:**

> These controls update state variables only. The chair's geometry remains in the same VBO. On each frame, we rebuild and upload a new MVP matrix, so transformations are handled efficiently by the vertex shader.

## 9. Brief optional texture explanation

**Show briefly:** `Model::loadDiffuseTexture()` and `TextureFromFile()` in `src/model.cpp`.

**Say:**

> Texture rendering is optional for the OpenGL portion, so we kept it simple. Assimp reads the first diffuse texture path from the OBJ's MTL material. `stb_image` loads the image pixels, and `glTexImage2D` uploads them to an OpenGL 2D texture. The texture is bound to texture unit zero when the mesh is drawn, and the fragment shader samples it using the UV coordinates interpolated from the vertices.
>
> The model also caches texture paths, so multiple meshes that use the same diffuse image reuse one OpenGL texture instead of loading it repeatedly. In wireframe mode the same geometry is drawn, but polygon mode limits rasterization to the triangle edges.

## 10. Closing statement

**Say:**

> In summary, Blender provides the chair geometry in OBJ format, Assimp converts it into meshes, our model code extracts positions and triangle indices, and our mesh code uploads that data into OpenGL buffers. The vertex shader transforms every imported position with the MVP matrix, and `glDrawElements` renders the indexed triangles. Keyboard input changes the chair's translation, rotation, and scale without modifying the original model data. This directly addresses the two graded OpenGL requirements: proper OBJ importing and correct display and transformation of the chair.

# Potential TA Questions and Suggested Answers

Every team member should understand these answers because the assignment states that the TA may direct a question to a specific student.

## OBJ and Assimp

### 1. Why did you use Assimp instead of parsing the OBJ manually?

Assimp handles OBJ syntax, separate indices, multiple meshes, scene nodes, materials, and error checking. It converts the file into a consistent `aiScene` representation, allowing our code to focus on copying renderable data into OpenGL buffers.

### 2. What information do `v`, `vt`, and `f` represent in an OBJ file?

- `v`: a 3D vertex position.
- `vt`: a 2D texture coordinate.
- `f`: a face described using indices that refer to positions, UVs, and possibly normals.

### 3. What is an `aiScene`?

It is Assimp's in-memory representation of the imported file. It contains the root node, meshes, materials, textures, and other imported data.

### 4. Why do you recursively process Assimp nodes?

A model can contain meshes on the root node and on child nodes. Recursion visits the complete hierarchy so no mesh is skipped.

### 5. What does `aiProcess_Triangulate` do?

It converts non-triangular faces into triangles. This matches our `GL_TRIANGLES` draw mode and ensures the index stream can be rendered consistently.

### 6. Why do you use `aiProcess_FlipUVs`?

It flips the vertical UV coordinate to account for different image and graphics coordinate conventions. However, the current program also flips images through stb_image, so the two corrections are redundant. We should keep only one; the required wireframe geometry is unaffected either way.

### 7. Why are normals not stored?

Normals are used mainly for lighting calculations. This program has no lighting and the assignment only requires wireframe rendering, so positions are sufficient for the required geometry. UVs are retained only for the optional diffuse texture.

### 8. Why does `processMesh()` collect indices as well as vertices?

Indices allow multiple triangles to reuse the same vertex data. They reduce duplication and are required by `glDrawElements`.

## OpenGL buffers and drawing

### 9. What are the VAO, VBO, and EBO?

- The VBO stores vertex attribute data.
- The EBO stores triangle indices.
- The VAO remembers how vertex attributes are laid out and which EBO is associated with the mesh.

### 10. What does `glVertexAttribPointer` describe?

It tells OpenGL how to interpret bytes in the bound VBO: the attribute location, number and type of components, stride between vertices, and byte offset of the attribute.

### 11. Why is the stride `sizeof(Vertex)`?

The buffer contains interleaved `Vertex` structures. OpenGL must skip one complete structure to reach the same attribute in the next vertex.

### 12. Why use `offsetof(Vertex, TexCoords)`?

It safely calculates the texture-coordinate byte offset according to the compiler's actual structure layout instead of relying on a hard-coded number.

### 13. What does `GL_STATIC_DRAW` mean?

It is a usage hint stating that the buffer data is uploaded once and used for many draw calls. The chair geometry is static; only its transformation matrix changes.

### 14. What exactly happens during `glDrawElements`?

OpenGL reads indices from the bound EBO, uses them to fetch vertices from the VBO according to the VAO layout, runs the vertex shader, assembles triangles, clips and rasterizes them, performs fragment and depth processing, and writes visible results to the framebuffer.

### 15. Why use `glDrawElements` rather than `glDrawArrays`?

The imported model already provides indexed faces. `glDrawElements` uses those indices and permits vertex reuse, whereas `glDrawArrays` would require the vertex data to be expanded into drawing order.

### 16. How is wireframe mode implemented?

The program calls:

```cpp
glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
```

Filled mode uses `GL_FILL`. The underlying triangle geometry and draw call remain the same.

### 17. Why is depth testing enabled?

Depth testing compares fragment depths so surfaces closer to the camera correctly hide surfaces behind them. Without it, triangles could appear in draw-call order rather than spatial order.

## Transformations and shaders

### 18. What are model, view, and projection transformations?

- The model transformation positions, rotates, and scales the object in the world.
- The view transformation represents the camera.
- The projection transformation maps the camera view into clip space using perspective or orthographic projection.

The project multiplies them into one MVP matrix before sending it to the vertex shader.

### 19. Why does matrix multiplication order matter?

Matrix multiplication is not commutative. With column vectors, the rightmost operation affects the vertex first. Our chair transformation applies scale, then rotation, then translation, which lets it rotate about its local origin before being placed at its translated position.

### 20. How do you ensure the chair rotates in place?

The rotation is applied before translation in the effective vertex-operation order. Therefore, rotation changes the chair around its own model origin, while the translation afterward determines where that rotated chair appears.

### 21. Are vertices changed on the CPU every time the chair moves?

No. The original vertices remain unchanged in the VBO. Input changes transformation values, C++ rebuilds the MVP matrix, and the vertex shader applies that matrix to each vertex during rendering.

### 22. Why use delta time for controls?

Delta time measures the elapsed time between frames. Multiplying speeds by it makes motion, rotation, and scaling approximately independent of frame rate.

### 23. What is the difference between moving the chair and moving the camera?

Chair movement changes the chair's model transformation. Camera movement changes the view matrix through `glm::lookAt`. Both affect the final screen position, but they represent different coordinate transformations.

### 24. What does the vertex shader output?

It outputs `gl_Position`, the transformed vertex position in clip space, and passes the UV coordinate to the fragment shader for optional texture sampling.

## Optional textures and project design

### 25. How is the diffuse texture connected to the OBJ model?

The OBJ references an MTL file, and the MTL's `map_Kd` entry identifies the diffuse image. Assimp exposes that path through the mesh material.

### 26. Why cache textures?

The chair contains multiple meshes that may reference the same image. The cache prevents duplicate image loading and duplicate OpenGL texture creation within one model.

### 27. Why is there only one texture per mesh?

The optional shader uses a single diffuse sampler and the project intentionally excludes lighting, normal, specular, and height maps. The loader therefore uses only the first diffuse texture.

### 28. Why are Mesh copies disabled but moves allowed?

A mesh owns OpenGL buffer and vertex-array handles. Copying those numeric handles would create multiple owners and risk deleting the same OpenGL object more than once. Move operations transfer ownership safely and allow meshes to be stored in `std::vector`.

### 29. When are OpenGL resources released?

Each `Mesh` destructor deletes its EBO, VBO, and VAO. Each `Model` destructor deletes the cached textures. The objects are scoped so these destructors run before `glfwTerminate()`, while the OpenGL context is still valid.

### 30. What would you need to add for lighting?

We would retain vertex normals, add light and material uniforms, transform normals correctly—usually with a normal matrix—and calculate ambient, diffuse, and possibly specular lighting in the shaders. None of that is required for this assignment's OpenGL wireframe result.

# Quick Reference: Code Path

```text
Blender chair
    ↓ export
chair.obj + chair.mtl
    ↓ Assimp::Importer::ReadFile
aiScene
    ↓ processNode
aiMesh objects
    ↓ processMesh
Vertex positions + UVs + triangle indices
    ↓ Mesh::setupMesh
VBO + EBO + VAO
    ↓ Mesh::Draw
glDrawElements(GL_TRIANGLES, ...)
    ↓ vertex shader
mvp × model-space position
    ↓ rasterization
wireframe edges or filled fragments
    ↓ optional fragment shader step
diffuse texture color
```
