#pragma once
#include <easy2d/easy2d.h>
#include "SceneControl.h"
using namespace easy2d;

void BaseKeyControl(BaseObject* obj)
{
    bool upKey = false, downKey = false, leftKey = false, rightKey = false, rotateLeftKey = false, rotateRightKey = false;
    if (Input::isDown(KeyCode::W))
    {
        upKey = true;
    }
    if (Input::isDown(KeyCode::S))
    {
        downKey = true;
    }
    if (Input::isDown(KeyCode::A))
    {
        leftKey = true;
    }
    if (Input::isDown(KeyCode::D))
    {
        rightKey = true;
    }
    if (Input::isDown(KeyCode::Q))
    {
        rotateLeftKey = true;
    }
    if (Input::isDown(KeyCode::E))
    {
        rotateRightKey = true;
    }
    //Move
    double time = (double)Time::getDeltaTime();
    float Angle = obj->angle * acos(-1) / 180;
    if (upKey)
    {
        obj->speedY += cos(Angle) * obj->acceleration * time;
        obj->speedX += sin(Angle) * obj->acceleration * time;
    }
    if (downKey)
    {
        obj->speedY -= cos(Angle) * obj->acceleration * time;
        obj->speedX -= sin(Angle) * obj->acceleration * time;
    }
    if (leftKey)
    {
        obj->speedY += sin(Angle) * obj->acceleration * time;
        obj->speedX -= cos(Angle) * obj->acceleration * time;
    }
    if (rightKey)
    {
        obj->speedY -= sin(Angle) * obj->acceleration * time;
        obj->speedX += cos(Angle) * obj->acceleration * time;
    }

    if (abs(obj->speedX) < abs(sin(Angle) * obj->friction * time)) obj->speedX = 0;
    else
    {
        if (obj->speedX < 0) obj->speedX += abs(sin(Angle) * obj->friction * time);
        if (obj->speedX > 0) obj->speedX -= abs(sin(Angle) * obj->friction * time);
    }
    if (abs(obj->speedY) < abs(cos(Angle) * obj->friction * time)) obj->speedY = 0;
    else
    {
        if (obj->speedY < 0) obj->speedY += abs(cos(Angle) * obj->friction * time);
        if (obj->speedY > 0) obj->speedY -= abs(cos(Angle) * obj->friction * time);
    }

    obj->speed = sqrt(obj->speedX * obj->speedX + obj->speedY * obj->speedY);

    if (obj->speed > obj->maxSpeed)
    {
        obj->speed = obj->maxSpeed;
    }

    if (obj->speedY != 0)
    {
        Angle = atan(obj->speedX / obj->speedY);
        if (obj->speedY < 0) Angle += acos(-1);
        obj->speedX = sin(Angle) * obj->speed;
        obj->speedY = cos(Angle) * obj->speed;
    }

    //Angle
    if (obj->targetObject == nullptr)
    {
        if (rotateLeftKey)
        {
            obj->speedAngle -= obj->accelerationAngle * time;
        }
        if (rotateRightKey)
        {
            obj->speedAngle += obj->accelerationAngle * time;
        }
    }
    else
    {
        //aim...
    }

    if (obj->speedAngle < 0) obj->speedAngle += obj->frictionAngle * time;
    if (obj->speedAngle > 0) obj->speedAngle -= obj->frictionAngle * time;
    if (abs(obj->speedAngle) <= obj->frictionAngle * time) obj->speedAngle = 0;

    if (abs(obj->speedAngle) > obj->maxSpeedAngle)
    {
        if (obj->speedAngle < 0) obj->speedAngle = -obj->maxSpeedAngle;
        if (obj->speedAngle > 0) obj->speedAngle = obj->maxSpeedAngle;
    }
}

