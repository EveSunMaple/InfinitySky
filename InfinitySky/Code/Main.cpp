#include <easy2d/easy2d.h>
#include <stdio.h>
#include <windows.h>
#include <iostream>
#include <thread>
#include <mutex>
#include <tuple>
#include "SceneControl.hpp"
#include "CameraModule.hpp"
#include "Environment.hpp"
#include "SourceLoad.hpp"
#include "Function.hpp"
#define width 1280
#define height 960

using namespace easy2d;
using namespace std;

int Back_length = 256;
int Ground_width = 0;
int Ground_height = 0;

bool Fire(const BaseObject& obj)
{
    if (Input::isDown(MouseCode::Left)) return true;
    return false;
}

class BackGround : public Sprite
{
public:
    BackGround()
    {
        int i = 0, j = 0;
        for (Ground_width = 0; Ground_width <= width * 4; i++)
        {
            for (Ground_height = 0; Ground_height <= height * 4; j++)
            {
                auto sprite = gcnew Sprite;
                sprite->open("Backgrounds/purple.png");
                this->addChild(sprite);
                sprite->setPos(Ground_width, Ground_height);
                Ground_height += Back_length;
                if (Ground_height >= height * 4) break;
            }
            Ground_width += Back_length;
            if (Ground_width >= width * 4) break;
        }
        Ground_width = Back_length * (i / 4);
        Ground_height = Back_length * (j / i / 4);
    }
};
/*
class WorldPage : public Sprite
{
public:
    BaseObject backGround;
    BaseObject player;

    BaseObject bullet;
    BaseObject leftGun;
    BaseObject rightGun;

    BaseObject particle[3];
    BaseObject particleGun[6];
    Camera mainCamera;
    WorldPage()
    {
        auto ground = gcnew BackGround;
        backGround = BaseObject(0, true, this, ground, nullptr, nullptr, false, 0, 0, width * 4, height * 4);
        player = BaseObject(1, true, this, spriteMap["playerShip1_blue"], nullptr, nullptr, false, 0, 0, 0, 0, 0, 0, Color::ForestGreen);
        player.SetControlFunction(BaseKeyControl);
        Point p1 = Point(0, 0);
        p1 = Point(10, -13); player.collisionPoints.push_back(p1);
        p1 = Point(36, 2); player.collisionPoints.push_back(p1);
        p1 = Point(45, -3); player.collisionPoints.push_back(p1);
        p1 = Point(43, 20); player.collisionPoints.push_back(p1);
        p1 = Point(6, 27); player.collisionPoints.push_back(p1);
        p1 = Point(-6, 27); player.collisionPoints.push_back(p1);
        p1 = Point(-43, 20); player.collisionPoints.push_back(p1);
        p1 = Point(-45, -3); player.collisionPoints.push_back(p1);
        p1 = Point(-36, 2); player.collisionPoints.push_back(p1);
        p1 = Point(-10, -13); player.collisionPoints.push_back(p1);
        p1 = Point(-6, -37); player.collisionPoints.push_back(p1);
        p1 = Point(6, -37); player.collisionPoints.push_back(p1);

        bullet = BaseObject(0, false, nullptr, spriteMap["laserBlue01"], nullptr, nullptr, false, 0, 0, 0, 0, 0, 0);
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
    void onUpdate()
    {
        player.ApplyControlFunction();
        leftGun.ApplyChildrenFunction();
        rightGun.ApplyChildrenFunction();
        mainCamera.FollowTarget();
        mainCamera.ProjectObject(backGround);
        mainCamera.ProjectObject(leftGun);
        mainCamera.ProjectObject(rightGun);
        mainCamera.ProjectObject(player);
        for (unsigned int i = 0; i <= 5; i++)
        {
            mainCamera.ProjectObject(particleGun[i]);
        }
        particleGun[0].ApplyChildrenFunction();
        particleGun[1].ApplyChildrenFunction();
        if (Input::isDown(KeyCode::W))
        {
            GameInit();
            particleGun[2].ApplyChildrenFunction();
            particleGun[3].ApplyChildrenFunction();
            particleGun[4].ApplyChildrenFunction();
            particleGun[5].ApplyChildrenFunction();
        }
        if (mainCamera.x <= backGround.x) backGround.x = (int)backGround.x - Ground_width;
        if (mainCamera.y >= backGround.y) backGround.y = (int)backGround.y + Ground_height;
        if (mainCamera.x >= backGround.x + Ground_width) backGround.x = (int)backGround.x + Ground_width;
        if (mainCamera.y <= backGround.y - Ground_height) backGround.y = (int)backGround.y - Ground_height;
    }
};*/

int main()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);//获取控制台光标信息
    CursorInfo.bVisible = false; //隐藏控制台光标
    SetConsoleCursorInfo(handle, &CursorInfo);//设置控制台光标状态

    if (Game::init("InfinitySky: 无尽之空", 960, 640))
    {
        loadScene = gcnew Scene;
        auto loadPage = gcnew LoadPage;   
        loadScene->addChild(loadPage);
        SceneManager::enter(loadScene);

        // auto world = gcnew Scene;
        // auto worldPage = gcnew WorldPage;  
        // world->addChild(worldPage);        
        // SceneManager::enter(world);         
        // loadThread.join();
        // time(&timedelay);
        Game::start();

        // Renderer::showFps(true);			//显示FPS

        // Window::setCustomCursor(aim);
    }
    Game::destroy();
    return 0;
}