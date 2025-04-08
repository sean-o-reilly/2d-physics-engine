//
// Created by oreil on 1/5/2025.
//

#include "Entity.h"
#include <string>
#include <stdlib.h>
#include <valarray>
#include <cassert>


Entity::Entity() {

    center = 60.0f;
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

Entity::Entity(float newCenter, int newHeight, int newWidth, Color newColor, Texture2D newTexture) {

    height = newHeight;
    width = newWidth;


    color = newColor;

    xPos = GetScreenWidth() / 2;
    yPos = -200;

    xVelocity = 0.0f;
    yVelocity = 0.0f;

    gravity = -1.3f;
    movementSpeed = 1.3f;
    maxSpeed = 12.0f;
    friction = 1.3f;
    jumpVelocity = 25.0f;
    doubleJumping = false;
    lastWallJumpDirection = ' ';


    envFloor = GetScreenHeight();
    envCeiling = 0;
    envLeft = 0.0;
    envRight = GetScreenWidth();


    spriteSheet = newTexture;
    assert(IsTextureValid(spriteSheet));
}

Entity::~Entity() = default;    //default destructor


char Entity::checkCollisionX() {

    char currentDirection = ' ';
    (xVelocity > 0.0f) ? currentDirection = 'R' : currentDirection = 'L';

    if (!checkCollisionFloor()) {  //wall jump

        if (IsKeyDown(' ')
            && (xPos <= envLeft + hitbox.width || xPos >= envRight - hitbox.width)
            && (abs(xVelocity) >= maxSpeed * 0.8) //must be going at least 80 percent of max speed to wall jump
            && currentDirection != lastWallJumpDirection //cannot wall jump twice in same direction
            ) {

            // WallJump();

            // return ' '; //cancel collision
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

    if (xPos > envLeft + hitbox.width && xPos < envRight - hitbox.width) {
        return ' '; //no collion
    }
    else if (xPos <= envLeft + (hitbox.width / 2)) {    //collision left

        xPos = envLeft + (hitbox.width / 2);

        xVelocity = 0.0f;

        return 'L';
    }
    else if (xPos >= envRight - (hitbox.width / 2)) { //right

        xPos = envRight - (hitbox.width / 2);

        xVelocity = 0.0f;
        return 'R';
    }

    return '?'; //freeze if there is an error
}

bool Entity::checkCollisionFloor() {

    if (yPos < envFloor - (hitbox.height / 2)) {

        return false;
    }
    else {  //collision
        if (yPos > envFloor - (hitbox.height / 2)) {
            yPos = envFloor - (hitbox.height / 2);
        }

        if (abs(yVelocity)  > 0.0f) {yVelocity = 0.0f;}
        doubleJumping = false;
        return true;
    }

    return true; //freeze if there is an error
}

bool Entity::checkCollisionCeiling() {
    if (yPos > envCeiling + (hitbox.height / 2)) {
        return false;
    }
    else {  //colliding with ceiling
        yVelocity = 0.0f;
        if (yPos < envCeiling + (hitbox.height / 2)) {yPos = envCeiling + (hitbox.height / 2);}
    }
    return true;
}

void Entity::CauseFriction() {  //make xVelocity reduce to zero
    if (xVelocity > 0.0f) {
        xVelocity -= friction;
    }
    else if (xVelocity < 0.0f) {
        xVelocity += friction;
    }
}

void Entity::MoveLeft() {
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
        facingLeft = true;
    }
}

void Entity::MoveRight() {
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
        facingLeft = false;
    }
}

void Entity::Brake() {
    checkCollisionX();
    xPos += xVelocity;
    if (xVelocity < 1.0f && xVelocity > -1.0f) {xVelocity = 0.0f;}
    CauseFriction();
}

void Entity::MoveLeftRight() {
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

void Entity::Freefall() {
    yVelocity += gravity; //acceleration due to gravity
    yPos -= yVelocity;
}

void Entity::Jump() {
    yVelocity += jumpVelocity;
    Freefall();
}

void Entity::WallJump() {

    (xVelocity > 0.0f) ? lastWallJumpDirection = 'R' : lastWallJumpDirection = 'L';

    xVelocity = -xVelocity * 1.04f;
    yVelocity = jumpVelocity;

}

void Entity::MoveUpDown() {
    checkCollisionCeiling();

    if (IsKeyDown(' ') && checkCollisionFloor()) {Jump();}

    else if (IsKeyDown(' ') &&
        !doubleJumping && !checkCollisionFloor() &&
        (checkCollisionX() == 'R' || checkCollisionX() == 'L')) {}

    else if (!checkCollisionFloor()) {Freefall();}
}

void Entity::Draw() {
    frameCounter++;
    if (frameCounter >= frameSpeed) {
        frameCounter = 0;
        currentFrame = (currentFrame + 1) % totalFrames;
    }

    // Calculate frame position in sprite sheet
    int frameX = currentFrame % columns;
    int frameY = currentFrame / columns;

    Rectangle sourceRec = {
        frameW * frameX,
        frameH * frameY,
        facingLeft? -frameW : frameW,
        frameH
    };

    float scale = 2.0f;
    Rectangle destRec = { xPos, yPos, frameW * scale, frameH * scale };
    Vector2 origin = { destRec.width / 2, destRec.height / 2 };

    DrawTexturePro(spriteSheet, sourceRec, destRec, origin, 0.0f, WHITE);
}

void Entity::UpdateHitbox() {
    hitbox = Rectangle{xPos - (width / 2), yPos - (height / 2), width, height};
}

void Entity::DrawHitbox() {
    DrawRectangleLinesEx(hitbox, 3, BLUE);
    DrawCircle(xPos, yPos, 10, PURPLE); //draws center
}

void Entity::EnvCollision(Rectangle objArray[], int objs) {

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

bool Entity::DoubleCollision() {
    if (checkCollisionFloor() && checkCollisionX()) {
        //

    }
    return false;
}

void Entity::Update(Rectangle objArray[], int objs) {
    MoveLeftRight();
    MoveUpDown();
    UpdateHitbox();
    EnvCollision(objArray, objs);
}
//getter and setters

float Entity::getRadius() {
    return center;
}

Color Entity::getColor() {
    return color;
}

void Entity::setColor(Color newColor) {
    color = newColor;
}

int Entity::getXPos() {
    return xPos;
}

int Entity::getYPos() {
    return yPos;
}

void Entity::setXPos(int newXPos) {
    xPos = newXPos;
}

void Entity::setYPos(int newYPos) {
    yPos = newYPos;
}

float Entity::getXVelocity() {
    return xVelocity;
}

float Entity::getYVelocity() {
    return yVelocity;
}

float Entity::getEnvFloor() {
    return envFloor;
}

float Entity::getEnvCeiling() {
    return envCeiling;
}

float Entity::getEnvLeft() {
    return envLeft;
}

float Entity::getEnvRight() {
    return envRight;
}

void Entity::setXVelocity(float newXVelocity) {
    xVelocity = newXVelocity;
}

void Entity::setYVelocity(float newYVelocity) {
    yVelocity = newYVelocity;
}

Rectangle Entity::getHitbox() {
    return hitbox;
}




