cmake_minimum_required(VERSION 3.30)
project(3D_renderer VERSION 0.1)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(3D_renderer main.cpp
        application.cpp
        except.cpp
        common/geometry.cpp
        common/picture.cpp
        common/transform.cpp
        common/z_buffer.cpp
        components/camera.cpp
        components/graphics.cpp
        components/light.cpp
        components/object.cpp
        components/runtime.cpp
        components/visualizer.cpp
        components/world.cpp
        generators/object_generator.cpp
        objects/cube.cpp
)

set(BUILD_SHARED_LIBS ON)
add_subdirectory(deps/SFML EXCLUDE_FROM_ALL)
target_link_libraries(3D_renderer PRIVATE sfml-window sfml-graphics)

add_subdirectory(deps/assimp EXCLUDE_FROM_ALL)
target_link_libraries(3D_renderer PRIVATE assimp)

include_directories(${PROJECT_SOURCE_DIR}/deps/assimp/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/SFML/include)
include_directories(${PROJECT_SOURCE_DIR}/deps/glm)

add_custom_command(TARGET 3D_renderer PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${SFML_BINARY_DIR}/bin/ ${CMAKE_BINARY_DIR}/bin)
