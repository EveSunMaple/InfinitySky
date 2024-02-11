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
#include "Environment.hpp"

std::mutex initMutex;  // 全局互斥锁
std::thread loadThread;// 全局线程

void StyleInit()
{
    /*  Black = 0x000000,
		Blue = 0x0000FF,
		BlueViolet = 0x8A2BE2,
		Brown = 0xA52A2A,
		Chocolate = 0xD2691E,
		DarkBlue = 0x00008B,
		DarkGray = 0xA9A9A9,
		DarkGreen = 0x006400,
		DarkOrange = 0xFF8C00,
		DarkRed = 0x8B0000,
		DarkViolet = 0x9400D3,
		ForestGreen = 0x228B22,
		Gold = 0xFFD700,
		Gray = 0x808080,
		Green = 0x008000,
		GreenYellow = 0xADFF2F,
		LightBlue = 0xADD8E6,
		LightCyan = 0xE0FFFF,
		LightGreen = 0x90EE90,
		LightGray = 0xD3D3D3,
		LightPink = 0xFFB6C1,
		LightSeaGreen = 0x20B2AA,
		LightSkyBlue = 0x87CEFA,
		LightYellow = 0xFFFFE0,
		Orange = 0xFFA500,
		OrangeRed = 0xFF4500,
		Pink = 0xFFC0CB,
		Purple = 0x800080,
		Red = 0xFF0000,
		Silver = 0xC0C0C0,
		SkyBlue = 0x87CEEB,
		Snow = 0xFFFAFA,
		Violet = 0xEE82EE,
		Wheat = 0xF5DEB3,
		White = 0xFFFFFF,
		WhiteSmoke = 0xF5F5F5,
		Wood = 0xDEB887,
		Yellow = 0xFFFF00,
		YellowGreen = 0x9ACD32*/
    modeMap["Solid"] = easy2d::DrawingStyle::Mode::Solid;
    modeMap["Round"] = easy2d::DrawingStyle::Mode::Round;
    modeMap["Fill"] = easy2d::DrawingStyle::Mode::Fill;
    lineJoinMap["None"] = easy2d::LineJoin::None;
    lineJoinMap["Miter"] = easy2d::LineJoin::Miter;
    lineJoinMap["Bevel"] = easy2d::LineJoin::Bevel;
    lineJoinMap["Round"] = easy2d::LineJoin::Round;
    Json::Value Data = readJsonFromFile(STYLES);
    const Json::Value styleList = Data["styles"];
    for (const Json::Value style : styleList)
    {
        easy2d::DrawingStyle newStyle;
        newStyle.mode = modeMap[style["mode"].asCString()];
        newStyle.lineJoin = lineJoinMap[style["lineJoin"].asCString()];
        newStyle.fillColor = easy2d::Color::Value(hexStringToInt(style["fillColor"].asCString()));
        newStyle.strokeColor = easy2d::Color::Value(hexStringToInt(style["strokeColor"].asCString()));
        newStyle.strokeWidth = style["strokeWidth"].asFloat();
        styleMap[style["name"].asCString()] = newStyle;
    }
}

