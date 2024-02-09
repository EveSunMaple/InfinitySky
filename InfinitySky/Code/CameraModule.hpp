#pragma once
#include <string>
#include <easy2d/easy2d.h>
#include "SceneControl.hpp"

// 摄像机类
class Camera
{
public:
    Camera(BaseObject* targetObject = nullptr,
        float x = 0, float y = 0, float width = 960.0f, float height = 640.0f,
        float speedX = 1.0f, float speedY = 1.0f, float maxSpeed = 10,
        float responseLX = 150.0f, float responseLY = 150.0f, float responseRX = 150.0f, float responseRY = 150.0f,
        float ctrlLX = 0, float ctrlLY = 0, float ctrlRX = 0, float ctrlRY = 0)
        : targetObject(targetObject), x(x), y(y), width(width), height(height),
        speedX(speedX), speedY(speedY), maxSpeed(maxSpeed), centerX(width / 2), centerY(height / 2),
        responseLX(responseLX), responseLY(responseLY), responseRX(responseRX), responseRY(responseRY),
        ctrlLX(ctrlLX), ctrlLY(ctrlLY), ctrlRX(ctrlRX), ctrlRY(ctrlRY)
    {
        // 其他初始化逻辑
    }
    ~Camera()
    {
        if (targetObject != nullptr) delete targetObject;
    }
    void FollowTarget()
    {
        float posX = targetObject->x;
        float posY = targetObject->y;
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
        if (posX > rx)
        {
            x = posX - centerX - responseLX;
        }
        if (posY < ry)
        {
            y = posY + centerY + responseLY;
        }

        if (ctrlLX != 0 || ctrlLY != 0 || ctrlRX != 0 || ctrlRY != 0)
        {
            if (x + speedX < ctrlLX)
            {
                x = ctrlLX;
                speedX = 0;
            }
            if (x + speedX > ctrlRX)
            {
                x = ctrlRX;
                speedX = 0;
            }
            if (y + speedY > ctrlLY)
            {
                y = ctrlLY;
                speedY = 0;
            }
            if (y + speedY < ctrlRY)
            {
                y = ctrlRY;
                speedY = 0;
            }
        }
    }
    void ProjectObject(BaseObject& object)
    {
        object.Apply();
        object.winX = object.x - x;
        object.winY = y - object.y;
        object.baseObject->setPos(object.winX, object.winY);
        object.baseObject->setAnchor(0.5f, 0.5f);
        object.baseObject->setRotation(object.angle);
        for (auto child : object.childlist)
        {
            child->x += child->speedX;
            child->y += child->speedY;
            child->winX = child->x - x;
            child->winY = y - child->y;
            if (child->baseObject != nullptr)
            {
                child->baseObject->setPos(child->winX, child->winY);
                child->baseObject->setAnchor(0.5f, 0.5f);
                child->baseObject->setRotation(child->angle);
            }
        }
    }
    void ProjectCollision(BaseObject& object)
    {
        easy2d::ShapeMaker collisionMaker;
        collisionMaker.beginPath(object.collisionPoints[0]);
        for (size_t i = 1; i < object.collisionPoints.size(); i++)
            collisionMaker.addLine(object.collisionPoints[i]);
        collisionMaker.endPath(true);
        auto collisionShape = collisionMaker.getShape();
        auto shapeNode = easy2d::gcnew easy2d::ShapeNode(collisionShape);
        shapeNode->setOrder(100); 
        easy2d::DrawingStyle style;
        style.mode = easy2d::DrawingStyle::Mode::Round;
        style.strokeColor = easy2d::Color::GreenYellow;
        style.strokeWidth = 2.0; 
        shapeNode->setDrawingStyle(style);
        object.baseObject->addChild(shapeNode);
    }
    // Function
    void CtrlCameraMove(char up, char right, char down, char left); // 手动控制摄像机
    // ...
    // Base
    BaseObject* targetObject;                               // 指向目标对象的指针
    float x, y;                                             // 摄像头默认坐标
    float width, height;                                    // 摄像头大小
    float speedX, speedY;                                   // 摄像头移动速度
    float maxSpeed;                                         // 摄像头默认速度
    float centerX, centerY;                                 // 摄像头默认跟随中心
    float responseLX, responseLY, responseRX, responseRY;   // 摄像头默认跟随阈值
    float ctrlLX, ctrlLY, ctrlRX, ctrlRY;                   // 摄像头默认移动范围 (不限制)
};