#pragma once
#include <easy2d/easy2d.h>
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <thread>
#include <string>
#include <mutex>
#include <ctime>
#include "json/json.h"
#include "Environment.hpp"
#include "SceneControl.hpp"
#include "ObjectOperation.hpp"
// 产生粒子，这个应该分配进飞船的ChildList中
BaseObject* NewParticle(BaseObject* father, const std::string& name, float fx, float fy)
{
    const Json::Value object = ParticleData["particles"][name];
    if (object == NULL) return nullptr;
    BaseObject* newObject = new BaseObject(5, false, nullptr, spriteMap[name], nullptr, nullptr, false);
    // sprite
    newObject->x = father->x + fx;
    newObject->y = father->y + fy;
    if (object["sprite"]["ifFollow"].asBool())
    {
        // 如果跟随父亲，就添加初速度
        newObject->speedX = father->speedX;
        newObject->speedY = father->speedY;
    }
    // 粒子真实的角度
    newObject->trueAngle = object["sprite"]["angle"].asFloat() + father->trueAngle;
    float Angle = Remainder(newObject->trueAngle, 360) * (float)acos(-1) / 180;
    // 通过发射初速度换算
    newObject->speedX += object["sprite"]["speed"].asFloat() * sin(Angle);
    newObject->speedY += object["sprite"]["speed"].asFloat() * cos(Angle);
    // variable
    newObject->variable["liftTime"] = object["sprite"]["maxForce"].asFloat();
    // shape
    if (object["sprite"]["circle"]["ifOpen"].asBool())
    {
        // 不写了！
    }
    else if (object["sprite"]["polygon"]["ifOpen"].asBool())
    {
        // 不写了！
    }
    else
    {
        ERR "存在未定义形状的粒子！" END;
        return nullptr;
    }
    return  newObject;
}
// 发射子弹，father 中有需要的内容，此Object应该被添加进其父亲的ChildList
BaseObject* NewBullet(BaseObject* father, const std::string& name)
{
    const Json::Value object = BulletData["bullets"][name];
    if (object == NULL) return nullptr;
    BaseObject* newObject = new BaseObject(5, false, nullptr, spriteMap[name], nullptr, nullptr, false);
    // sprite
    float fireSpeed = father->variable["fireSpeed"];
    float fireAngle = father->variable["fireAngle"];
    newObject->mass = object["sprite"]["mass"].asFloat();
    newObject->health = object["sprite"]["health"].asFloat();
    newObject->maxSpeed = object["sprite"]["maxSpeed"].asFloat();
    newObject->maxSpeedAngle = object["sprite"]["maxSpeedAngle"].asFloat();
    newObject->friction = object["sprite"]["friction"].asFloat();
    newObject->frictionAngle = object["sprite"]["frictionAngle"].asFloat();
    // 注意这里是写的tureAngle，因为子弹不从属与任何一个物体，Weapon只是负责渲染
    newObject->trueAngle = father->trueAngle + RandomNumber(-fireAngle, fireAngle);
    float Angle = Remainder(newObject->trueAngle, 360) * (float)acos(-1) / 180;
    newObject->speedX = father->speedX + fireSpeed * sin(Angle);
    newObject->speedY = father->speedY + fireSpeed * cos(Angle);
    newObject->x = father->x;
    newObject->y = father->y;
    // variable
    newObject->variable["rotateAcceleration"] = object["sprite"]["rotateAcceleration"].asFloat();
    newObject->variable["forceAcceleration"] = object["sprite"]["forceAcceleration"].asFloat();
    newObject->variable["maxForce"] = object["sprite"]["maxForce"].asFloat();
    newObject->variable["liftTime"] = object["sprite"]["maxForce"].asFloat();
    // collisionBox
    for (const Json::Value point : object["collisionBox"])
        easy2d::Point(point[0].asFloat(), point[1].asFloat());
    newObject->Init(); // 初始化
    
    return newObject;
}
// 新建武器，这个武器应该被绑定在一个父亲节点之上
BaseObject* NewWeapon(const std::string& name, float fx, float fy, float angle)
{
    const Json::Value object = WeaponData["weapons"][name];
    if (object == NULL) return nullptr;
    BaseObject* newObject = new BaseObject(11, false, nullptr, spriteMap[name], nullptr, nullptr, false);
    // sprite
    newObject->mass = object["sprite"]["mass"].asFloat();
    newObject->health = object["sprite"]["health"].asFloat();
    newObject->maxSpeedAngle = object["sprite"]["maxSpeedAngle"].asFloat();
    newObject->frictionAngle = object["sprite"]["frictionAngle"].asFloat();
    // 这里写的是angle，是相对于父亲的角度，在Apply中换算真实角度
    newObject->angle = angle;
    newObject->fatherX = fx;
    newObject->fatherY = fy;
    // variable
    newObject->variable["fireSpeed"] = object["sprite"]["fireSpeed"].asFloat();
    newObject->variable["fireAngle"] = object["sprite"]["fireAngle"].asFloat();
    newObject->variable["rotateAcceleration"] = object["sprite"]["rotateAcceleration"].asFloat();
    newObject->variable["target"] = object["sprite"]["target"].asBool();
    
    return newObject;
}
// 新建一艘独立的船只
BaseObject* NewShip(const std::string& name, float x, float y, float angle)
{
    const Json::Value object = ShipData["ships"][name];
    if (object == NULL) return nullptr;
    BaseObject* newObject = new BaseObject(10, false, nullptr, spriteMap[name], nullptr, nullptr, false, x, y);
    // sprite
    newObject->mass = object["sprite"]["mass"].asFloat();
    newObject->health = object["sprite"]["health"].asFloat();
    newObject->maxSpeed = object["sprite"]["maxSpeed"].asFloat();
    newObject->maxSpeedAngle = object["sprite"]["maxSpeedAngle"].asFloat();
    newObject->friction = object["sprite"]["friction"].asFloat();
    newObject->frictionAngle = object["sprite"]["frictionAngle"].asFloat();
    // angle = tureAngle
    newObject->angle = angle;
    newObject->x = x;
    newObject->y = y;
    // variable
    newObject->variable["rotateAcceleration"] = object["sprite"]["rotateAcceleration"].asFloat();
    newObject->variable["forceAcceleration"] = object["sprite"]["forceAcceleration"].asFloat();
    newObject->variable["maxForce"] = object["sprite"]["maxForce"].asFloat();
    // collisionBox
    for (const Json::Value point : object["collisionBox"])
        easy2d::Point(point[0].asFloat(), point[1].asFloat());
    newObject->Init(); // 初始化
    // weapons
    for (const Json::Value weapon : object["weapons"])
    {
        BaseObject* temp = NewWeapon(weapon["name"].asString(), weapon["position"][0].asFloat(), weapon["position"][0].asFloat(), weapon["angle"].asFloat());
        newObject->childlist.push_back(temp);
        temp->fatherObject = newObject;
        temp->Init();
    }
    // particles
    for (const Json::Value particle : object["particles"])
    {
        BaseObject* temp = NewWeapon(particle["name"].asString(), particle["position"][0].asFloat(), particle["position"][0].asFloat(), particle["angle"].asFloat());
        newObject->childlist.push_back(temp);
        temp->fatherObject = newObject;
        temp->Init();
    }
}