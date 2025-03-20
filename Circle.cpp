//
// Created by oreil on 1/5/2025.
//

#include "Circle.h"
#include <string>
#include <stdlib.h>
#include <valarray>


Circle::Circle() {

    radius = 60.0f;
    color = BLUE;

    xPos = GetScreenWidth() / 2;
    yPos = GetScreenHeight() / 2;

    xVelocity = 0.0f;
    yVelocity = 0.0f;

    gravity = -1.0f;
    movementSpeed = 1.0f;
    maxSpeed = 21.0f;
    friction = 1.2f;
    jumpVelocity = 30.0f;
    doubleJumping = false;
    lastWallJumpDirection = ' ';
}

Circle::Circle(float newRadius, Color newColor) {
    radius = newRadius;
    color = newColor;

    xPos = GetScreenWidth() / 2;
    yPos = -200;

    xVelocity = 0.0f;
    yVelocity = 0.0f;

    gravity = -1.8f;
    movementSpeed = 2.3f;
    maxSpeed = 20.0f;
    friction = 1.3f;
    jumpVelocity = 30.0f;
    doubleJumping = false;
    lastWallJumpDirection = ' ';
    hitbox = Rectangle{xPos, yPos, radius, radius};
}

Circle::~Circle() = default;    //default destructor


char Circle::checkCollisionX() {

    char currentDirection = ' ';
    (xVelocity > 0.0f) ? currentDirection = 'R' : currentDirection = 'L';

    if (!checkCollisionFloor(GetScreenHeight())) {  //wall jump

        if (IsKeyDown(' ')
            && (xPos <= 0.0f + radius || xPos >= GetScreenWidth() - radius)
            && (abs(xVelocity) >= maxSpeed * 0.8)
            && currentDirection != lastWallJumpDirection
            ) {

            WallJump();

            return ' '; //cancel collision
        }
        else {
            doubleJumping = true;
        }
    }
    else {
        //color = BLUE;
        doubleJumping = false;
        lastWallJumpDirection = ' ';
    }

    if (xPos > 0.0f + radius && xPos < GetScreenWidth() - radius) {
        return ' '; //no collion
    }
    else if (xPos <= 0.0f + radius) {    //collision left

        xPos = 0.0f + radius;

        xVelocity = 0.0f;

        return 'L';
    }
    else if (xPos >= GetScreenWidth() - radius) { //right

        xPos = GetScreenWidth() - radius;

        xVelocity = 0.0f;
        return 'R';
    }

    return '?'; //freeze if there is an error
}

bool Circle::checkCollisionFloor(const int floor) {

    if (yPos < floor - radius) {

        return false;
    }
    else {  //collision
        if (yPos > floor - radius) {
            yPos = floor - radius;
        }

        if (abs(yVelocity)  > 0.0f) {yVelocity = 0.0f;}
        doubleJumping = false;
        return true;
    }

    return true; //freeze if there is an error
}

bool Circle::checkCollisionCeiling() {
    if (yPos > 0 + radius) {
        return false;
    }
    else {  //colliding with ceiling
        yVelocity = 0.0f;
        if (yPos < 0 + radius) {yPos = 0 + radius;}
    }
    return true;
}

void Circle::CauseFriction() {  //make xVelocity reduce to zero
    if (xVelocity > 0.0f) {
        xVelocity -= friction;
    }
    else if (xVelocity < 0.0f) {
        xVelocity += friction;
    }
}

void Circle::MoveLeft() {
    if (checkCollisionX() != 'L') { //can move left
        if (IsKeyDown('A') && abs(xVelocity) < maxSpeed) {
            xVelocity -= movementSpeed; //give a negative x velocity if A is pressed
            if (abs(xVelocity) > maxSpeed) {xVelocity = -maxSpeed;}
        }
        else if (!IsKeyDown('A') && abs(xVelocity) > 0.0f) { //positive friction
            if (xVelocity + friction > 0.0f) {
                xVelocity = 0.0f;
            }
            else {
                CauseFriction();
            }
        }

        xPos += xVelocity;
    }
}