void BaseMouseControl(BaseObject* obj)
{
    bool upKey = false, downKey = false, leftKey = false, rightKey = false, rotateLeftKey = false, rotateRightKey = false;
    if (Input::isDown(KeyCode::W))
    {
        upKey = true;
    }
    if (Input::isDown(KeyCode::S))
    {
        downKey = true;
    }
    if (Input::isDown(KeyCode::A))
    {
        leftKey = true;
    }
    if (Input::isDown(KeyCode::D))
    {
        rightKey = true;
    }
    if (Input::isDown(KeyCode::Q))
    {
        rotateLeftKey = true;
    }
    if (Input::isDown(KeyCode::E))
    {
        rotateRightKey = true;
    }
    //Move
    double time = (double)Time::getDeltaTime();
    float Angle = obj->angle * acos(-1) / 180;
    if (upKey)
    {
        obj->speedY += cos(Angle) * obj->acceleration * time;
        obj->speedX += sin(Angle) * obj->acceleration * time;
    }
    if (downKey)
    {
        obj->speedY -= cos(Angle) * obj->acceleration * time;
        obj->speedX -= sin(Angle) * obj->acceleration * time;
    }
    if (leftKey)
    {
        obj->speedY += sin(Angle) * obj->acceleration * time;
        obj->speedX -= cos(Angle) * obj->acceleration * time;
    }
    if (rightKey)
    {
        obj->speedY -= sin(Angle) * obj->acceleration * time;
        obj->speedX += cos(Angle) * obj->acceleration * time;
    }

    if (abs(obj->speedX) < abs(sin(Angle) * obj->friction * time)) obj->speedX = 0;
    else
    {
        if (obj->speedX < 0) obj->speedX += abs(sin(Angle) * obj->friction * time);
        if (obj->speedX > 0) obj->speedX -= abs(sin(Angle) * obj->friction * time);
    }
    if (abs(obj->speedY) < abs(cos(Angle) * obj->friction * time)) obj->speedY = 0;
    else
    {
        if (obj->speedY < 0) obj->speedY += abs(cos(Angle) * obj->friction * time);
        if (obj->speedY > 0) obj->speedY -= abs(cos(Angle) * obj->friction * time);
    }

    obj->speed = sqrt(obj->speedX * obj->speedX + obj->speedY * obj->speedY);

    if (obj->speed > obj->maxSpeed)
    {
        obj->speed = obj->maxSpeed;
    }

    if (obj->speedY != 0)
    {
        Angle = atan(obj->speedX / obj->speedY);
        if (obj->speedY < 0) Angle += acos(-1);
        obj->speedX = sin(Angle) * obj->speed;
        obj->speedY = cos(Angle) * obj->speed;
    }

    //Angle
    if (obj->targetObject == nullptr)
    {
        float mouseX = Input::getMouseX();
        float mouseY = Input::getMouseY();
        float x = mouseX - obj->winX;
        float y = obj->winY - mouseY;
        float change = sqrt(x * x + y * y);
        x /= change; y /= change;
        float angle = acos(y);
        if (x < 0) angle = -angle;
        float tragetAngle = angle / acos(-1) * 180;
        float changeAngle = tragetAngle - obj->angle;
        if (changeAngle < -180)
        {
            changeAngle += 360;
            obj->angle -= 360;
        }
        if (changeAngle > 180)
        {
            changeAngle -= 360;
            obj->angle += 360;
        }

        float h = obj->speedAngle / (obj->frictionAngle * time);

        if (abs(obj->speedAngle * h) < abs(changeAngle))
        {
            if (changeAngle > 0) obj->speedAngle += obj->frictionAngle * time;
            if (changeAngle < 0) obj->speedAngle -= obj->frictionAngle * time;
        }
    }
    else
    {
        //aim...
    }

    if (abs(obj->speedAngle) <= obj->frictionAngle * time) obj->speedAngle = 0;

    if (abs(obj->speedAngle) > obj->maxSpeedAngle)
    {
        if (obj->speedAngle < 0) obj->speedAngle = -obj->maxSpeedAngle;
        if (obj->speedAngle > 0) obj->speedAngle = obj->maxSpeedAngle;
    }
}

