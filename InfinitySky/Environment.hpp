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
#define ERROR std::cout << "\033[1;31m ERROR \033[0m" <<
#define END << "\033[0m \n"

#define ENDL std::cout << std::endl;

#define LOCK std::lock_guard<std::mutex> lock(initMutex);

std::mutex initMutex;  // 全局互斥锁
std::thread loadThread;// 全局线程

using namespace easy2d;

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

void StyleInit()
{
    hollowBarStyle.mode = DrawingStyle::Mode::Round;
    hollowBarStyle.fillColor = Color::White;
    hollowBarStyle.strokeColor = Color::White;
    hollowBarStyle.strokeWidth = 2.0;
    hollowBarStyle.lineJoin = LineJoin::Round;

    loadBarStyle.mode = DrawingStyle::Mode::Solid;
    loadBarStyle.fillColor = Color::White;
    loadBarStyle.strokeColor = Color::Black;
    loadBarStyle.strokeWidth = 0.0;
    loadBarStyle.lineJoin = LineJoin::Miter;
}

void ActionInit()
{
    auto scaleTo = gcnew ScaleTo(0.8, 0.0f);
    auto opacityTo = gcnew OpacityTo(0.8, 0.0f);
    auto two = gcnew Spawn({ scaleTo, opacityTo });
    scaleTo = gcnew ScaleTo(0, 0.1f);
    action_particle1 = gcnew Sequence({ scaleTo, two });
    action_particle1->retain();

    scaleTo = gcnew ScaleTo(0.5f, 0.12f);
    opacityTo = gcnew OpacityTo(0.4f, 0.0f);
    two = gcnew Spawn({ scaleTo, opacityTo });
    scaleTo = gcnew ScaleTo(0, 0.5f);
    action_particle2 = gcnew Sequence({ scaleTo, two });
    action_particle2->retain();

    scaleTo = gcnew ScaleTo(0.4f, 0.15f);
    opacityTo = gcnew OpacityTo(0.4f, 0.0f);
    two = gcnew Spawn({ scaleTo, opacityTo });
    scaleTo = gcnew ScaleTo(0, 0.4f);
    action_particle3 = gcnew Sequence({ scaleTo, two });
    action_particle3->retain();
}

void MusicInit()
{
    easy2d::MusicPlayer::preload("./Source/Bonus/sfx_laser1.wav");
    easy2d::MusicPlayer::preload("./Source/Bonus/sfx_laser2.wav");
    easy2d::MusicPlayer::preload("./Source/Bonus/sfx_lose.wav");
    easy2d::MusicPlayer::preload("./Source/Bonus/sfx_shieldDown.wav");
    easy2d::MusicPlayer::preload("./Source/Bonus/sfx_shieldUp.wav");
    easy2d::MusicPlayer::preload("./Source/Bonus/sfx_twoTone.wav");
    easy2d::MusicPlayer::preload("./Source/Bonus/sfx_zap.wav");
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

void GameInit();
int nowLoadNumber;
int totalLoadNumber;
Json::Value newData;
Json::Value readData;
easy2d::ShapeNode* locateBar;
easy2d::ShapeNode* loadbar;
easy2d::Text* tip;
easy2d::Text* rate;

//public 加载线程与主线程交流的变量（需要上锁）
int check = 0;
int nowLoadLink;
std::string loadInfo = "加载中";
std::string musicPath = "NULL";  // 只能在主线程加载
std::string spritePath = "NULL"; // 只能在主线程加载
std::string musicName = "NULL";  // 只能在主线程加载
std::string spriteName = "NULL"; // 只能在主线程加载

void coutProgressBar(int total, int now)
{
    int i = 0; 
    nowLoadLink = now;
    float loadBarValue = (float)now / (float)total * 100.0;
    std::cout << "\r 进度：[";
    for (i = 1; i < loadBarValue; i += 10) std::cout << GREEN_TEXT << "-" << RESET_TEXT;
    for (int j = 100; j > i; j -= 10) std::cout << RED_TEXT << "-" << RESET_TEXT;
    std::cout << "] " << (int)loadBarValue << "%";
}

void initDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            initDirectory(entry.path().string());
        }
        else
        {
            LOCK
            newData["resourcePath"].append(replaceBackslash(entry.path().string()));
        }
    }
}

void traverseDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        Sleep(1);
        if (entry.is_directory())
        {
            traverseDirectory(entry.path().string());
        }
        else
        {
            LOCK
            if (entry.path().extension() == ".png")
            {
                nowLoadNumber++;
                spriteName = entry.path().stem().string();
                spritePath = replaceBackslash(entry.path().string());
            }
            if (entry.path().extension() == ".wav")
            {
                nowLoadNumber++;
            }
            coutProgressBar(totalLoadNumber, nowLoadNumber);
        }
    }
}

Json::Value readJsonFromFile(const std::string& filename) 
{
    std::ifstream inputFile(filename);
    Json::Value jsonData;
    inputFile >> jsonData;
    inputFile.close();
    return jsonData;
}

void checkOver(int val)
{
    LOCK check = val;
}

