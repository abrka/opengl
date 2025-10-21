cmake_minimum_required(VERSION 3.6)
project(opengl_proj)

file(GLOB OPENGL_SOURCES src/*.h src/*.cpp)
add_executable(opengl_demo ${OPENGL_SOURCES})
set_property(TARGET opengl_demo PROPERTY CXX_STANDARD 20)
target_include_directories(opengl_demo PUBLIC src)

# Copy Assets to build dir
add_custom_command(
        TARGET opengl_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/data/meshes
                $<TARGET_FILE_DIR:opengl_demo>/meshes)

add_custom_command(
        TARGET opengl_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/data/textures
                $<TARGET_FILE_DIR:opengl_demo>/textures)

add_custom_command(
        TARGET opengl_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_CURRENT_SOURCE_DIR}/data/shaders
                $<TARGET_FILE_DIR:opengl_demo>/shaders)

# Build glad 
add_library(glad external/glad/src/glad.c external/glad/include/glad/glad.h external/glad/include/KHR/khrplatform.h)
target_include_directories(glad PUBLIC external/glad/include)
target_link_libraries(opengl_demo PUBLIC glad)

# Add glfw
add_subdirectory(external/glfw)
target_link_libraries(opengl_demo PUBLIC glfw)

# Add glm
add_subdirectory(external/glm)
target_link_libraries(opengl_demo PUBLIC glm::glm)

# Add assimp
add_subdirectory(external/assimp)
target_link_libraries(opengl_demo PUBLIC assimp::assimp)

# Build imgui
add_library(imgui external/imgui-docking/imconfig.h
                  external/imgui-docking/imgui_demo.cpp
                  external/imgui-docking/imgui_draw.cpp
                  external/imgui-docking/imgui_internal.h
                  external/imgui-docking/imgui_tables.cpp 
                  external/imgui-docking/imgui_widgets.cpp 
                  external/imgui-docking/imgui.cpp 
                  external/imgui-docking/imgui.h 
                  external/imgui-docking/imstb_rectpack.h 
                  external/imgui-docking/imstb_textedit.h 
                  external/imgui-docking/imstb_truetype.h 
                  external/imgui-docking/misc/cpp/imgui_stdlib.cpp 
                  external/imgui-docking/misc/cpp/imgui_stdlib.h
                  external/imgui-docking/backends/imgui_impl_opengl3.h
                  external/imgui-docking/backends/imgui_impl_opengl3.cpp
                  external/imgui-docking/backends/imgui_impl_glfw.h
                  external/imgui-docking/backends/imgui_impl_glfw.cpp )

target_include_directories(imgui PUBLIC external/imgui-docking/backends external/imgui-docking external/imgui-docking/misc/cpp)
target_link_libraries(imgui PUBLIC glfw)
target_link_libraries(opengl_demo PUBLIC imgui)

