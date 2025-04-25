//
// Created by Gniht on 10.04.2025.
//

#include "cube.h"

namespace renderer {
namespace {

const std::vector<glm::vec3> cube_vertices = {
    {-0.5, -0.5, -0.5},  // FRONT-BOTTOM-LEFT
    {0.5, -0.5, -0.5},   // FRONT-BOTTOM-RIGHT
    {0.5, 0.5, -0.5},    // FRONT-TOP-RIGHT
    {-0.5, 0.5, -0.5},   // FRONT-TOP-LEFT

    {-0.5, -0.5, 0.5},  // BACK-BOTTOM-LEFT
    {0.5, -0.5, 0.5},   // BACK-BOTTOM-RIGHT
    {0.5, 0.5, 0.5},    // BACK-TOP-RIGHT
    {-0.5, 0.5, 0.5}    // BACK-TOP-LEFT
};

const std::vector<Triangle3D> cube_triangles = {
    // FRONT
    {.points = {cube_vertices[2], cube_vertices[1], cube_vertices[0]}, .color = Color::kRed},
    {.points = {cube_vertices[3], cube_vertices[2], cube_vertices[0]}, .color = Color::kRed},
    //
    // // LEFT
    {.points = {cube_vertices[4], cube_vertices[3], cube_vertices[0]}, .color = Color::kBlue},
    {.points = {cube_vertices[7], cube_vertices[3], cube_vertices[4]}, .color = Color::kBlue},
    //
    // // RIGHT
    {.points = {cube_vertices[1], cube_vertices[2], cube_vertices[5]}, .color = Color::kGreen},
    {.points = {cube_vertices[5], cube_vertices[2], cube_vertices[6]}, .color = Color::kGreen},

    // TOP
    {.points = {cube_vertices[2], cube_vertices[3], cube_vertices[6]}, .color = Color::kWhite},
    {.points = {cube_vertices[6], cube_vertices[3], cube_vertices[7]}, .color = Color::kWhite},

    // BOTTOM
    {.points = {cube_vertices[0], cube_vertices[1], cube_vertices[4]}, .color = Color::kYellow},
    {.points = {cube_vertices[4], cube_vertices[1], cube_vertices[5]}, .color = Color::kYellow},

    // BACK
    {.points = {cube_vertices[4], cube_vertices[5], cube_vertices[6]}, .color = Color::kPurple},
    {.points = {cube_vertices[6], cube_vertices[7], cube_vertices[4]}, .color = Color::kPurple},
};

}  // namespace

Cube::Cube() : Object(cube_triangles) {}

}  // namespace renderer