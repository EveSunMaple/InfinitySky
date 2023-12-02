/*#include "CameraModule.h"
#include <easy2d/easy2d.h>
#include <vector>
#include <queue>
#include <iostream>
using namespace easy2d;
using namespace std;

void Camera::SetTarget(BaseObject& target)
{
    float posX = target.x;
    float posY = target.y;
    float Width = target.collisionX;
    float Height = target.collisionY;
    float lx = x + centerX - responseLX;
    float ly = y - centerY + responseLY;
    float rx = x + centerX + responseRX;
    float ry = y - centerY - responseRY;

    if (posX < lx)
    {
        x = posX - centerX + responseLX;
    }
    if (posY > ly)
    {
        y = posY + centerY - responseLY;
    }
    if (posX + Width > rx)
    {
        x = posX + Width - centerX - responseLX;
    }
    if (posY - Height < ry)
    {
        y = posY - Height + centerY + responseLY;
    }

    if (ctrlLX != 0 || ctrlLY != 0 || ctrlRX != 0 || ctrlRY != 0)
    {
        if (x + speedX < ctrlLX)
        {
            x = ctrlLX;
            speedX = 0;
        }
        if (x + width + speedX > ctrlRX)
        {
            x = ctrlRX - width;
            speedX = 0;
        }
        if (y + speedY > ctrlLY)
        {
            y = ctrlLY;
            speedY = 0;
        }
        if (y - height + speedY < ctrlRY)
        {
            y = ctrlRY + height;
            speedY = 0;
        }
    }
}
void Camera::ProjectObject(BaseObject& object)
{
    object.winX = object.x - x;
    object.winY = y - object.y;
    object.baseObject->setPos(object.winX, object.winY);
    object.baseObject->setAnchor(0.5f, 0.5f);
    object.baseObject->setRotation(object.angle);
    /*
    if constexpr (std::is_same<T, Player>::value)
    {
    }
    if constexpr (std::is_same<T, Weapon>::value)
    {
        object.winX = object.x - x;
        object.winY = y - object.y;
        object.node->setPos(object.winX, object.winY);
        object.node->setAnchor(0.5f, 0.5f);
        object.node->setRotation(object.angle);
        for (float i = 0; i < object.bullet_list.size(); i++)
        {
            object.bullet_list[i].x += object.bullet_list[i].speedX;
            object.bullet_list[i].y += object.bullet_list[i].speedY;
            object.bullet_list[i].winX = object.bullet_list[i].x - x;
            object.bullet_list[i].winY = y - object.bullet_list[i].y;

            if (object.bullet_list[i].bulletNode != nullptr)
            {
                object.bullet_list[i].bulletNode->setPos(object.bullet_list[i].winX, object.bullet_list[i].winY);
                object.bullet_list[i].bulletNode->setAnchor(0.5f, 0.5f);
                object.bullet_list[i].bulletNode->setRotation(object.bullet_list[i].angle);
            }
        }
    }

    if constexpr (std::is_same<T, Particle>::value)
    {
        object.winX = object.x - x;
        object.winY = y - object.y;
        double x = -object.x + object.winX;
        double y = object.y + object.winY;

        auto action = gcnew Spawn;
        class easy2d::ShapeNode* part = gcnew ShapeNode;
        class easy2d::Sprite* sprite = object.particleSprite;

        DrawingStyle style;
        style.mode = DrawingStyle::Mode::Solid;      
        style.fillColor = object.baseColor;          
        part = ShapeNode::createCircle(object.radius, style);

        object.particleNode->addChild(part);
        object.particleNode->setPos(x, y);

        float angle = object.trueAngle * acos(-1) / 180;
        float time = 1 / Time::getDeltaTime();

        float speed_x = (sin(angle) * object.speed) * object.lifeTime;
        float speed_y = (cos(angle) * object.speed) * object.lifeTime;
        if (object.Follow)
        {
            speed_x += object.baseSpeedX * 120 * object.lifeTime;
            speed_y += object.baseSpeedY * 120 * object.lifeTime;
        }
        auto move = gcnew MoveBy(1.0f, Vector2(speed_x, -speed_y));
        if (object.action == NULL) action = gcnew Spawn({ move });
        else action = gcnew Spawn({ move, object.action->clone() });

        part->setPos(-x + object.winX, -y + object.winY);
        part->runAction(action);
        part->setAnchor(0.5f, 0.5f);

        if (sprite != NULL)
        {
            part->addChild(sprite);
            sprite->setRotation(object.trueAngle);
            sprite->setAnchor(0.5f, 0.5f);
        }

        Timer::add([=]() {object.particleNode->removeChild(part); }, object.lifeTime, 1);
    }
}
*/