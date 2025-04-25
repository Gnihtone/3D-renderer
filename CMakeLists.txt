cmake_minimum_required(VERSION 3.30)
project(3D_renderer)

add_subdirectory(deps/boost EXCLUDE_FROM_ALL)

if (POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif ()

set(BOOST_ENABLE_CMAKE ON)

#include(FetchContent)
#FetchContent_Declare(
#        Boost
#        GIT_REPOSITORY https://github.com/boostorg/boost.git
#        GIT_TAG boost-1.87.0
#        GIT_SHALLOW TRUE
#)
#FetchContent_MakeAvailable(Boost)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(Boost_INCLUDE_DIR $ENV{Boost_INCLUDE_DIR})

add_executable(3D_renderer main.cpp
        application.cpp
        except.cpp
        components/visualizer.cpp
        common/geometry.cpp
        components/object.cpp
        components/graphics.cpp
        components/world.cpp
        generators/object_generator.cpp
        components/camera.cpp
        objects/cube.cpp
        common/picture.cpp
        common/z_buffer.cpp
        components/runtime.cpp
        common/transform.cpp
        components/light.cpp
)

#find_package(Boost 1.87 REQUIRED)
#target_link_libraries(3D_renderer PRIVATE ${Boost_LIBRARIES})
#include_directories(${Boost_INCLUDE_DIR})

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network REQUIRED)
target_link_libraries(3D_renderer PRIVATE SFML::System SFML::Graphics SFML::Audio SFML::Network)
include_directories(${SFML_INCLUDE_DIR})

find_package(glm REQUIRED)
target_link_libraries(3D_renderer PRIVATE glm::glm)
include_directories(${glm_INCLUDE_DIR})