void GameInit()
{
    bool returnValue = false;
    std::cout << "/\\__  _\\          /'___\\ __          __/\\ \\__         /\\  _`\\ /\\ \\                  " << std::endl;
    std::cout << "\\/_/\\ \\/     ___ /\\ \\__//\\_\\    ___ /\\_\\ \\ ,_\\  __  __\\ \\,\\L\\_\\ \\ \\/'\\   __  __     " << std::endl;
    std::cout << "   \\ \\ \\   /' _ `\\ \\ ,__\\/\\ \\ /' _ `\\/\\ \\ \\ \\/ /\\ \\/\\ \\\\/_\\__ \\\\ \\ , <  /\\ \\/\\ \\    " << std::endl;
    std::cout << "    \\_\\ \\__/\\ \\/\\ \\ \\ \\_/\\ \\ \\/\\ \\/\\ \\ \\ \\ \\ \\_\\ \\ \\_\\ \\ /\\ \\L\\ \\ \\ \\\\`\\\\ \\ \\_\\ \\   " << std::endl;
    std::cout << "    /\\_____\\ \\_\\ \\_\\ \\_\\  \\ \\_\\ \\_\\ \\_\\ \\_\\ \\__\\\\/`____ \\\\ `\\____\\ \\_\\ \\_\\/`____ \\  " << std::endl;
    std::cout << "    \\/_____/\\/_/\\/_/\\/_/   \\/_/\\/_/\\/_/\\/_/\\/__/ `/___/> \\\\/_____/\\/_/\\/_/`/___/> \\ " << std::endl;
    std::cout << "                                                /\\___/                   /\\___/     " << std::endl;
    std::cout << "                                                \\/__/                    \\/__/      " << std::endl;
    INFO "作者：EveSunMaple" END;
    INFO "项目地址：https://github.com/EveSunMaple/InfinitySky" END;

    // 检查项目完整性
    nowLoadNumber = 0;
    totalLoadNumber = 0;
    INFO "正在检查项目资源完整性……" END;
    totalLoadNumber = readData["resourcePath"].size();
    for (const auto& resource : readData["resourcePath"])
    {
        Sleep(1);
        nowLoadNumber++;
        if (returnValue) break;
        if (fs::exists(resource.asCString()))
        {
            LOCK
            coutProgressBar(totalLoadNumber, nowLoadNumber);
        }
        else
        {
            ENDL WARN "资源文件不存在！路径：" << PURPLE_TEXT << resource.asCString() << RESET_TEXT END;
            returnValue = true;
        }
    }
    ENDL
    if (returnValue)
    {
        checkOver(-1);
        ERROR "资源文件缺失或已被移动，请根据报错查找丢失文件！" END;
        return;
    }
    INFO "项目资源检查完毕✅" END;

    checkOver(1);

    // 载入所有资源文件
    nowLoadNumber = 0;
    totalLoadNumber = readData["totalNumber"].asInt();
    INFO "正在载入所有资源文件" END;
    traverseDirectory("./Source");
    ENDL
    INFO "资源文件载入完毕✅" END;
}

class LoadPage : public Sprite
{
public:
    int start = 0;
    std::string musicPathIner = "NULL";  // 只能在主线程加载
    std::string spritePathIner = "NULL"; // 只能在主线程加载
    LoadPage()
    {
        // 初始化
        StyleInit();
        readData = readJsonFromFile("Data.json");
        loadThread = std::thread(GameInit);
        locateBar = gcnew easy2d::ShapeNode(easy2d::Shape::createRoundedRect(Rect(Point(), Size(800, 40)), Vector2(10, 10)));
        locateBar->setDrawingStyle(hollowBarStyle);
        locateBar->setAnchor(0.5f, 0.5f);
        locateBar->setPos(480, 420);
        locateBar->setOrder(0);
        this->addChild(locateBar);
        loadbar = gcnew easy2d::ShapeNode(easy2d::Shape::createRoundedRect(Rect(Point(), Size(800, 40)), Vector2(10, 10)));
        loadbar->setDrawingStyle(loadBarStyle);
        loadbar->setPos(0, 0);
        locateBar->addChild(loadbar);
        tip = gcnew easy2d::Text("初始化中……");
        tip->setPos(0, -100);
        locateBar->addChild(tip);
        rate = gcnew easy2d::Text("0%");
        rate->setPos(700, -100);
        locateBar->addChild(rate);
    }
    void onUpdate()
    {
        if (check == 0)
        {
            float loadBarValue = (float)nowLoadLink / (float)readData["resourcePath"].size();
            loadbar->setScale(loadBarValue, 1.0f);
            tip->setText("正在检查项目资源完整性……");
            rate->setText(std::to_string(loadBarValue * 100.0) + "%");
        }
        if (check == 1)
        {
            float loadBarValue = (float)nowLoadLink / (float)readData["totalNumber"].asInt();
            loadbar->setScale(loadBarValue, 1.0f);
            tip->setText("正在加载" + spriteName);
            rate->setText(std::to_string(loadBarValue * 100.0) + "%");
            if (spritePath != spritePathIner)
            {
                auto spriteTemp = gcnew easy2d::Sprite(spritePath);
                spriteTemp->retain();
                spriteMap[spriteName] = spriteTemp;
                spritePathIner = spritePath;
            }
        }
    }
};