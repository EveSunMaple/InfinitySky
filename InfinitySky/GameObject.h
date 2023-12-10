#pragma once
#include <easy2d/easy2d.h>
#include "SceneControl.h"
#include "CameraModule.h"
#include "Environment.h"
#include "Function.h"
using namespace easy2d;

/*class PlayerShip1_blue
{
public:
    BaseObject backGround;
    BaseObject player;

    BaseObject bullet;
    BaseObject leftGun;
    BaseObject rightGun;

    BaseObject particle[3];
    BaseObject particleGun[6];
    PlayerShip1_blue()
    {
        player = BaseObject(1, true, this, playerShip1_blue, nullptr, nullptr, false, 0, 0, 0, 0, 0, 0, Color::ForestGreen);
        player.SetControlFunction(BaseKeyControl);
        Point p1 = Point(45, -7); player.collisionPoints.push_back(p1);
        p1 = Point(44, 20); player.collisionPoints.push_back(p1);
        p1 = Point(6, 27); player.collisionPoints.push_back(p1);
        p1 = Point(-6, 27); player.collisionPoints.push_back(p1);
        p1 = Point(-44, 20); player.collisionPoints.push_back(p1);
        p1 = Point(-45, -7); player.collisionPoints.push_back(p1);
        p1 = Point(-5, -37); player.collisionPoints.push_back(p1);
        p1 = Point(5, -37); player.collisionPoints.push_back(p1);

        bullet = BaseObject(0, false, nullptr, laserBlue01, nullptr, nullptr, false, 0, 0, 0, 0, 0, 0);
        leftGun = BaseObject(0, true, this, nullptr, &player, nullptr, true, 0, 0, 0, 0, 34, 20);
        rightGun = BaseObject(0, true, this, nullptr, &player, nullptr, true, 0, 0, 0, 0, -34, 20);
        leftGun.SetChildrenFunction(BaseFireControl);
        rightGun.SetChildrenFunction(BaseFireControl);
        leftGun.childObject = &bullet;
        rightGun.childObject = &bullet;
        leftGun.childSpeed = 10;
        rightGun.childSpeed = 10;

        particle[0] = BaseObject(0, false, nullptr, particle1_white, nullptr, action_particle1, false, 0, 0, 0, 0, 0, 0);
        particle[1] = BaseObject(0, false, nullptr, particle1_blue, nullptr, action_particle2, false, 0, 0, 0, 0, 0, 0);
        particle[2] = BaseObject(0, false, nullptr, particle1_yellow, nullptr, action_particle3, false, 0, 0, 0, 0, 0, 0);
        particleGun[0] = BaseObject(0, true, this, nullptr, &player, nullptr, false, 0, 0, 0, 0, -44, -20);
        particleGun[1] = BaseObject(0, true, this, nullptr, &player, nullptr, false, 0, 0, 0, 0, 44, -20);
        particleGun[2] = BaseObject(0, true, this, nullptr, &player, nullptr, true, 0, 0, 0, 0, -25, -15);
        particleGun[3] = BaseObject(0, true, this, nullptr, &player, nullptr, true, 0, 0, 0, 0, 25, -15);
        particleGun[4] = BaseObject(0, true, this, nullptr, &player, nullptr, true, 0, 0, 0, 0, -25, -15);
        particleGun[5] = BaseObject(0, true, this, nullptr, &player, nullptr, true, 0, 0, 0, 0, 25, -15);
        particleGun[0].childObject = &particle[0];
        particleGun[1].childObject = &particle[0];
        particleGun[2].childObject = &particle[1];
        particleGun[3].childObject = &particle[1];
        particleGun[4].childObject = &particle[2];
        particleGun[5].childObject = &particle[2];
        particleGun[2].childSpeed = 1.5f;
        particleGun[3].childSpeed = 1.5f;
        particleGun[4].childSpeed = 1.5f;
        particleGun[5].childSpeed = 1.5f;
        particleGun[2].childAngle = 180;
        particleGun[3].childAngle = 180;
        particleGun[4].childAngle = 180;
        particleGun[5].childAngle = 180;
        for (unsigned int i = 0; i <= 5; i++)
        {
            particleGun[i].SetChildrenFunction(BaseParticle);
            particleGun[i].lntervalTime = 0.0f;
            particleGun[i].lifeTime = 0.8f;
        }

        mainCamera = Camera(&player);
        mainCamera.ProjectCollision(player);
    }
};