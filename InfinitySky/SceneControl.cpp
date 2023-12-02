/*#include "SceneControl.h"
#include <iostream>
#include <vector>
#include <queue>
#include <math.h>
#include <easy2d/easy2d.h>

using namespace easy2d;
using namespace std;

void MapBlock::InitMapBlock()
{
    mapBlock->setWidth(collisionX);
    mapBlock->setHeight(collisionY);

    DrawingStyle style;
    style.mode = DrawingStyle::Mode::Solid; // 绘图模式为填充
    style.fillColor = baseColor;           // 填充色

    mapBlock = ShapeNode::createRect(Size(collisionX, collisionY));
    mapBlock->setDrawingStyle(style);
}
void MapBlock::Coordinates()
{
}

void MapText::InitMapText()
{
    // 创建文字样式
    TextStyle style;
    style.alignment = TextAlign::Left;      // 左对齐
    style.wrapping = true;                  // 开启自动换行
    style.wrappingWidth = width;            // 文字自动换行的宽度
    style.lineSpacing = lineSpacing;        // 行间距
    style.hasUnderline = underLine;         // 下划线
    style.hasStrikethrough = strikeThrough; // 删除线

    mapText->setTextStyle(style);
    mapText->setRotation(rotation);
    mapText->setScale(scale);
    mapText->setOpacity(opacity);
}
void MapText::Coordinates()
{
}

void Player::InitPlayer()
{
    node->setWidth(collisionX);
    node->setHeight(collisionY);
    node->setAnchor(0.5f, 0.5f);

    DrawingStyle style;
    style.mode = DrawingStyle::Mode::Solid; // 绘图模式为填充
    style.fillColor = baseColor;           // 填充色

    node = ShapeNode::createRect(Size(collisionX, collisionY));
    node->setDrawingStyle(style);
}
void Player::Coordinates()
{
    double time = (double)Time::getDeltaTime();
    // 获取鼠标位置
    if (true)
    {
        float mouse_x = Input::getMouseX();
        float mouse_y = Input::getMouseY();
        float x = mouse_x - winX;
        float y = winY - mouse_y;
        float change = sqrt(x * x + y * y);
        x /= change; y /= change;
        float angle = acos(y);
        if (x < 0) angle = -angle;
        tragetAngle = angle / acos(-1) * 180;
        changeAngle = tragetAngle - angle;
        if (changeAngle < -180)
        {
            changeAngle += 360;
            angle -= 360;
        }
        if (changeAngle > 180)
        {
            changeAngle -= 360;
            angle += 360;
        }

        float h = speedAngle / (frictionAngle * time);
        if (abs(speedAngle * h) < abs(changeAngle))
        {
            if (changeAngle > 0) speedAngle += accelerationAngle * time;
            if (changeAngle < 0) speedAngle -= accelerationAngle * time;
        }
    }
    else
    {
        if (Input::isDown(KeyCode::D))
        {
            speedAngle += accelerationAngle * time;
        }
        if (Input::isDown(KeyCode::A))
        {
            speedAngle -= accelerationAngle * time;
        }
    }
    double limX = maxSpeed, limY = maxSpeed;
    double Angle = angle * acos(-1) / 180;
    if (Input::isDown(KeyCode::W))
    {
        //limX = abs(sin(angle) * maxSpeed);
        //limY = abs(cos(angle) * maxSpeed);
        speedY += cos(Angle) * acceleration * time;
        speedX += sin(Angle) * acceleration * time;
    }
    if (Input::isDown(KeyCode::D))
    {
        //limX = abs(cos(angle) * maxSpeed);
        //limY = abs(sin(angle) * maxSpeed);
        speedY -= sin(Angle) * acceleration * time;
        speedX += cos(Angle) * acceleration * time;
    }
    if (Input::isDown(KeyCode::S))
    {
        //limX = abs(sin(angle) * maxSpeed);
        //limY = abs(cos(angle) * maxSpeed);
        speedY -= cos(Angle) * acceleration * time;
        speedX -= sin(Angle) * acceleration * time;
    }
    if (Input::isDown(KeyCode::A))
    {
        //limX = abs(cos(angle) * maxSpeed);
        //limY = abs(sin(angle) * maxSpeed);
        speedY += sin(Angle) * acceleration * time;
        speedX -= cos(Angle) * acceleration * time;
    }

    //cout << "X:" << speedX << "Y:" << speedY << endl;

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

    if (speedAngle < 0) speedAngle += frictionAngle * time;
    if (speedAngle > 0) speedAngle -= frictionAngle * time;
    if (abs(speedAngle) <= frictionAngle * time) speedAngle = 0;

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

    if (abs(speedAngle) > maxSpeedAngle)
    {
        if (speedAngle < 0) speedAngle = -maxSpeedAngle;
        if (speedAngle > 0) speedAngle = maxSpeedAngle;
    }

    speed = sqrt(speedX * speedX + speedY * speedY);
}
void Player::CheckCollision(MapBlock Map)
{
    float posX = x + collisionX;
    float posY = y - collisionY;
    float lx = Map.x;
    float ly = Map.y;
    float rx = lx + Map.collisionX;
    float ry = ly - Map.collisionY;
    if ((y < ly + collisionY) && (y > ry))
    {
        if ((posX + speedX >= lx) && (posX <= lx))
        {
            x = lx - collisionX;
            speedX = -speedX * Map.elastic;
        }
        if ((x + speedX <= rx) && (x >= rx))
        {
            x = rx;
            speedX = -speedX * Map.elastic;
        }
    }
    if ((x < rx) && (x > lx - collisionX))
    {
        if ((posY + speedY <= ly) && (posY >= ly))
        {
            y = ly + collisionY;
            speedY = -speedY * Map.elastic;
        }
        if ((y + speedY >= ry) && (y <= ry))
        {
            y = ry;
            speedY = -speedY * Map.elastic;
        }
    }
}
void Player::UseResults()
{
    x += speedX;
    y += speedY;
    angle += speedAngle;
}
void Weapon::InitWeapon()
{
    DrawingStyle style;
    style.mode = DrawingStyle::Mode::Solid;          // 绘图模式为填充
    style.fillColor = baseColor;           // 填充色
    node = ShapeNode::createCircle(radius, style);
    bulletNode = ShapeNode::createCircle(radius, style);
}
void Weapon::Coordinates()
{
    baseSpeedX = targetPlayer->speedX;
    baseSpeedY = targetPlayer->speedY;
    trueAngle = targetPlayer->angle + angle;
    double angle = targetPlayer->angle * acos(-1) / 180;
    float changeX = 0, changeY = 0;
    changeY += cos(angle) * y;
    changeX += sin(angle) * y;
    changeY -= sin(angle) * x;
    changeX += cos(angle) * x;
    x = targetPlayer->x + changeX;
    y = targetPlayer->y + changeY;
    float time = Time::getDeltaTime();
    nowTime += time;
    if (nowTime >= fireTime)
    {
        nowTime -= fireTime;
        Bullet bullet;

        DrawingStyle style;
        style.mode = DrawingStyle::Mode::Solid;          // 绘图模式为填充
        style.fillColor = baseColor;           // 填充色
        bullet.bulletNode = ShapeNode::createCircle(1, style);
        bullet.bulletNode->setAnchor(0.5f, 0.5f);
        auto bullet_sp = gcnew Sprite;
        bullet_sp->open("PNG/Lasers/laserBlue01.png"); // 从本地图片加载
        bullet.bullet_Sprite = bullet_sp;
        bullet.bullet_Sprite->setAnchor(0.5f, 0.5f);

        bullet.angle = trueAngle;
        bullet.x = x;
        bullet.y = y;
        //bullet.bulletNode = bulletNode;
        //bullet.bullet_Sprite = bullet_Sprite;
        bullet.baseColor = baseColor;
        float angle = bullet.angle * acos(-1) / 180;
        bullet.speedX = sin(angle) * speed * time + baseSpeedX;
        bullet.speedY = cos(angle) * speed * time + baseSpeedY;

        //cout << time << endl;
        if (bulletSprite != NULL)
        {
            bullet.bulletNode->addChild(bullet.bullet_Sprite);
            bullet.bullet_Sprite->setAnchor(0.5f, 0.5f);
        }
        targetSprite->addChild(bullet.bulletNode);
        bullet_list.push_back(bullet);
        Timer::add([=]()
            {
                targetSprite->removeChild(bullet.bulletNode);
                bullet_list.pop_front();
            }, lifeTime, 1);
    }
}

void Particle::SetParticleToPlayer(float x, float y, Player node)
{
    baseSpeedX = targetPlayer->speedX;
    baseSpeedY = targetPlayer->speedY;
    trueAngle = targetPlayer->angle + angle;
    double angle = targetPlayer->angle * acos(-1) / 180;
    float changeX = 0, changeY = 0;
    changeY += cos(angle) * y;
    changeX += sin(angle) * y;
    changeY -= sin(angle) * x;
    changeX += cos(angle) * x;
    x = targetPlayer->x + changeX;
    y = targetPlayer->y + changeY;
}
void Particle::InitParticle(float speed, float angle, Color color)
{
    speed = speed;
    angle = angle;
    baseColor = color;
}
*/