void BaseFireControl(BaseObject* obj)
{
    float time = Time::getDeltaTime();
    if (obj->tempTime <= obj->lntervalTime)
        obj->tempTime += time;
    else obj->tempTime = obj->lntervalTime;
    if (Input::isDown(MouseCode::Left))
    {
        // std::cout << tempTime << '\n';
        if (obj->tempTime >= obj->lntervalTime)
        {
            MusicPlayer::play("./Bonus/sfx_laser1.wav");
            obj->tempTime -= obj->lntervalTime;
            float baseSpeedX = 0.0f;
            float baseSpeedY = 0.0f;
            if (obj->followFather)
            {
                baseSpeedX = obj->speedX;
                baseSpeedY = obj->speedY;
                if (obj->fatherObject != nullptr)
                {
                    baseSpeedX = obj->fatherObject->speedX;
                    baseSpeedY = obj->fatherObject->speedY;
                }
            }
            obj->childTrueAngle = obj->angle + obj->childAngle;
            float Angle = obj->angle * acos(-1) / 180;
            float changeX = 0, changeY = 0;
            changeY += cos(Angle) * obj->childY;
            changeX += sin(Angle) * obj->childY;
            changeY -= sin(Angle) * obj->childX;
            changeX += cos(Angle) * obj->childX;
            float newX = obj->x + changeX;
            float newY = obj->y + changeY;
            BaseObject child_ = BaseObject(obj->childObject);
            // child_.baseSprite = childObject->baseSprite;
            // child_.DeepCopy(childObject);
            child_.angle = obj->childTrueAngle;
            child_.x = newX;
            child_.y = newY;
            child_.baseColor = obj->baseColor;
            Angle = child_.angle * acos(-1) / 180;
            child_.speedX = sin(Angle) * obj->childSpeed + baseSpeedX;
            child_.speedY = cos(Angle) * obj->childSpeed + baseSpeedY;

            child_.Init();
            obj->targetSprite->addChild(child_.baseObject);
            obj->childlist.push_back(new BaseObject(child_));
            // std::cout << "PUSH\n";
            Timer::add([=]()
                {
                    // std::cout << "REMOVE CHILD\n";
                    obj->targetSprite->removeChild(child_.baseObject);
                    obj->childlist.pop_front();
                }, obj->lifeTime, 1);
        }
    }
}

void BaseParticle(BaseObject* obj)
{
    float time = Time::getDeltaTime();
    obj->tempTime += time;
    if (obj->tempTime >= obj->lntervalTime)
    {
        obj->tempTime -= obj->lntervalTime;
        float baseSpeedX = 0.0f;
        float baseSpeedY = 0.0f;
        if (obj->followFather)
        {
            baseSpeedX = obj->speedX;
            baseSpeedY = obj->speedY;
            if (obj->fatherObject != nullptr)
            {
                baseSpeedX = obj->fatherObject->speedX;
                baseSpeedY = obj->fatherObject->speedY;
            }
        }
        obj->childTrueAngle = obj->angle + obj->childAngle;
        float Angle = obj->angle * acos(-1) / 180;
        float changeX = 0, changeY = 0;
        changeY += cos(Angle) * obj->childY;
        changeX += sin(Angle) * obj->childY;
        changeY -= sin(Angle) * obj->childX;
        changeX += cos(Angle) * obj->childX;
        float newX = obj->x + changeX;
        float newY = obj->y + changeY;
        BaseObject child_ = BaseObject(obj->childObject);
        child_.angle = obj->childTrueAngle;
        child_.x = newX;
        child_.y = newY;
        child_.baseColor = obj->baseColor;
        Angle = child_.angle * acos(-1) / 180;
        child_.speedX = sin(Angle) * obj->childSpeed + baseSpeedX;
        child_.speedY = cos(Angle) * obj->childSpeed + baseSpeedY;

        child_.Init();
        obj->targetSprite->addChild(child_.baseObject);
        obj->childlist.push_back(new BaseObject(child_));
        Timer::add([=]()
            {
                // std::cout << "REMOVE CHILD\n";
                obj->targetSprite->removeChild(child_.baseObject);
                obj->childlist.pop_front();
            }, obj->lifeTime, 1);
    }
}