void ActionInit()
{
    Json::Value Data = readJsonFromFile(ACTIONS);
    const Json::Value actionList = Data["actions"];
    for (const Json::Value action : actionList)
    {
        easy2d::Sequence* totalAction = nullptr;
        for (const Json::Value actionIn : action["info"])
        {
            int operation = actionIn["connect"].asInt();
            Json::Value Info = actionIn["info"];
            std::string name = actionIn["name"].asCString();
            easy2d::Sequence* nowAction = nullptr;
            if (name == "MoveTo") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::MoveTo(actionIn["info"][0].asFloat(), easy2d::Point(100, 200)));
            else if (name == "MoveBy") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::MoveBy(Info[0].asFloat(), easy2d::Vector2(Info[1].asFloat(), Info[2].asFloat())));
            else if (name == "ScaleTo")nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::ScaleTo(Info[0].asFloat(), Info[1].asFloat()));
            else if (name == "ScaleBy") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::ScaleBy(Info[0].asFloat(), Info[1].asFloat()));
            else if (name == "OpacityTo") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::OpacityTo(Info[0].asFloat(), Info[1].asFloat()));
            else if (name == "OpacityBy") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::OpacityBy(Info[0].asFloat(), Info[1].asFloat()));
            else if (name == "FadeIn") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::FadeIn(Info[0].asFloat()));
            else if (name == "FadeOut") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::FadeOut(Info[0].asFloat()));
            else if (name == "RotateTo") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::RotateTo(Info[0].asFloat(), Info[1].asFloat()));
            else if (name == "RotateBy") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::RotateBy(Info[0].asFloat(), Info[1].asFloat()));
            // else if (name == "JumpTo") nowAction = dynamic_cast<easy2d::Sequence*>(new easy2d::JumpTo(Info[0].asFloat(), Point(Info[1].asFloat(), Info[2].asFloat())));
            // else if (name == "JumpBy") nowAction = dynamic_cast<easy2d::Sequence*>(new easy2d::JumpBy(Info[0].asFloat(), Point(Info[1].asFloat(), Info[2].asFloat())));
            else if (name == "Delay") nowAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::Delay(Info[0].asFloat()));
            else nowAction = nullptr;
            // 衔接动画
            if (totalAction == nullptr || operation == 0) totalAction = nowAction;
            else if (operation == 1) totalAction = dynamic_cast<easy2d::Sequence*>(easy2d::gcnew easy2d::Spawn({ totalAction, nowAction }));
            else if (operation == 2) totalAction = easy2d::gcnew easy2d::Sequence({ totalAction, nowAction });
        }
        totalAction->retain();
        actionMap[action["name"].asCString()] = totalAction;
    }
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

void GameInit();
int nowLoadNumber;
int totalLoadNumber;
int initLoadNumber = 0;
Json::Value newData;
Json::Value readData;
easy2d::ShapeNode* locateBar;
easy2d::ShapeNode* loadbar;
easy2d::Sprite* startUp;
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
    float loadBarValue = (float)now / (float)total * 100.0f;
    if (loadBarValue > 100.0f) loadBarValue = 100.0f;
    std::cout << "\r\033[1;32m INFO \033[0m进度：[";
    for (i = 1; i < loadBarValue - 5; i += 10) std::cout << GREEN_TEXT << "-" << RESET_TEXT;
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
            initLoadNumber++;
            newData["resourcePath"].append(replaceBackslash(entry.path().string()));
        }
    }
}

void reLoad()
{
    WARN "正在使用重加载模块！错误可能被隐蔽" END; ENDL
    initDirectory(SRC);
    newData["totalNumber"] = initLoadNumber;
    writeJsonToFile(DATA, newData);
}

void traverseDirectory(const std::string& directoryPath)
{
    for (const auto& entry : fs::directory_iterator(directoryPath))
    {
        if (entry.is_directory())
        {
            traverseDirectory(entry.path().string());
        }
        else
        {
            LOCK
                nowLoadNumber++;
            if (entry.path().extension() == ".png")
            {
                spriteName = entry.path().stem().string();
                spritePath = replaceBackslash(entry.path().string());
                // spriteMap[spriteName] = gcnew easy2d::Sprite(spritePath);;
            }
            if (entry.path().extension() == ".wav")
            {
            }
            coutProgressBar(totalLoadNumber, nowLoadNumber);
        }
    }
}

void checkOver(int val)
{
    LOCK check = val;
}