void Circle::MoveRight() {
    if (checkCollisionX() != 'R') { //can move right
        if (IsKeyDown('D') && abs(xVelocity) < maxSpeed) {
            xVelocity += movementSpeed;
            if (abs(xVelocity) > maxSpeed) {xVelocity = maxSpeed;}
        }
        else if (!IsKeyDown('D') && abs(xVelocity) > 0.0f) {    //negative friction
            if (xVelocity - friction < 0.0f) {
                xVelocity = 0.0f;
            }
            else {
                CauseFriction();
            }
        }

        xPos += xVelocity;
    }
}

void Circle::Brake() {
    checkCollisionX();
    xPos += xVelocity;
    if (xVelocity < 1.0f && xVelocity > -1.0f) {xVelocity = 0.0f;}
    CauseFriction();
}

void Circle::MoveLeftRight() {
    if (IsKeyDown('A') && IsKeyDown('D')) {
        if (abs(xVelocity) > 0.0f) {    //keep moving, but slow
            Brake();
        }
    }
    else {
        if (IsKeyDown('A') || xVelocity < 0.0f) {MoveLeft();}
        if (IsKeyDown('D') || xVelocity > 0.0f) {MoveRight();}
    }
}

void Circle::Freefall() {
    yVelocity += gravity;
    yPos -= yVelocity;
}

void Circle::Jump() {
    yVelocity += jumpVelocity;
    Freefall();
}

void Circle::WallJump() {

    (xVelocity > 0.0f) ? lastWallJumpDirection = 'R' : lastWallJumpDirection = 'L';

    xVelocity = -xVelocity * 1.04f;
    yVelocity = jumpVelocity;

}

void Circle::MoveUpDown() {
    checkCollisionCeiling();

    if (IsKeyDown(' ') && checkCollisionFloor(GetScreenHeight())) {Jump();}

    else if (IsKeyDown(' ') &&
        !doubleJumping && !checkCollisionFloor(GetScreenHeight()) &&
        (checkCollisionX() == 'R' || checkCollisionX() == 'L')) {}

    else if (!checkCollisionFloor(GetScreenHeight())) {Freefall();}
}

void Circle::Draw() {
    DrawCircle(xPos, yPos, radius, color);
}

void Circle::UpdateHitbox() {
    hitbox = Rectangle{xPos - radius, yPos - radius, radius * 2, radius * 2};
}

void Circle::DrawHitbox() {
    DrawRectangleLinesEx(hitbox, 3, RED);
}

void Circle::EnvCollision(Rectangle objArray[], int objs) {

    for (int i = 0; i < objs; i++) {
        if (CheckCollisionRecs(objArray[i], hitbox)) {
            DrawHitbox();
        }
    }

    return;
}

void Circle::Update(Rectangle objArray[], int objs) {
    MoveLeftRight();
    MoveUpDown();
    UpdateHitbox();
    EnvCollision(objArray, objs);
}





//getter and setters

float Circle::getRadius() {
    return radius;
}

Color Circle::getColor() {
    return color;
}

void Circle::setColor(Color newColor) {
    color = newColor;
}

int Circle::getXPos() {
    return xPos;
}

int Circle::getYPos() {
    return yPos;
}

void Circle::setXPos(int newXPos) {
    xPos = newXPos;
}

void Circle::setYPos(int newYPos) {
    yPos = newYPos;
}

float Circle::getXVelocity() {
    return xVelocity;
}

float Circle::getYVelocity() {
    return yVelocity;
}

void Circle::setXVelocity(float newXVelocity) {
    xVelocity = newXVelocity;
}

void Circle::setYVelocity(float newYVelocity) {
    yVelocity = newYVelocity;
}

Rectangle Circle::getHitbox() {
    return hitbox;
}




