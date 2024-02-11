#pragma once
#include <easy2d/easy2d.h>
#include <unordered_map>
#include <vector>
#include <queue>
#include <tuple>
#include "json/json.h"

inline easy2d::Sprite* copySprite(easy2d::Sprite* aimSprite)
{
    easy2d::Sprite* trueSprite = easy2d::gcnew easy2d::Sprite;
    trueSprite->setProperty(aimSprite->getProperty());
    trueSprite->setImage(aimSprite->getImage());
    for (auto temp : aimSprite->getAllChildren())
    {
        trueSprite->addChild(copySprite(dynamic_cast<easy2d::Sprite*>(temp)));
    }
    return trueSprite;
}

class BaseObject
{
public:
    using FunctionList = std::deque<std::function<void()>>;
    using Function = std::function<void()>;
    using PointList = std::vector<easy2d::Point>;
    using ObjectList = std::deque<BaseObject*>;
    using String = std::string;
    using Dict = std::unordered_map<String, float>;

    // 构造函数，用于初始化对象的各种属性（直接新建）
    BaseObject(int order = 0, bool init = false, 
        easy2d::Sprite* targetSprite = nullptr, easy2d::Sprite* baseSprite = nullptr, BaseObject* fatherObject = nullptr,
        easy2d::Sequence* action = nullptr, bool followFather = false, float x = 0.0f, float y = 0.0f, 
        float fatherX = 0.0f, float fatherY = 0.0f,
        easy2d::Color color = easy2d::Color::White)
        : order(order), x(x), y(y), winX(0.0f), winY(0.0f), baseColor(color),
        baseObject(nullptr), baseSprite(baseSprite), targetSprite(targetSprite), action(action),
        text(nullptr), fatherObject(fatherObject), targetObject(nullptr),
        speed(0.0f), speedAngle(0.0f), speedX(0.0f), speedY(0.0f), angle(0.0f),
        health(100.0f), mass(100.0f), maxSpeed(10.0f), maxSpeedAngle(5.0),
        friction(1.5f), frictionAngle(1.5f), elastic(0.0f), 
        fatherX(fatherX), fatherY(fatherY), followFather(followFather),
        openCollision(false), collisionRotation(0.0f)
    {
        if (init)
            Init();
    }
    // 通过文件生成
    BaseObject(String data)
        : speed(0.0f), speedAngle(0.0f), speedX(0.0f), speedY(0.0f), angle(0.0f),
        health(100.0f), mass(100.0f), maxSpeed(10.0f), maxSpeedAngle(5.0),
        friction(1.5f), frictionAngle(1.5f), elastic(0.0f)
    {
        // 待定...此内容需要更为个性化的设计
    }
    // 拷贝函数
    BaseObject(const BaseObject* other)
        : order(other->order),
        targetSprite(other->targetSprite),
        action(other->action),
        baseSprite(other->baseSprite),
        baseObject(other->baseObject),
        x(other->x),
        y(other->y),
        angle(other->angle),
        speed(other->speed),
        speedAngle(other->speedAngle),
        winX(other->winX),
        winY(other->winY),
        health(other->health),
        speedX(other->speedX),
        speedY(other->speedY),
        maxSpeed(other->maxSpeed),
        maxSpeedAngle(other->maxSpeedAngle),
        friction(other->friction),
        frictionAngle(other->frictionAngle),
        elastic(other->elastic),
        baseColor(other->baseColor),
        text(other->text),
        followFather(other->followFather),
        childlist(other->childlist),
        fatherObject(other->fatherObject),
        fatherX(other->fatherX),
        fatherY(other->fatherY),
        targetObject(other->targetObject),
        upKey(other->upKey),
        downKey(other->downKey),
        leftKey(other->leftKey),
        rightKey(other->rightKey),
        rotateLeftKey(other->rotateLeftKey),
        rotateRightKey(other->rotateRightKey),
        openCollision(other->openCollision),
        collisionRotation(other->collisionRotation),
        collisionPoints(other->collisionPoints) {
    }
    // 析构函数，用于销毁对象
    ~BaseObject()
    {
        if (targetSprite != nullptr) delete targetSprite;   // 释放目标精灵
        if (action != nullptr) delete action;               // 释放动画序列
        if (baseSprite != nullptr)  delete baseSprite;      // 释放基础精灵
        if (baseObject != nullptr) delete baseObject;       // 释放基础节点
        if (text != nullptr) delete text;                   // 释放文字节点
        if (fatherObject != nullptr) delete fatherObject;   // 释放父亲对象
        for (BaseObject* child : childlist)                 // 释放儿子列表中的对象
        {
            if (child->fatherObject != nullptr) delete child->fatherObject;
            delete child;
        }
        childlist.clear();                                  // 清空儿子列表
        collisionPoints.clear();                            // 清空碰撞点集
        controlFunctionList.clear();                        // 清空函数列表
    }
    // 初始化（补充）
    void Init()
    {
        // 根据点集生成碰撞箱
        easy2d::ShapeMaker collisionMaker;
        if (collisionPoints.empty())
        {
            // collisionPoints.push_back(easy2d::Point(100, 100));
            // collisionPoints.push_back(easy2d::Point(100, -100));
            // collisionPoints.push_back(easy2d::Point(-100, -100));
            // collisionPoints.push_back(easy2d::Point(-100, 100));
        }
        else
        {
            collisionMaker.beginPath(collisionPoints[0]);
            for (size_t i = 1; i < collisionPoints.size(); i++)
                collisionMaker.addLine(collisionPoints[i]);
            collisionMaker.endPath(true);
            auto collisionShape = collisionMaker.getShape();
            baseObject = easy2d::gcnew easy2d::ShapeNode(collisionShape);
            baseObject->setOrder(100);
            easy2d::DrawingStyle style;
            style.mode = easy2d::DrawingStyle::Mode::Solid;
            style.fillColor = easy2d::Color::White;
            baseObject->setDrawingStyle(style);
        }
        // 初始化
        if (baseSprite != nullptr)
        {
            auto trueBaseSprite = copySprite(baseSprite);
            trueBaseSprite->setAnchor(0.5f, 0.5f);
            baseObject->addChild(trueBaseSprite);
            trueBaseSprite->setOrder(order);
            baseObject->setOrder(order);
            if (action != nullptr)
            {
                trueBaseSprite->runAction(action->clone());
            }
        }
        if (targetSprite != nullptr)
            targetSprite->addChild(baseObject);
    }
    // 添加控制函数
    void AddControlFunction(Function controlFunction)
    {
        controlFunctionList.push_back(controlFunction);
    }
    // 应用控制函数
    void ApplyControlFunction()
    {
        if (!controlFunctionList.empty())
        {
            for (Function controlFunction : controlFunctionList)
                controlFunction();
        }
    }
    // 对象应用函数
    void Apply()
    {
        x += speedX;
        y += speedY;
        angle += speedAngle;
        if (fatherObject != nullptr)
        {
            trueAngle = fatherObject->trueAngle + angle;
            float Angle = fatherObject->trueAngle * (float)acos(-1) / 180;
            float changeX = 0, changeY = 0;
            changeY += cos(Angle) * fatherY;
            changeX += sin(Angle) * fatherY;
            changeY -= sin(Angle) * fatherX;
            changeX += cos(Angle) * fatherX;
            x = fatherObject->x + changeX;
            y = fatherObject->y + changeY;
        }
        if (openCollision)
        {
            for (const easy2d::Point& point : collisionPoints)
            {
                // 将每个点绕碰撞箱中心旋转
                float rotatedX = cos(collisionRotation) * point.x - sin(collisionRotation) * point.y;
                float rotatedY = sin(collisionRotation) * point.x + cos(collisionRotation) * point.y;

                // 计算旋转后的点的全局坐标
                float globalX = x + rotatedX;
                float globalY = y + rotatedY;

                collisionPoints.emplace_back(globalX, globalY);
            }
        }
    }
    // Function
    void SolePhysics();                                     // 独立物理计算
    void ApplyForce(float val, float angle_);               // 施加力（局部）
    void ApplyGlobalForce(float val, float angle_);         // 施加力（全局）
    void ApplyVelocity(float val, float angle_);            // 赋值速度（局部）
    void ApplyGlobalVelocity(float val, float angle_);      // 赋值速度（全局）
    void ApplyAcceleration(float val, float angle_);        // 给予加速度（局部）
    void ApplyGlobalAcceleration(float val, float angle_);  // 给予加速度（全局）
    // Tag
    String tag;
    String level;
    String layout;
    // Base
    int order;  
    easy2d::Sprite* targetSprite;       // 目标精灵
    easy2d::Sequence* action;           // 定义动画
    easy2d::Sprite* baseSprite;         // 定义精灵
    easy2d::ShapeNode* baseObject;      // 定义节点
    float x, y;                         // 默认坐标
    float mass;                         // 默认质量
    float angle;                        // 当前角度
    float trueAngle;                    // 真正角度
    float speed;                        // 当前速度
    float speedAngle;                   // 当前角速度
    float winX, winY;                   // 默认窗口坐标
    float health;                       // 默认生命值
    float speedX, speedY;               // 当前移动速度
    float maxSpeed;                     // 默认最大速度
    float maxSpeedAngle;                // 默认最大角速度
    float friction;                     // 默认摩擦系数（独立）
    float frictionAngle;                // 默认角度摩擦（独立）
    float elastic;                      // 默认弹力转化率
    Dict variable;                      // 自定义变量
    easy2d::Color baseColor;            // 默认节点颜色
    // Text
    easy2d::Text* text;                 // 自定义文字节点
    // Child
    bool followFather;                  // 设置是否相对
    ObjectList childlist;               // 定义儿子列表
    // 已拥有儿子列表，下列内容失去存在价值，已删除
    // BaseObject* childObject;         // 指向儿子的指针（待定）
    // float lifeTime;                  // 定义儿子存在时间
    // float tempTime;                  // 当前儿子操作阈值
    // float lntervalTime;              // 定义儿子间歇时间
    // float childSpeed;                // 儿子发射速度
    // float childAngle;                // 儿子发射角度
    // float childTrueAngle;            // 儿子发射角度（真）
    // float childX, childY;            // 定义相对坐标
    // Father
    BaseObject* fatherObject;           // 指向父亲的指针
    float fatherX, fatherY;             // 定义相对坐标
    float fatherAngle;                  // 定义相对旋转
    // Target
    BaseObject* targetObject;           // 指向目标的指针
    // Collision
    bool openCollision;                 // 开启碰撞箱
    PointList collisionPoints;          // 碰撞箱点集
    float collisionRotation;            // 碰撞箱旋转
private:
    // 控制函数
    FunctionList controlFunctionList;   // 操作函数列表
    // 控制键的状态
    bool upKey, downKey, leftKey, rightKey, rotateLeftKey, rotateRightKey;
};