void GameInit()
{
    bool returnValue = false;
    std::cout << "/\\__  _\\          /'___\\ __          __/\\ \\__         /\\  _`\\ /\\ \\                                               " << std::endl;
    std::cout << "\\/_/\\ \\/     ___ /\\ \\__//\\_\\    ___ /\\_\\ \\ ,_\\  __  __\\ \\,\\L\\_\\ \\ \\/'\\   __  __                        " << std::endl;
    std::cout << "   \\ \\ \\   /' _ `\\ \\ ,__\\/\\ \\ /' _ `\\/\\ \\ \\ \\/ /\\ \\/\\ \\\\/_\\__ \\\\ \\ , <  /\\ \\/\\ \\                " << std::endl;
    std::cout << "    \\_\\ \\__/\\ \\/\\ \\ \\ \\_/\\ \\ \\/\\ \\/\\ \\ \\ \\ \\ \\_\\ \\ \\_\\ \\ /\\ \\L\\ \\ \\ \\\\`\\\\ \\ \\_\\ \\   " << std::endl;
    std::cout << "    /\\_____\\ \\_\\ \\_\\ \\_\\  \\ \\_\\ \\_\\ \\_\\ \\_\\ \\__\\\\/`____ \\\\ `\\____\\ \\_\\ \\_\\/`____ \\           " << std::endl;
    std::cout << "    \\/_____/\\/_/\\/_/\\/_/   \\/_/\\/_/\\/_/\\/_/\\/__/ `/___/> \\\\/_____/\\/_/\\/_/`/___/> \\                         " << std::endl;
    std::cout << "                                                /\\___/                   /\\___/                                         " << std::endl;
    std::cout << "                                                \\/__/                    \\/__/                                          " << std::endl;
    INFO "作者：EveSunMaple" END;
    INFO "项目地址：https://github.com/EveSunMaple/InfinitySky" END;

    // 检查项目完整性
    nowLoadNumber = 0;
    totalLoadNumber = 0;
    INFO "正在检查项目资源完整性……" END;
    totalLoadNumber = readData["resourcePath"].size();
    for (const auto& resource : readData["resourcePath"])
    {
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
            ERR "资源文件缺失或已被移动，请根据报错查找丢失文件！" END;
            return;
        }
    INFO "项目资源检查完毕✅" END;
    Sleep(1);
    checkOver(1);

    // 载入所有资源文件
    nowLoadNumber = 0;
    // totalLoadNumber = readData["totalNumber"].asInt();
    INFO "正在载入所有资源文件" END;
    traverseDirectory("./Source");
    ENDL
        INFO "资源文件载入完毕✅" END;
    Sleep(1);
    checkOver(2);

    // 初始化 Json
    ShipData = readJsonFromFile(SHIPS);
    BulletData = readJsonFromFile(BULLETS);
    WeaponData = readJsonFromFile(WEAPONS);
    ParticleData = readJsonFromFile(PARTICLES);
    Sleep(1);
    checkOver(3);
}

class LoadPage : public easy2d::Sprite
{
public:
    int start = 0;
    std::string musicPathIner = "NULL";  // 只能在主线程加载
    std::string spritePathIner = "NULL"; // 只能在主线程加载
    LoadPage()
    {
        // 初始化
        reLoad(); // 重加载!
        StyleInit();
        readData = readJsonFromFile(DATA);
        loadThread = std::thread(GameInit);
        startUp = easy2d::gcnew easy2d::Sprite("./Source/PNG/Maps/icon.png");
        startUp->setAnchor(0.5f, 0.5f);
        startUp->setOpacity(0.0f);
        startUp->setPos(480, 320);
        this->addChild(startUp);
        locateBar = easy2d::gcnew easy2d::ShapeNode(easy2d::Shape::createRoundedRect(easy2d::Rect(easy2d::Point(), easy2d::Size(800, 40)), easy2d::Vector2(10, 10)));
        locateBar->setDrawingStyle(styleMap["hollowBarStyle"]);
        locateBar->setAnchor(0.5f, 0.5f);
        locateBar->setPos(480, 420);
        locateBar->setOrder(0);
        this->addChild(locateBar);
        loadbar = easy2d::gcnew easy2d::ShapeNode(easy2d::Shape::createRoundedRect(easy2d::Rect(easy2d::Point(), easy2d::Size(800, 40)), easy2d::Vector2(10, 10)));
        loadbar->setDrawingStyle(styleMap["loadBarStyle"]);
        loadbar->setPos(0, 0);
        locateBar->addChild(loadbar);
        tip = easy2d::gcnew easy2d::Text("初始化中……");
        tip->setPos(0, -100);
        locateBar->addChild(tip);
        rate = easy2d::gcnew easy2d::Text("0%");
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
                auto spriteTemp = easy2d::gcnew easy2d::Sprite(spritePath);
                spriteTemp->retain();
                spriteMap[spriteName] = spriteTemp;
                spritePathIner = spritePath;
            }
        }
        if (check == 2)
        {
            tip->setText("正在读入 Json 文件");
        }
        if (check == 3)
        {
            easy2d::OpacityTo* opacityTo = easy2d::gcnew easy2d::OpacityTo(1.0f, 1.0f);
            easy2d::OpacityTo* _opacityTo = easy2d::gcnew easy2d::OpacityTo(0.5f, 0.0f);
            locateBar->runAction(_opacityTo->clone());
            loadbar->runAction(_opacityTo->clone());
            rate->runAction(_opacityTo->clone());
            tip->runAction(_opacityTo->clone());
            tip->setText("已完成！");
            startUp->runAction(opacityTo);
        }

    }
};