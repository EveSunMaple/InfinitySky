#include <easy2d/easy2d.h>
#include <iostream>
#include <tuple>
#include "SceneControl.h"
#include "CameraModule.h"
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
tuple<bool, bool, bool, bool, bool, bool> Control(const BaseObject& obj)
{
    tuple<bool, bool, bool, bool, bool, bool> controlValue;
    if (Input::isDown(KeyCode::W))
    {
        get<0>(controlValue) = true;
    }
    if (Input::isDown(KeyCode::S))
    {
        get<1>(controlValue) = true;
    }
    if (Input::isDown(KeyCode::A))
    {
        get<2>(controlValue) = true;
    }
    if (Input::isDown(KeyCode::D))
    {
        get<3>(controlValue) = true;
    }
    if (Input::isDown(KeyCode::Q))
    {
        get<4>(controlValue) = true;
    }
    if (Input::isDown(KeyCode::E))
    {
        get<5>(controlValue) = true;
    }
    return controlValue;
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
        cout << Ground_width << endl;
        cout << Ground_height << endl;
    }
};
class WorldPage : public Sprite
{
public:
    BaseObject backGround;
    BaseObject player;
    BaseObject leftGun;
    BaseObject rightGun;
    BaseObject bullet;
    Camera mainCamera;
    WorldPage()
    {
        auto ground = gcnew BackGround;
        auto playerShip = gcnew Sprite;
        auto bulletSprite = gcnew Sprite;
        playerShip->open("PNG/playerShip1_blue.png");
        bulletSprite->open("PNG/Lasers/laserBlue01.png");
        backGround = BaseObject(true, this, ground, nullptr, 0, 0, width * 4, height * 4);
        player = BaseObject(true, this, playerShip, nullptr, 0, 0, 0, 0, 0, 0, Color::ForestGreen);
        player.SetControlFunction(Control);
        player.baseSprite = playerShip;

        bullet = BaseObject(false, nullptr, bulletSprite, 0, 0, 0, 0, 0, 0);
        leftGun = BaseObject(true, this, nullptr, &player, 0, 0, 0, 0, 34, 20);
        rightGun = BaseObject(true, this, nullptr, &player, 0, 0, 0, 0, -34, 20);
        leftGun.childObject = &bullet;
        rightGun.childObject = &bullet;
        leftGun.childSpeed = 10;
        rightGun.childSpeed = 10;
        leftGun.followFather = true;
        rightGun.followFather = true;
        leftGun.SetChildrenFunction(Fire);
        rightGun.SetChildrenFunction(Fire);
        mainCamera = Camera(&player);
    }
    void onUpdate()
    {
        player.ApplyControlFunction();
        leftGun.ApplyChildrenFunction();
        rightGun.ApplyChildrenFunction();
        mainCamera.FollowTarget();
        mainCamera.ProjectObject(player);
        mainCamera.ProjectObject(backGround);
        mainCamera.ProjectObject(leftGun);
        mainCamera.ProjectObject(rightGun);
        // cout << leftGun.childlist.size() << '\n';
        if (mainCamera.x <= backGround.x) backGround.x = (int)backGround.x - Ground_width;
        if (mainCamera.y >= backGround.y) backGround.y = (int)backGround.y + Ground_height;
        if (mainCamera.x >= backGround.x + Ground_width) backGround.x = (int)backGround.x + Ground_width;
        if (mainCamera.y <= backGround.y - Ground_height) backGround.y = (int)backGround.y - Ground_height;
    }
};

int main()
{
    if (Game::init("InfinitySky: 无尽之空", 960, 640))
    {
        // Renderer::showFps(true);			//显示FPS

        // auto sprite = gcnew Sprite;
        // sprite->open("PNG/Lasers/laserBlue11.png"); // 从本地图片加载
        // sprite->setAnchor(0.5f, 0.5f);
        // Window::setCustomCursor(sprite);

        auto world = new Scene;             //新建场景

        SceneManager::enter(world);         //进入场景

        auto worldPage = gcnew WorldPage;   //加载世界

        world->addChild(worldPage);         //把世界添加进场景

        Game::start();
    }
    Game::destroy();
    return 0;
}