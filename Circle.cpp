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

    envFloor = GetScreenHeight();
    envCeiling = 0;
    envLeft = 0.0;
    envRight = GetScreenWidth();

    //thinking about making the circle class better by implementing boolean locks for collision
    //like in Update() -> if touchingFLoor, dont allow y movement downwards, else, freefall
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

    envFloor = GetScreenHeight();
    envCeiling = 0;
    envLeft = 0.0;
    envRight = GetScreenWidth();
}

Circle::~Circle() = default;    //default destructor


char Circle::checkCollisionX() {

    char currentDirection = ' ';
    (xVelocity > 0.0f) ? currentDirection = 'R' : currentDirection = 'L';

    if (!checkCollisionFloor()) {  //wall jump

        if (IsKeyDown(' ')
            && (xPos <= envLeft + radius || xPos >= envRight - radius)
            && (abs(xVelocity) >= maxSpeed * 0.8) //must be going at least 80 percent of max speed to wall jump
            && currentDirection != lastWallJumpDirection //cannot wall jump twice in same direction
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

    if (xPos > envLeft + radius && xPos < envRight - radius) {
        return ' '; //no collion
    }
    else if (xPos <= envLeft + radius) {    //collision left

        xPos = envLeft + radius;

        xVelocity = 0.0f;

        return 'L';
    }
    else if (xPos >= envRight - radius) { //right

        xPos = envRight - radius;

        xVelocity = 0.0f;
        return 'R';
    }

    return '?'; //freeze if there is an error
}

bool Circle::checkCollisionFloor() {

    if (yPos < envFloor - radius) {

        return false;
    }
    else {  //collision
        if (yPos > envFloor - radius) {
            yPos = envFloor - radius;
        }

        if (abs(yVelocity)  > 0.0f) {yVelocity = 0.0f;}
        doubleJumping = false;
        return true;
    }

    return true; //freeze if there is an error
}

bool Circle::checkCollisionCeiling() {
    if (yPos > envCeiling + radius) {
        return false;
    }
    else {  //colliding with ceiling
        yVelocity = 0.0f;
        if (yPos < envCeiling + radius) {yPos = envCeiling + radius;}
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
    yVelocity += gravity; //acceleration due to gravity
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

    if (IsKeyDown(' ') && checkCollisionFloor()) {Jump();}

    else if (IsKeyDown(' ') &&
        !doubleJumping && !checkCollisionFloor() &&
        (checkCollisionX() == 'R' || checkCollisionX() == 'L')) {}

    else if (!checkCollisionFloor()) {Freefall();}
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

    // moved the else case up here to make sure we set to default, THEN check if collision is still happening
    // fixed collision not working with multiple objects

    //default bounds are screen width and height
    envFloor = GetScreenHeight();
    envCeiling = 0;
    envLeft = 0;
    envRight = GetScreenWidth();

    bool collided = false;

    for (int i = 0; i < objs; ++i) {
        if (CheckCollisionRecs(objArray[i], hitbox)) {
            collided = true;
            bool onPlatform = false;
            bool hitWall = false; //trying to fix corner collision bug here

            if (this->yPos < objArray[i].y) { // standing on platform
                this->envFloor = objArray[i].y + 1; // + 1 fixes frame vibrating bug
                onPlatform = true;
            }

            if (this->xPos < objArray[i].x) { //collision to right of circle
                //fix corner collision here?

                if (!onPlatform) { //works for now, but what if circle is on an L shape platform??
                    //wow L shape actually works
                    this->envRight = objArray[i].x + 1;
                    hitWall = true;
                }
            }
            if (this->xPos > objArray[i].x + objArray[i].width) { //collision to left of the circle
                if (!onPlatform) {
                    this->envLeft = objArray[i].x + objArray[i].width - 1;
                    hitWall = true;
                }

            }

            if (this->yPos > objArray[i].y + objArray[i].height) { // hitting ceiling
                if (!hitWall) {
                    //bad corner collision happening here too
                    this->envCeiling = objArray[i].y + objArray[i].height;
                }

            }
        }
    }
}

bool Circle::DoubleCollision() {
    if (checkCollisionFloor() && checkCollisionX()) {
        //

    }
    return false;
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

float Circle::getEnvFloor() {
    return envFloor;
}

float Circle::getEnvCeiling() {
    return envCeiling;
}

float Circle::getEnvLeft() {
    return envLeft;
}

float Circle::getEnvRight() {
    return envRight;
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




