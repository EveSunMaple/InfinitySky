#pragma once
#include <easy2d/easy2d.h>
#include <filesystem>
#include <iostream>
#include "SceneControl.hpp"
#include "Environment.hpp"

BaseObject* NewParticle(BaseObject* father, const std::string& name, float fx, float fy);
BaseObject* NewBullet(BaseObject* father, const std::string& name);
BaseObject* NewWeapon(const std::string& name, float fx, float fy, float angle);
BaseObject* NewShip(const std::string& name, float x, float y, float angle);
