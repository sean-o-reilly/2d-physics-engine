#pragma once
#include "raylib.h"

//
// Created by oreil on 1/5/2025.
//



#ifndef CIRCLE_H
#define CIRCLE_H



class Circle {

private:
    float radius;
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

    Rectangle hitbox;

public:

    Circle();
    Circle(float newRadius, Color newColor);
    ~Circle();

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
