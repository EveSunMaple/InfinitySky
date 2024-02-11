#pragma once
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <random>
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

#define LOCK std::lock_guard<std::mutex> lock(initMutex);

// Json 
#define SRC "./Source"
#define DATA "./Data/Data.json"
#define SHIPS "./Data/Ships.json"
#define STYLES "./Data/Styles.json"
#define WEAPONS "./Data/Weapons.json"
#define BULLETS "./Data/Bullets.json"
#define ACTIONS "./Data/Actions.json"
#define PARTICLES "./Data/Particles.json"

Json::Value ShipData;
Json::Value BulletData;
Json::Value WeaponData;
Json::Value ParticleData;

// 场景类
easy2d::Scene* loadScene;

// 图片类
std::unordered_map<std::string, easy2d::Sprite*> spriteMap;

// 样式类
std::unordered_map<std::string, easy2d::DrawingStyle> styleMap;
std::unordered_map<std::string, easy2d::DrawingStyle::Mode> modeMap;
std::unordered_map<std::string, easy2d::LineJoin> lineJoinMap;

// 动画类
std::unordered_map<std::string, easy2d::Sequence*> actionMap;

// 物体类
std::unordered_map<std::string, BaseObject*> objectMap;
BaseObject* playerShip1_blue_object;

namespace fs = std::filesystem;

// 将16进制字符串转换为整数
int hexStringToInt(const std::string& hexString) 
{
    std::istringstream converter(hexString);
    int result;
    converter >> std::hex >> result;
    return result;
}

// 生成指定范围内的随机实数
float RandomNumber(float min, float max) 
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<float> distribution(min, max);
    return distribution(gen);
}

// 取余
float Remainder(float val, int in)
{
    if (val < 0) return Remainder(val + in, in);
    if (val > in) return Remainder(val - in, in);
    return val;
}

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