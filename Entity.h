#pragma once
#include "raylib.h"

//
// Created by oreil on 1/5/2025.
//



#ifndef CIRCLE_H
#define CIRCLE_H



class Entity {

private:
    float center;
    int height;
    int width;
    Color color;

    int xPos;
    int yPos;

    float xVelocity;  //velocity = distance per frame
    float yVelocity;

    float gravity;  //in case we want to change this objects gravity
    float movementSpeed;
    float maxSpeed;
    float friction;  //in case we want to change this objects friction

    float jumpVelocity;
    bool doubleJumping;
    char lastWallJumpDirection;

    bool touchingFloor;

    float envFloor;
    float envCeiling;
    float envLeft;
    float envRight;

    Texture2D spriteSheet;
    int currentFrame = 0;
    int frameCounter = 0;
    int frameSpeed = 8;
    int totalFrames = 7; //7 for idle sprite sheet
    int columns = 5;
    float frameW = 74.0f;
    float frameH = 132.0f; //png widths for sprite sheet
    bool facingLeft = false;

    Rectangle hitbox;


public:

    Entity();
    Entity(float newRadius, int height, int width, Color newColor, Texture2D newTexture);
    ~Entity();

    char checkCollisionX();
    bool checkCollisionFloor();
    bool checkCollisionCeiling();

    void CauseFriction();
    void MoveLeft();
    void MoveRight();
    void Brake();
    void MoveLeftRight();

    void Jump();
    void Freefall();
    void WallJump();
    void MoveUpDown();

    void Draw();

    void UpdateHitbox();
    void DrawHitbox();

    void EnvCollision(Rectangle objArray[], int objs);
    bool DoubleCollision();

    void Update(Rectangle objArray[], int objs);




    float getRadius();
    Color getColor();
    void setColor(Color newColor);
    int getXPos();
    int getYPos();
    void setXPos(int newXPos);
    void setYPos(int newYPos);
    float getXVelocity();
    float getYVelocity();
    void setXVelocity(float newXVelocity);
    void setYVelocity(float newYVelocity);
    Rectangle getHitbox();
    float getEnvFloor();
    float getEnvCeiling();
    float getEnvLeft();
    float getEnvRight();
};



#endif //CIRCLE_H
