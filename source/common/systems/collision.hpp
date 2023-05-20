#pragma once
#include "../ecs/world.hpp"
#include "../components/movement.hpp"
#include "../components/mesh-renderer.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>
#include <string>

namespace our
{

    class CollisionSystem
    {
        std::vector<Entity *> cars, batteries, planes, packages;
        Entity *player;

    public:
        float two_d_collides(const glm::vec4 &carMax, const glm::vec4 &carMin, const glm::vec4 &batteryMax, const glm::vec4 &batteryMin)
        {
            // calcculate the eclidian distance between the centers of the two objects
            float distance = glm::distance(glm::vec3(carMax.x, carMax.y, carMax.z), glm::vec3(batteryMax.x, batteryMax.y, batteryMax.z));
            return distance;
            
            /*
            // apply the following algorithm a.minX <= b.maxX && a.maxX >= b.minX && a.minY <= b.maxY && a.maxY >= b.minY && a.minZ <= b.maxZ && a.maxZ >= b.minZ
            if ( (carMax.x >= batteryMin.x && carMin.x <= batteryMax.x &&
                    carMax.y >= batteryMin.y && carMin.y <= batteryMax.y &&
                    carMax.z >= batteryMin.z && carMin.z <= batteryMax.z) )
            {
                std::cout<< "collision happened" << std::endl;
                return true;
            }
            else
            {
                //std::cout<< "collision didn't happen" << std::endl;
                return false;
            }
            */
        }

        bool checkCollision(Entity *batteryComponent, Entity *carComponent)
        {

            // get centers and difference between centers
            glm::vec4 carCenter = carComponent->getLocalToWorldMatrix() * glm::vec4(carComponent->localTransform.position, 1.0);
            glm::vec4 batteryCenter = glm::vec4(batteryComponent->localTransform.position, 1.0);

            glm::vec4 carMax = carCenter + glm::vec4(carComponent->localTransform.scale, 0.0);
            glm::vec4 carMin = carCenter - glm::vec4(carComponent->localTransform.scale, 0.0);

            glm::vec4 batteryMax = batteryCenter + glm::vec4(batteryComponent->localTransform.scale, 0.0);
            glm::vec4 batteryMin = batteryCenter - glm::vec4(batteryComponent->localTransform.scale, 0.0);

            // check two_d_collides for x and z
            bool result = false;
            result = (two_d_collides(carMax, carMin, batteryMax, batteryMin) < 2.5f) ? 1 : 0;
            return result;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        int update(World *world, float deltaTime)
        {
            batteries.clear();
            cars.clear();

            // iterate over all entities in the world and check for collisions with the player
            for (const auto &entity : world->getEntities())
            {
                if (entity->name.substr(0, 7) == "battery")
                {
                    batteries.push_back(entity);
                }
                else if ((entity->name.substr(0, 2) == "cr") || (entity->name.substr(0, 2) == "cl"))
                {
                    cars.push_back(entity);
                }
                /*
                else if ( (entity->name.substr(0, 5) =="plane") )
                {
                    planes.push_back(entity);
                }
                */
                else if ( (entity->name.substr(0, 7) =="package") )
                {
                    packages.push_back(entity);
                }
                else if (entity->name.substr(0, 4) == "play")
                {
                    player = entity;
                }
            }

            for (Entity *battery : batteries)
            {
                // if collision happened with a battery
                if (checkCollision(battery, player))
                {
                    world->markForRemoval(battery);
                    // world->deleteMarkedEntities();

                    return 1; // collision with battery
                }
            }

            for (Entity *car : cars)
            {
                // if collision happened with a battery
                if (checkCollision(car, player))
                {
                    world->markForRemoval(car);
                    // world->deleteMarkedEntities();
                    return -1; // collision with car
                }
            }

            /*
            for ( Entity* plane : planes)
            {
                // if collision happened with a battery
                if (checkCollision(plane, player ))
                {
                    world->markForRemoval(plane);
                    world->deleteMarkedEntities();
                    return -1; //collision with car
                }
            }
            */

            for (Entity *package : packages)
            {
                // std::cout<< "in packagggggggggggggggggggggggggggggggge" << std::endl;
                //  if collision happened with a battery
                if (checkCollision(package, player))
                {
                    world->markForRemoval(package);
                    //  world->deleteMarkedEntities();
                    return 2; // collision with car
                }
            }

            return 0;
        }
    };
}