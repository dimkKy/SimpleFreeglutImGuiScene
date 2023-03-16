## SimpleFreeglutImGuiScene
is a simple program with several primitives displayed using freeglut and ImGui interface to provide some control over scene.
![demo gif](https://media4.giphy.com/media/v1.Y2lkPTc5MGI3NjExMWE4YjMyOTdiODlmM2NmZDZmOTVjMDllMzVhOTMzMWExYWE5ZDYzYyZjdD1n/RfoiM525yghmLS7hTv/giphy.gif)

### Dependencies
* **freeglut** 3.4.0 (latest for today, 16.03.2023), source files can be found [here](https://github.com/FreeGLUTProject/freeglut). Add **FREEGLUT_STATIC** to prepocessor definitions to link statically.
* **Dear ImGui** v1.89 (*imgui_impl_opengl3.cpp* file newer then 2022-11-09 will result in black screen after the first frame. All other files can be taken from 1.89.4 release (latest for today, 16.03.2023)), source files can be found [here](https://github.com/ocornut/imgui).
