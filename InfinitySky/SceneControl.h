#pragma once
#include <easy2d/easy2d.h>
#include <vector>
#include <queue>
#include <tuple>

using namespace easy2d;

class BaseObject;

class ChildObject
{
public:
    // 构造函数，用于初始化对象的各种属性
    ChildObject(bool init = false, class easy2d::Sprite* targetSprite = nullptr, class easy2d::Sprite* baseSprite = nullptr, // BaseObject* fatherObject = nullptr,
        float x = 0.0f, float y = 0.0f, float collisionX = 100.0f, float collisionY = 100.0f, float fatherX = 0.0f, float fatherY = 0.0f,
        class easy2d::Color color = Color::White)
        : x(x), y(y), winX(0.0f), winY(0.0f), collisionX(collisionX), collisionY(collisionY), baseColor(color),
        baseObject(gcnew ShapeNode), baseSprite(baseSprite), targetSprite(targetSprite), action(nullptr),
        text(nullptr), // fatherObject(fatherObject), targetObject(nullptr),
        speed(0.0f), speedAngle(0.0f), speedX(0.0f), speedY(0.0f), angle(0.0f),
        health(100.0f), maxSpeed(10.0f), maxSpeedAngle(10.0f),
        acceleration(5.5f), accelerationAngle(5.0f),
        friction(1.5f), frictionAngle(2.5f), elastic(0.0f),
        fatherX(fatherX), fatherY(fatherY),
        tempTime(0.0f) {
        if (init)
            Init();
    }
    void Init()
    {
        baseObject->setWidth(collisionX);
        baseObject->setHeight(collisionY);
        baseObject->setAnchor(0.5f, 0.5f);

        DrawingStyle style;
        style.mode = DrawingStyle::Mode::Solid;
        style.fillColor = baseColor;

        baseObject = ShapeNode::createRect(Size(collisionX, collisionY));
        baseObject->setDrawingStyle(style);
        if (baseSprite != nullptr)
        {
            baseSprite->setAnchor(0.5f, 0.5f);
            baseObject->addChild(baseSprite);
        }
        if (targetSprite != nullptr)
            targetSprite->addChild(baseObject);
    }
    // 拷贝函数
    void DeepCopy(const ChildObject* other)
    {
        action = other->action;
        baseSprite = other->baseSprite;
        baseObject = other->baseObject;
        targetSprite = other->targetSprite;
        // fatherObject = other->fatherObject;
        x = other->x;
        y = other->y;
        winX = other->winX;
        winY = other->winY;
        angle = other->angle;
        speed = other->speed;
        health = other->health;
        collisionX = other->collisionX;
        collisionY = other->collisionY;
        speedX = other->speedX;
        speedY = other->speedY;
        maxSpeed = other->maxSpeed;
        maxSpeedAngle = other->maxSpeedAngle;
        acceleration = other->acceleration;
        accelerationAngle = other->accelerationAngle;
        friction = other->friction;
        frictionAngle = other->frictionAngle;
        elastic = other->elastic;
        fatherX = other->fatherX;
        fatherY = other->fatherY;
        baseColor = other->baseColor;
    }
    // 应用函数
    void Apply()
    {
        x += speedX;
        y += speedY;
        angle += speedAngle;
        /*if (fatherObject != nullptr)
        {
            angle = fatherObject->angle;
            float Angle = fatherObject->angle * acos(-1) / 180;
            float changeX = 0, changeY = 0;
            changeY += cos(Angle) * fatherY;
            changeX += sin(Angle) * fatherY;
            changeY -= sin(Angle) * fatherX;
            changeX += cos(Angle) * fatherX;
            x = fatherObject->x + changeX;
            y = fatherObject->y + changeY;
        }*/
    }
    //Base
    class easy2d::Sprite* targetSprite; // 目标精灵
    class easy2d::Spawn* action;        // 定义动画
    class easy2d::Sprite* baseSprite;   // 定义精灵
    class easy2d::ShapeNode* baseObject;// 定义节点
    // 公共属性...
    float x, y;                         // 默认坐标
    float angle;                        // 当前角度
    float speed;                        // 当前速度
    float speedAngle;                   // 当前角速度
    float winX, winY;                   // 默认窗口坐标
    float health;                       // 默认生命值
    float collisionX, collisionY;       // 默认碰撞箱
    float speedX, speedY;               // 当前移动速度
    float maxSpeed;                     // 默认最大速度
    float maxSpeedAngle;                // 默认最大角速度
    float acceleration;                 // 默认每帧加速度
    float accelerationAngle;            // 默认每帧角速度
    float friction;                     // 默认摩擦系数
    float frictionAngle;                // 默认角度摩擦
    float elastic;                      // 默认弹力转化率
    class easy2d::Color baseColor;      // 默认节点颜色
    // Text
    class easy2d::Text* text;           // 定义文字节点
    // Father
    // BaseObject* fatherObject;           // 指向父亲的指针
    float fatherX, fatherY;             // 定义相对坐标
private:
    float tempTime;
    // 控制键的状态
    bool upKey, downKey, leftKey, rightKey, rotateLeftKey, rotateRightKey;
    // Target
    // BaseObject* targetObject;           // 指向目标的指针
};
class BaseObject
{
public:
    using ControlFunction = std::function<std::tuple<bool, bool, bool, bool, bool, bool>(const BaseObject&)>;
    using ChildrenFunction = std::function<bool(const BaseObject&)>;

