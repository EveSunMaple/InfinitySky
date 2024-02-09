#pragma once
#include <easy2d/easy2d.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include <tuple>
#include "json/json.h"
#include "SceneControl.hpp"

// 取余
float Remainder(float val, int in)
{
    if (val < 0) return Remainder(val + in, in);
    if (val > in) return Remainder(val - in, in);
    return val;
}

void BaseObject::SolePhysics()
{
    float time = (float)easy2d::Time::getDeltaTime();
    float Angle = angle * (float)acos(-1) / 180;
    // 移动摩擦
    speed = sqrt(speedX * speedX + speedY * speedY);
    if (speed < friction * time) speed = 0;
    else speed -= friction * time;
    // 检查是否超过限速
    if (speed > maxSpeed)
    {
        speed = maxSpeed;
    }
    // 重置速度
    if (speedY != 0)
    {
        Angle = atan(speedX / speedY);
        if (speedY < 0) Angle += (float)acos(-1);
        speedX = sin(Angle) * speed;
        speedY = cos(Angle) * speed;
    }
    else
    {
        if (speedX < 0) speedX = -speed;
        if (speedX > 0) speedX = speed;
    }
    // 旋转摩擦
    if (speedAngle < 0) speedAngle += frictionAngle * time;
    if (speedAngle > 0) speedAngle -= frictionAngle * time;
    if (abs(speedAngle) <= frictionAngle * time) speedAngle = 0;
    // 检查是否超过限速
    if (abs(speedAngle) > maxSpeedAngle)
    {
        if (speedAngle < 0) speedAngle = -maxSpeedAngle;
        if (speedAngle > 0) speedAngle = maxSpeedAngle;
    }
}
void BaseObject::ApplyForce(float val, float angle_)
{
    float acc = val / mass;
    ApplyAcceleration(acc, angle_);
}
void BaseObject::ApplyGlobalForce(float val, float angle_)
{
    float acc = val / mass;
    ApplyGlobalAcceleration(acc, angle_);
}
void BaseObject::ApplyVelocity(float val, float angle_)
{
    float time = (float)easy2d::Time::getDeltaTime();
    float Angle = Remainder(angle + angle_, 360) * (float)acos(-1) / 180;
    speedY = cos(Angle) * val * time;
    speedX = sin(Angle) * val * time;
}
void BaseObject::ApplyGlobalVelocity(float val, float angle_)
{
    float time = (float)easy2d::Time::getDeltaTime();
    float Angle = Remainder(angle_, 360) * (float)acos(-1) / 180;
    speedY = cos(Angle) * val * time;
    speedX = sin(Angle) * val * time;
}
void BaseObject::ApplyAcceleration(float val, float angle_)
{
    float time = (float)easy2d::Time::getDeltaTime();
    float Angle = Remainder(angle + angle_, 360) * (float)acos(-1) / 180;
    speedY += cos(Angle) * val * time;
    speedX += sin(Angle) * val * time;
}
void BaseObject::ApplyGlobalAcceleration(float val, float angle_)
{
    float time = (float)easy2d::Time::getDeltaTime();
    float Angle = Remainder(angle_, 360) * (float)acos(-1) / 180;
    speedY += cos(Angle) * val * time;
    speedX += sin(Angle) * val * time;
}