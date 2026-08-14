# COMP 371 — Assignment 3 Contribution Report

**Term:** Summer 2026  
**Project:** Blender Chair Model and OpenGL OBJ Rendering  

## Team Members

| Team member | Student ID |
|---|---:|
| Nerina An | 40310293 |
| Noemie Corneillier | 40284815 |
| Ryan Anthony Khireddine | 40315218 |

## Project Summary

The objective of this project was to create a detailed chair model in Blender, export it in OBJ format, import it into a C++ OpenGL application, and demonstrate interactive translation, rotation, and scaling. The program uses GLFW for window creation and keyboard input, GLEW for access to OpenGL functions, GLM for matrix transformations, and Assimp for importing OBJ geometry and material data. The required chair can be displayed in wireframe mode, and the project also includes optional diffuse textures and a simple floor-and-wall environment.

## Individual Contributions

### Nerina An — 40310293

Nerina focused primarily on creating and preparing the Blender model and its visual assets. Her contributions included:

- Studying the reference chair and identifying its main proportions and components.
- Creating and refining the chair geometry in Blender, including the seat, back, legs, and supporting pieces.
- Improving the accuracy and level of detail of the model relative to the reference images.
- Organizing the chair into appropriate Blender objects and preparing it for export.
- UV unwrapping the chair and checking the placement of the texture coordinates.
- Creating or applying the chair material and diffuse texture.
- Helping prepare the Blender scene used to present the completed chair model.
- Capturing screenshots of the modeling process for the Blender documentation.
- Verifying that the exported OBJ and MTL files contained the expected geometry and material references.
- Participating in final visual testing and demo preparation.

### Noemie Corneillier — 40284815

Noemie focused primarily on the OpenGL application structure, model importing, and GPU rendering pipeline. Her contributions included:

- Setting up and maintaining the CMake-based C++ project structure.
- Integrating GLFW, GLEW, GLM, Assimp, and stb_image into the application.
- Implementing the Assimp model-loading pipeline for OBJ files.
- Processing the Assimp scene hierarchy recursively so that all chair meshes are imported.
- Extracting vertex positions, texture coordinates, faces, and indices from each Assimp mesh.
- Implementing the `Model` and `Mesh` classes used to organize imported geometry.
- Creating and configuring the VAO, VBO, and EBO for each mesh.
- Defining the OpenGL vertex attribute layout and rendering imported geometry with `glDrawElements`.
- Simplifying the mesh representation so that CPU vertex and index arrays are released after being uploaded to OpenGL.
- Managing the lifetime of OpenGL buffers, vertex arrays, textures, and shader programs.
- Debugging model-loading and rendering issues and verifying that the project built successfully.
- Updating the project README and preparing the technical demo explanation.

### Ryan Anthony Khireddine — 40315218

Ryan focused primarily on transformations, user interaction, shaders, and optional scene features. His contributions included:

- Implementing keyboard controls for chair translation, rotation, and scaling.
- Using delta time to keep transformations consistent across different frame rates.
- Building the chair's transformation matrix with GLM.
- Ensuring that the chair rotates around its own model origin and remains at its translated position.
- Implementing the view matrix and keyboard-controlled camera movement.
- Implementing perspective and orthographic projection modes with the correct framebuffer aspect ratio.
- Implementing wireframe and filled rendering modes with `glPolygonMode`.
- Writing and integrating the vertex and fragment shaders.
- Combining the model, view, and projection transformations into the MVP matrix sent to the vertex shader.
- Adding the optional floor and wall environment and controls to show or hide it.
- Helping implement and test optional diffuse texture loading and rendering.
- Testing the keyboard controls, camera behavior, projection switching, and rendering on the final scene.
- Participating in final integration, documentation review, and demo preparation.

## Shared Contributions

All three members also contributed to the following tasks:

- Discussing the project design and dividing the work into Blender and OpenGL components.
- Reviewing the chair model against the assignment reference images.
- Integrating the Blender export with the OpenGL application.
- Testing the OBJ, MTL, texture, and shader resource paths.
- Reviewing each other's work and resolving integration problems.
- Testing translation, rotation, scaling, wireframe rendering, and camera movement.
- Preparing the required screenshots and submission documents.
- Reviewing the final source code and project directory before submission.
- Preparing for the demonstration and reviewing the complete model-import and rendering pipeline.

## Contribution Summary

The project was completed collaboratively. Nerina's primary responsibility was the Blender model and its visual preparation, Noemie's primary responsibility was the Assimp/OpenGL model-import and mesh-rendering pipeline, and Ryan's primary responsibility was interactive transformations, shaders, and optional scene features. Although each member had a primary area, the team shared testing, integration, documentation, and demo preparation. Every member reviewed the full project and is prepared to explain its main Blender and OpenGL components during the demonstration.

## Team Confirmation

By submitting this report, the team confirms that the contribution descriptions above accurately represent the work completed by each member.

| Team member | Signature or initials | Date |
|---|---|---|
| Nerina An |  |  |
| Noemie Corneillier |  |  |
| Ryan Anthony Khireddine |  |  |