    // 构造函数，用于初始化对象的各种属性
    BaseObject(bool init = false, class easy2d::Sprite* targetSprite = nullptr, class easy2d::Sprite* baseSprite = nullptr, BaseObject* fatherObject = nullptr,
        float x = 0.0f, float y = 0.0f, float collisionX = 100.0f, float collisionY = 100.0f, float fatherX = 0.0f, float fatherY = 0.0f,
        class easy2d::Color color = Color::White)
        : x(x), y(y), winX(0.0f), winY(0.0f), collisionX(collisionX), collisionY(collisionY), baseColor(color),
        baseObject(gcnew ShapeNode), baseSprite(baseSprite), targetSprite(targetSprite), action(nullptr),
        text(nullptr), childObject(nullptr), fatherObject(fatherObject), targetObject(nullptr),
        speed(0.0f), speedAngle(0.0f), speedX(0.0f), speedY(0.0f), angle(0.0f),
        health(100.0f), maxSpeed(10.0f), maxSpeedAngle(10.0f),
        acceleration(5.5f), accelerationAngle(5.0f),
        friction(1.5f), frictionAngle(2.5f), elastic(0.0f),
        lifeTime(1.0f), childSpeed(0.0f), childAngle(0.0f), childTrueAngle(0.0f), lntervalTime(0.1f), childX(0.0f), childY(0.0f),
        fatherX(fatherX), fatherY(fatherY), followFather(false),
        tempTime(0.0f) {
        if (init)
            Init();
    }
    void Init()
    {
        baseObject->setWidth(collisionX);
        baseObject->setHeight(collisionY);
        baseObject->setAnchor(0.5f, 0.5f);

        DrawingStyle style;
        style.mode = DrawingStyle::Mode::Solid;
        style.fillColor = baseColor;

        baseObject = ShapeNode::createRect(Size(collisionX, collisionY));
        baseObject->setDrawingStyle(style);
        if (baseSprite != nullptr)
        {
            baseSprite->setAnchor(0.5f, 0.5f);
            baseObject->addChild(baseSprite);
        }
        if (targetSprite != nullptr)
            targetSprite->addChild(baseObject);
    }
    // 设置控制函数
    void SetControlFunction(ControlFunction controlFunction)
    {
        controlFunction_ = controlFunction;
    }
    // 设置儿子函数
    void SetChildrenFunction(ChildrenFunction childrenFunction)
    {
        childrenFunction_ = childrenFunction;
    }
    // 获取并应用控制函数的返回值
    void ApplyControlFunction()
    {
        if (controlFunction_)
        {
            auto controlValues = controlFunction_(*this);
            ApplyControlValues(controlValues);
        }
    }
    // 获取并应用儿子函数的返回值
    void ApplyChildrenFunction()
    {
        if (childrenFunction_)
        {
            auto childrenValues = childrenFunction_(*this);
            ApplyChildrenValues(childrenValues);
        }
    }
    // 应用控制值 上下左右
    void ApplyControlValues(const std::tuple<bool, bool, bool, bool, bool, bool>& controlValues)
    {
        std::tie(upKey, downKey, leftKey, rightKey, rotateLeftKey, rotateRightKey) = controlValues;
        //Move
        double time = (double)Time::getDeltaTime();
        // float limX = abs(cos(angle) * maxSpeed);
        // float limY = abs(sin(angle) * maxSpeed);
        float limX = maxSpeed;
        float limY = maxSpeed;
        float Angle = angle * acos(-1) / 180;
        if (upKey)
        {
            speedY += cos(Angle) * acceleration * time;
            speedX += sin(Angle) * acceleration * time;
        }
        if (downKey)
        {
            speedY -= cos(Angle) * acceleration * time;
            speedX -= sin(Angle) * acceleration * time;
        }
        if (leftKey)
        {
            speedY += sin(Angle) * acceleration * time;
            speedX -= cos(Angle) * acceleration * time;
        }
        if (rightKey)
        {
            speedY -= sin(Angle) * acceleration * time;
            speedX += cos(Angle) * acceleration * time;
        }

        if (abs(speedX) < abs(sin(Angle) * friction * time)) speedX = 0;
        else
        {
            if (speedX < 0) speedX += abs(sin(Angle) * friction * time);
            if (speedX > 0) speedX -= abs(sin(Angle) * friction * time);
        }
        if (abs(speedY) < abs(cos(Angle) * friction * time)) speedY = 0;
        else
        {
            if (speedY < 0) speedY += abs(cos(Angle) * friction * time);
            if (speedY > 0) speedY -= abs(cos(Angle) * friction * time);
        }

        if (abs(speedX) > limX)
        {
            if (speedX < 0) speedX = -limX;
            if (speedX > 0) speedX = limX;
        }
        if (abs(speedY) > limY)
        {
            if (speedY < 0) speedY = -limY;
            if (speedY > 0) speedY = limY;
        }

        speed = sqrt(speedX * speedX + speedY * speedY);

        //Angle
        if (targetObject == nullptr)
        {
            if (rotateLeftKey)
            {
                speedAngle -= accelerationAngle * time;
            }
            if (rotateRightKey)
            {
                speedAngle += accelerationAngle * time;
            }
        }
        else
        {
            //aim...
        }

        if (speedAngle < 0) speedAngle += frictionAngle * time;
        if (speedAngle > 0) speedAngle -= frictionAngle * time;
        if (abs(speedAngle) <= frictionAngle * time) speedAngle = 0;

        if (abs(speedAngle) > maxSpeedAngle)
        {
            if (speedAngle < 0) speedAngle = -maxSpeedAngle;
            if (speedAngle > 0) speedAngle = maxSpeedAngle;
        }
    }
    // 应用儿子
    void ApplyChildrenValues(bool& childrenValues)
    {
        if (childrenValues)
        {
            float time = Time::getDeltaTime();
            tempTime += time;
            // std::cout << tempTime << '\n';
            if (tempTime >= lntervalTime)
            {
                tempTime -= lntervalTime;
                float baseSpeedX = 0.0f;
                float baseSpeedY = 0.0f;
                if (followFather)
                {
                    baseSpeedX = speedX;
                    baseSpeedY = speedY;
                    if (fatherObject != nullptr)
                    {
                        baseSpeedX = fatherObject->speedX;
                        baseSpeedY = fatherObject->speedY;
                    }
                }
                childTrueAngle = angle + childAngle;
                float Angle = angle * acos(-1) / 180;
                float changeX = 0, changeY = 0;
                changeY += cos(Angle) * childY;
                changeX += sin(Angle) * childY;
                changeY -= sin(Angle) * childX;
                changeX += cos(Angle) * childX;
                float newX = x + changeX;
                float newY = y + changeY;
                ChildObject child_ = childObject;
                child_.DeepCopy(childObject);
                auto bulletSprite = gcnew Sprite;
                bulletSprite->open("PNG/Lasers/laserBlue01.png");
                child_.baseSprite = bulletSprite;
                DrawingStyle style;
                style.mode = DrawingStyle::Mode::Solid;
                style.fillColor = baseColor;
                child_.baseObject = ShapeNode::createCircle(1, style);
                child_.baseObject->setAnchor(0.5f, 0.5f);
                child_.baseSprite->setAnchor(0.5f, 0.5f);
                child_.baseSprite->setPos(0.0f, 0.0f);

                child_.angle = childTrueAngle;
                child_.x = newX;
                child_.y = newY;
                child_.baseColor = baseColor;
                Angle = child_.angle * acos(-1) / 180;
                child_.speedX = sin(Angle) * childSpeed + baseSpeedX;
                child_.speedY = cos(Angle) * childSpeed + baseSpeedY;

                child_.Init();
                targetSprite->addChild(child_.baseObject);
                childlist.push_back(child_);
                // std::cout << "PUSH\n";
                Timer::add([=]()
                    {
                        // std::cout << "REMOVE CHILD\n";
                        targetSprite->removeChild(child_.baseObject);
                        childlist.pop_front();
                    }, lifeTime, 1);
            }
        }
    }
    // 拷贝函数
    void DeepCopy(const BaseObject* other)
    {
        action = other->action;
        baseSprite = other->baseSprite;
        baseObject = other->baseObject;
        targetSprite = other->targetSprite;
        fatherObject = other->fatherObject;
        controlFunction_ = other->controlFunction_;
        childrenFunction_ = other->childrenFunction_;
        x = other->x;
        y = other->y;
        winX = other->winX;
        winY = other->winY;
        angle = other->angle;
        speed = other->speed;
        health = other->health;
        collisionX = other->collisionX;
        collisionY = other->collisionY;
        speedX = other->speedX;
        speedY = other->speedY;
        maxSpeed = other->maxSpeed;
        maxSpeedAngle = other->maxSpeedAngle;
        acceleration = other->acceleration;
        accelerationAngle = other->accelerationAngle;
        friction = other->friction;
        frictionAngle = other->frictionAngle;
        elastic = other->elastic;
        lifeTime = other->lifeTime;
        childSpeed = other->childSpeed;
        childAngle = other->childAngle;
        childTrueAngle = other->childTrueAngle;
        fatherX = other->fatherX;
        fatherY = other->fatherY;
        baseColor = other->baseColor;
    }
    // 应用函数
    void Apply()
    {
        x += speedX;
        y += speedY;
        angle += speedAngle;
        if (fatherObject != nullptr)
        {
            angle = fatherObject->angle;
            float Angle = fatherObject->angle * acos(-1) / 180;
            float changeX = 0, changeY = 0;
            changeY += cos(Angle) * fatherY;
            changeX += sin(Angle) * fatherY;
            changeY -= sin(Angle) * fatherX;
            changeX += cos(Angle) * fatherX;
            x = fatherObject->x + changeX;
            y = fatherObject->y + changeY;
        }
    }
    //Base
    class easy2d::Sprite* targetSprite; // 目标精灵
    class easy2d::Spawn* action;        // 定义动画
    class easy2d::Sprite* baseSprite;   // 定义精灵
    class easy2d::ShapeNode* baseObject;// 定义节点
    // 公共属性...
    float x, y;                         // 默认坐标
    float angle;                        // 当前角度
    float speed;                        // 当前速度
    float speedAngle;                   // 当前角速度
    float winX, winY;                   // 默认窗口坐标
    float health;                       // 默认生命值
    float collisionX, collisionY;       // 默认碰撞箱
    float speedX, speedY;               // 当前移动速度
    float maxSpeed;                     // 默认最大速度
    float maxSpeedAngle;                // 默认最大角速度
    float acceleration;                 // 默认每帧加速度
    float accelerationAngle;            // 默认每帧角速度
    float friction;                     // 默认摩擦系数
    float frictionAngle;                // 默认角度摩擦
    float elastic;                      // 默认弹力转化率
    class easy2d::Color baseColor;      // 默认节点颜色
    // Text
    class easy2d::Text* text;           // 定义文字节点
    // Child
    bool followFather;                  // 设置相对
    std::deque<ChildObject> childlist;  // 定义儿子列表
    ChildObject* childObject;            // 指向儿子的指针
    float lifeTime;                     // 定义儿子存在时间
    float lntervalTime;                 // 定义儿子间歇时间
    float childSpeed;                   // 儿子发射速度
    float childAngle;                   // 儿子发射角度
    float childTrueAngle;               // 儿子发射角度 （真）
    float childX, childY;             // 定义相对坐标
    // Father
    BaseObject* fatherObject;           // 指向父亲的指针
    float fatherX, fatherY;             // 定义相对坐标
    //Target
    BaseObject* targetObject;           // 指向目标的指针
private:
    // 控制函数
    ControlFunction controlFunction_;   // 操作函数
    ChildrenFunction childrenFunction_; // 儿子操作函数
    float tempTime;
    // 控制键的状态
    bool upKey, downKey, leftKey, rightKey, rotateLeftKey, rotateRightKey;
};