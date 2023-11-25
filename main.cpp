#include <SDL2/SDL.h>
#include <cstdlib>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/geometric.hpp>
#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "color.h"
#include "imageloader.h"
#include "intersect.h"
#include "object.h"
#include "light.h"
#include "camera.h"
#include "cube.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const float ASPECT_RATIO = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
const int MAX_RECURSION = 3;
const float BIAS = 0.0001f;

SDL_Renderer* renderer;
std::vector<Object*> objects;
std::vector<Light> lights = {
    {glm::vec3(6.5, 2.0, -1.0f), 1.5f, Color(230, 208, 158)},
};
Camera camera(glm::vec3(7.5, 5.5, 5.0f), glm::vec3(2.5f, 1.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f), 3.0f);


void point(glm::vec2 position, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderDrawPoint(renderer, position.x, position.y);
}

float castShadow(const glm::vec3& shadowOrigin, const glm::vec3& lightDir, Object* hitObject) {
    float shadowIntensity = 1.0f;

    for (const auto& light : lights) {
        for (auto& obj : objects) {
            if (obj != hitObject) {
                Intersect shadowIntersect = obj->rayIntersect(shadowOrigin, light.position - shadowOrigin);
                if (shadowIntersect.isIntersecting && shadowIntersect.dist > 0 && shadowIntersect.dist < glm::length(light.position - shadowOrigin)) {
                    float shadowRatio = shadowIntersect.dist / glm::length(light.position - shadowOrigin);
                    shadowRatio = glm::min(1.0f, shadowRatio);
                    shadowIntensity *= shadowRatio;
                }
            }
        }
    }

    return shadowIntensity;
}


Color castRay(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const short recursion = 0) {
    float zBuffer = 99999;
    Object* hitObject = nullptr;
    Intersect intersect;

    for (const auto& object : objects) {
        Intersect i = object->rayIntersect(rayOrigin, rayDirection);
        if (i.isIntersecting && i.dist < zBuffer) {
            zBuffer = i.dist;
            hitObject = object;
            intersect = i;
        }
    }

    if (!intersect.isIntersecting || recursion == MAX_RECURSION) {
        return Color(0, 0, 0);
    }

    glm::vec3 viewDir = glm::normalize(intersect.point - rayOrigin);
    glm::vec3 reflectDir = glm::reflect(viewDir, intersect.normal);

    Color color(0.0f, 0.0f, 0.0f);

    for (const auto& light : lights) {
        glm::vec3 lightDir = glm::normalize(light.position - intersect.point);

        float shadowIntensity = castShadow(intersect.point, lightDir, hitObject);

        float diffuseLightIntensity = std::max(0.0f, glm::dot(intersect.normal, lightDir));
        float specLightIntensity = std::pow(std::max(0.0f, glm::dot(viewDir, reflectDir)), hitObject->material.specularCoefficient);

        Color texColor = ImageLoader::getPixelColor(intersect.normal.y > 0 ? hitObject->material.topKey : hitObject->material.key, intersect.texCoords.x * hitObject->material.tSize.x, hitObject->material.tSize.y - intersect.texCoords.y * hitObject->material.tSize.y) * 0.6f;

        Color diffuseLight = texColor * light.intensity * diffuseLightIntensity * hitObject->material.albedo * shadowIntensity;
        Color specularLight = light.color * light.intensity * specLightIntensity * hitObject->material.specularAlbedo * shadowIntensity;
        
        color = color + (diffuseLight * light.color) + (specularLight * light.color);
    }

    Color reflectedColor(0.0f, 0.0f, 0.0f);
    if (hitObject->material.reflectivity > 0) {
        glm::vec3 origin = intersect.point + intersect.normal * BIAS;
        reflectedColor = castRay(origin, reflectDir, recursion + 1);
    }

    Color refractedColor(0.0f, 0.0f, 0.0f);
    if (hitObject->material.transparency > 0) {
        glm::vec3 origin = intersect.point - intersect.normal * BIAS;
        glm::vec3 refractDir = glm::refract(rayDirection, intersect.normal, hitObject->material.refractionIndex);
        refractedColor = castRay(origin, refractDir, recursion + 1);
    }

    color = color * (1.0f - hitObject->material.reflectivity - hitObject->material.transparency) + reflectedColor * hitObject->material.reflectivity + refractedColor * hitObject->material.transparency;

    return color;
}


