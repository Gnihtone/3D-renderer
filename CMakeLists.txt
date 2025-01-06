cmake_minimum_required(VERSION 3.30)
project(3D_renderer)

if (POLICY CMP0167)
    cmake_policy(SET CMP0167 OLD)
endif ()

set(CMAKE_CXX_STANDARD 23)
set(Boost_INCLUDE_DIR $ENV{Boost_INCLUDE_DIR})

add_executable(3D_renderer main.cpp
        application.cpp
        application.h
        components/visualizer.cpp
        components/visualizer.h
        common/geometry.cpp
        common/geometry.h
        common/object.cpp
        common/object.h
        components/graphics.cpp
        components/graphics.h
        components/world.cpp
        components/world.h)

find_package(Boost 1.87 REQUIRED)
target_link_libraries(3D_renderer PRIVATE ${Boost_LIBRARIES})
include_directories(${Boost_INCLUDE_DIR})

find_package(SFML 3 REQUIRED COMPONENTS Graphics Audio Network REQUIRED)
target_link_libraries(3D_renderer PRIVATE SFML::System SFML::Graphics SFML::Audio SFML::Network)
include_directories(${SFML_INCLUDE_DIR})

find_package(glm REQUIRED)
target_link_libraries(3D_renderer PRIVATE glm::glm)
include_directories(${glm_INCLUDE_DIR})
