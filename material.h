#pragma once

#include "color.h"
#include <string>

struct Material {
  Color diffuse;
  float albedo;
  float specularAlbedo;
  float specularCoefficient;
  float reflectivity;
  float transparency;
  float refractionIndex;
  std::string key;
  std::string topKey;
  glm::vec2 tSize;
};

 Material coal = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "coal_ore",
        "coal_ore",
        glm::vec2(128, 128),
    };

    Material stone = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "stone",
        "stone",
        glm::vec2(128, 128),
    };

    Material stone_rail = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "stone",
        "stone_rail",
        glm::vec2(128, 128),
    };

    Material andesite = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "andesite",
        "andesite",
        glm::vec2(128, 128),
    };

    Material oak_plank = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "oak_plank",
        "oak_plank",
        glm::vec2(128, 128),
    };

    Material oak_plank_rail = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "oak_plank",
        "oak_planks_rail",
        glm::vec2(128, 128),
    };

    Material mini_oak_plank = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "oak_plank",
        "oak_plank",
        glm::vec2(32, 32),
    };

    Material dirt = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "dirt",
        "dirt",
        glm::vec2(128, 128),
    };

    Material dirt_rail = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "dirt",
        "dirt_rail",
        glm::vec2(128, 128),
    };

    Material dirt_rail2 = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "dirt",
        "dirt_rail2",
        glm::vec2(128, 128),
    };

    Material granite = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "granite",
        "granite",
        glm::vec2(128, 128),
    };

    Material diorite = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "diorite",
        "diorite",
        glm::vec2(128, 128),
    };

    Material tnt = {
        Color(80, 0, 0),   // diffuse
        0.9,
        0.1,
        10.0f,
        0.0f,
        0.0f,
        0.0f,
        "tnt",
        "tnt_top",
        glm::vec2(128, 128),
    };

    Material water = {
        Color(43, 60, 244),   // diffuse
        1.0,
        0.3,
        10.0f,
        0.15f,
        0.2f,
        1.333f,
        "water",
        "water",
        glm::vec2(128, 128),
    };

    Material lamp = {
        Color(43, 60, 244),   // diffuse
        1.0,
        0.3,
        10.0f,
        0.0f,
        0.5f,
        0.5f,
        "lamp",
        "lamp",
        glm::vec2(128, 128),
    };