void setUp() {

    objects.push_back(new Cube(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, andesite));
    objects.push_back(new Cube(glm::vec3(0.0f, 0.0f, -1.0f), 1.0f, dirt));
    objects.push_back(new Cube(glm::vec3(0.0f, 0.0f, -2.0f), 1.0f, dirt));
    objects.push_back(new Cube(glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, dirt));
    objects.push_back(new Cube(glm::vec3(1.0f, 0.0f, -1.0f), 1.0f, dirt_rail));
    objects.push_back(new Cube(glm::vec3(2.0f, 0.0f, 0.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(3.0f, 0.0f, 0.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(4.0f, 0.0f, 0.0f), 1.0f, oak_plank));
    objects.push_back(new Cube(glm::vec3(5.0f, 0.0f, 0.0f), 1.0f, oak_plank));
    objects.push_back(new Cube(glm::vec3(2.0f, 0.0f, -1.0f), 1.0f, stone_rail));


    objects.push_back(new Cube(glm::vec3(3.0f, -0.075f, -1.0f), 1.0f, water)); // granite 3 0 -1
    objects.push_back(new Cube(glm::vec3(3.0f, -1.0f, -1.0f), 1.0f, stone)); //temp
    objects.push_back(new Cube(glm::vec3(4.0f, 0.0f, -1.0f), 1.0f, granite)); // granite 4 0 -1
    //objects.push_back(new Cube(glm::vec3(4.0f, -1.0f, -1.0f), 1.0f, stone)); //temp


    objects.push_back(new Cube(glm::vec3(5.0f, 0.0f, -1.0f), 1.0f, oak_plank_rail));
    objects.push_back(new Cube(glm::vec3(1.0f, 0.0f, -2.0f), 1.0f, dirt_rail2)); // minecart
    objects.push_back(new Cube(glm::vec3(2.0f, 0.0f, -2.0f), 1.0f, dirt));


    objects.push_back(new Cube(glm::vec3(3.0f, -0.075f, -2.0f), 1.0f, water)); // stone 3 0 -2
    objects.push_back(new Cube(glm::vec3(3.0f, -1.0f, -2.0f), 1.0f, stone)); // temp
    objects.push_back(new Cube(glm::vec3(3.0f, 0.0f, -3.0f), 1.0f, stone)); // temp


    objects.push_back(new Cube(glm::vec3(4.0f, -0.075f, -2.0f), 1.0f, water)); // granite 4 0 -2
    objects.push_back(new Cube(glm::vec3(4.0f, -1.0f, -2.0f), 1.0f, stone)); // temp
    objects.push_back(new Cube(glm::vec3(4.0f, 0.0f, -3.0f), 1.0f, stone)); // temp


    objects.push_back(new Cube(glm::vec3(5.0f, 0.0f, -2.0f), 1.0f, oak_plank));
    objects.push_back(new Cube(glm::vec3(5.0f, 0.0f, -3.0f), 1.0f, andesite));
    objects.push_back(new Cube(glm::vec3(5.0f, 1.0f, -3.0f), 1.0f, andesite));
    objects.push_back(new Cube(glm::vec3(5.0f, 2.0f, -3.0f), 1.0f, andesite));
    objects.push_back(new Cube(glm::vec3(4.0f, 1.0f, -3.0f), 1.0f, andesite));
    objects.push_back(new Cube(glm::vec3(4.0f, 2.0f, -3.0f), 1.0f, andesite));
    objects.push_back(new Cube(glm::vec3(5.0f, 3.0f, -3.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(4.0f, 3.0f, -3.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(3.0f, 3.0f, -3.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(2.0f, 3.0f, -3.0f), 1.0f, diorite));
    objects.push_back(new Cube(glm::vec3(1.0f, 3.0f, -3.0f), 1.0f, diorite));
    objects.push_back(new Cube(glm::vec3(0.0f, 3.0f, -3.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(0.0f, 2.0f, -3.0f), 1.0f, dirt));
    objects.push_back(new Cube(glm::vec3(0.0f, 1.0f, -3.0f), 1.0f, dirt));
    objects.push_back(new Cube(glm::vec3(1.0f, 1.0f, -3.0f), 1.0f, dirt));
    objects.push_back(new Cube(glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(1.0f, 2.0f, -3.0f), 1.0f, stone));
    objects.push_back(new Cube(glm::vec3(2.0f, 2.0f, -3.0f), 1.0f, coal)); // coal 2 2 -3
    objects.push_back(new Cube(glm::vec3(3.0f, 2.0f, -3.0f), 1.0f, coal));
    objects.push_back(new Cube(glm::vec3(3.0f, 1.0f, -3.0f), 1.0f, coal));
    objects.push_back(new Cube(glm::vec3(5.0f, 3.0f, -2.0f), 1.0f, oak_plank));
    objects.push_back(new Cube(glm::vec3(5.0f, 3.0f, -1.0f), 1.0f, lamp)); // oak
    objects.push_back(new Cube(glm::vec3(5.0f, 3.0f, 0.0f), 1.0f, oak_plank));
    objects.push_back(new Cube(glm::vec3(0.0f, 3.0f, -2.0f), 1.0f, oak_plank));
    objects.push_back(new Cube(glm::vec3(0.0f, 3.0f, -1.0f), 1.0f, lamp)); // oak
    objects.push_back(new Cube(glm::vec3(0.0f, 3.0f, 0.0f), 1.0f, oak_plank));

    for(float i = 0.5f; i<2.5001f; i += 0.25f) {
        objects.push_back(new Cube(glm::vec3(0.0f, i, 0.0f), 0.25f, mini_oak_plank));
    }

    for(float i = 0.5f; i<2.5001f; i += 0.25f) {
        objects.push_back(new Cube(glm::vec3(5.0f, i, 0.0f), 0.25f, mini_oak_plank));
    }

    for(float i = 0.5f; i<2.5001f; i += 0.25f) {
        objects.push_back(new Cube(glm::vec3(0.0f, i, -2.0f), 0.25f, mini_oak_plank));
    }

    for(float i = 0.5f; i<2.5001f; i += 0.25f) {
        objects.push_back(new Cube(glm::vec3(5.0f, i, -2.0f), 0.25f, mini_oak_plank));
    }

    objects.push_back(new Cube(glm::vec3(5.0f, 2.25f, -2.25f), 0.25f, mini_oak_plank));
    objects.push_back(new Cube(glm::vec3(5.0f, 1.5f, -2.25f), 0.25f, mini_oak_plank));
    objects.push_back(new Cube(glm::vec3(5.0f, 1.15f, -2.25f), 0.25f, mini_oak_plank));

    objects.push_back(new Cube(glm::vec3(0.0f, 2.25f, -2.25f), 0.25f, mini_oak_plank));
    objects.push_back(new Cube(glm::vec3(0.0f, 1.5f, -2.25f), 0.25f, mini_oak_plank));
    objects.push_back(new Cube(glm::vec3(0.0f, 1.15f, -2.25f), 0.25f, mini_oak_plank));

    objects.push_back(new Cube(glm::vec3(2.0f, 1.0f, -2.0f), 1.0f, tnt));

}

void render() {
    float fov = 3.1415/3;
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {

            float screenX = (2.0f * (x + 0.5f)) / SCREEN_WIDTH - 1.0f;
            float screenY = -(2.0f * (y + 0.5f)) / SCREEN_HEIGHT + 1.0f;
            screenX *= ASPECT_RATIO;
            screenX *= tan(fov/2.0f);
            screenY *= tan(fov/2.0f);

            glm::vec3 cameraDir = glm::normalize(camera.target - camera.position);

            glm::vec3 cameraX = glm::normalize(glm::cross(cameraDir, camera.up));
            glm::vec3 cameraY = glm::normalize(glm::cross(cameraX, cameraDir));
            glm::vec3 rayDirection = glm::normalize(
                cameraDir + cameraX * screenX + cameraY * screenY
            );
           
            Color pixelColor = castRay(camera.position, rayDirection);
            /* Color pixelColor = castRay(glm::vec3(0,0,20), glm::normalize(glm::vec3(screenX, screenY, -1.0f))); */

            point(glm::vec2(x, y), pixelColor);
        }
    }
}

int main(int argc, char* argv[]) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window* window = SDL_CreateWindow("FPS: 0", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          SCREEN_WIDTH, SCREEN_HEIGHT, 
                                          SDL_WINDOW_SHOWN);

    if (!window) {
        SDL_Log("Unable to create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        SDL_Log("Unable to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    ImageLoader::loadImage("oak_plank", "block/oak_planks.png");
    ImageLoader::loadImage("stone", "block/stone.png");
    ImageLoader::loadImage("dirt", "block/dirt.png");
    ImageLoader::loadImage("granite", "block/granite.png");
    ImageLoader::loadImage("diorite", "block/diorite.png");
    ImageLoader::loadImage("coal_ore", "block/coal_ore.png");
    ImageLoader::loadImage("andesite", "block/andesite.png");
    ImageLoader::loadImage("oak_planks_rail", "block/oak_planks_rail.png");
    ImageLoader::loadImage("stone_rail", "block/stone_rail.png");
    ImageLoader::loadImage("dirt_rail", "block/dirt_rail.png");
    ImageLoader::loadImage("dirt_rail2", "block/dirt_rail2.png");
    ImageLoader::loadImage("tnt", "block/tnt_side.png");
    ImageLoader::loadImage("tnt_top", "block/tnt_top.png");
    ImageLoader::loadImage("water", "block/water.png");
    ImageLoader::loadImage("lamp", "block/redstone_lamp.png");

    int frameCount = 0;
    Uint32 startTime = SDL_GetTicks();
    Uint32 currentTime = startTime;
    
    setUp();

    while (running) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch(event.key.keysym.sym) {
                    case SDLK_UP:
                        camera.move(-1.0f);
                        break;
                    case SDLK_DOWN:
                        camera.move(1.0f);
                        break;
                    case SDLK_LEFT:
                        camera.rotate(-1.0f, 0.0f);
                        break;
                    case SDLK_RIGHT:
                        camera.rotate(1.0f, 0.0f);
                        break;
                 }
            }

        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        render();

        // Present the renderer
        SDL_RenderPresent(renderer);

        frameCount++;

        // Calculate and display FPS
        if (SDL_GetTicks() - currentTime >= 1000) {
            currentTime = SDL_GetTicks();
            std::string title = "FPS: " + std::to_string(frameCount);
            SDL_SetWindowTitle(window, title.c_str());
            frameCount = 0;
        }
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

