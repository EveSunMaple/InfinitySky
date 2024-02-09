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

BaseObject* NewShip(std::string name, float x, float y, float angle)
{
    Json::Value shipData;

}