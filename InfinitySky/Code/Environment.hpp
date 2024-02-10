#pragma once
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
#include "SceneControl.hpp"

// ANSI color codes
#define GREEN_TEXT "\033[1;32m"
#define RED_TEXT "\033[1;31m"
#define BLUE_TEXT "\033[1;34m"
#define PURPLE_TEXT "\033[1;35m"
#define ORANGE_TEXT "\033[1;33m"
#define RESET_TEXT "\033[0m"

#define INFO std::cout << "\033[1;32m INFO \033[0m" <<
#define WARN std::cout << "\033[1;33m WARN \033[0m" <<
#define ERR std::cout << "\033[1;31m ERROR \033[0m" <<
#define END << "\033[0m \n"

#define ENDL std::cout << std::endl;

// Json 
#define SRC "./Source"
#define DATA "./Data/Data.json"
#define SHIPS "./Data/Ships.json"

#define LOCK std::lock_guard<std::mutex> lock(initMutex);

// 场景类
easy2d::Scene* loadScene;

// 图片类
std::unordered_map<std::string, easy2d::Sprite*> spriteMap;
easy2d::Sprite* particle1_blue;
easy2d::Sprite* particle1_green;
easy2d::Sprite* particle1_red;
easy2d::Sprite* particle1_white;
easy2d::Sprite* particle1_yellow;

// 样式类
easy2d::DrawingStyle hollowBarStyle;
easy2d::DrawingStyle loadBarStyle;

// 动画类
std::unordered_map<std::string, easy2d::Sequence*> actionMap;
easy2d::Sequence* action_particle1; // 基本粒子动画（轨迹）
easy2d::Sequence* action_particle2; // 基本粒子动画（航迹）
easy2d::Sequence* action_particle3; // 基本粒子动画（引擎）

// 物体类
std::unordered_map<std::string, BaseObject*> objectMap;
BaseObject* playerShip1_blue_object;

namespace fs = std::filesystem;

std::string replaceBackslash(const std::string& input) 
{
    std::string result = input;
    for (char& c : result) 
    {
        if (c == '\\') 
        {
            c = '/';
        }
    }
    return result;
}

Json::Value readJsonFromFile(const std::string& filename) 
{
    std::ifstream inputFile(filename);
    Json::Value jsonData;
    inputFile >> jsonData;
    inputFile.close();
    return jsonData;
}

void writeJsonToFile(const std::string& filename, Json::Value data)
{
    std::ofstream outputFile(filename);
    outputFile << data;
    outputFile.close();
}