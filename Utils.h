//
// Created by oreil on 4/7/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include "Entity.h"
#include <string>

//MAIN DEBUGGING
inline void showTime(int x, int y, int fontSize, Color color) {
    const char* timeStr = std::to_string(GetTime()).c_str();


    DrawText("Time: ", x, y, fontSize, color);
    DrawText(timeStr, x+ (fontSize * 3), y, fontSize, color);
}

inline void showFPS(int x, int y, int fontSize, Color color) {
    int fps = GetFPS();
    const char* fpsStr = std::to_string(fps).c_str();
    Color fpsColor;
    if (fps >= 60) fpsColor = GREEN;
    else if (fps >= 35) fpsColor = YELLOW;
    else fpsColor = RED;

    DrawText("FPS: ", x, y, fontSize, color);
    DrawText(fpsStr, x + (3 * fontSize), y, fontSize, fpsColor);
}

inline void showDebugMain(int x, int y, int fontSize, Color color) {
    int lines = 2;

    showTime(x, y, fontSize, color);
    showFPS(x, y + fontSize, fontSize, color);

    Rectangle border = Rectangle{(float)(x - (fontSize / 4)), (float)(y - (fontSize / 4)),
        (float)(fontSize * 10), (float)((lines * fontSize) + (fontSize / 2))};

    DrawRectangleLinesEx(border, 3, color);
}

//ENTITY DEBUGGING
inline void showPosition(Entity ent, Vector2 pos, int fontSize, Color color) {
    const char* xStr = std::to_string(ent.getXPos()).c_str();
    const char* yStr = std::to_string(ent.getYPos()).c_str();

    DrawText("xPos: ", pos.x, pos.y, fontSize, color);
    DrawText(xStr, pos.x + (4 * fontSize), pos.y, fontSize, color);

    DrawText("yPos: ", pos.x, pos.y + fontSize, fontSize, color);
    DrawText(yStr, pos.x + (4 * fontSize), pos.y + fontSize, fontSize, color);
};
inline void showVelocity(Entity ent, Vector2 pos, int fontSize, Color color) {
    const char* xVelocityStr = std::to_string(ent.getXVelocity()).c_str();
    const char* yVelocityStr = std::to_string(ent.getYVelocity()).c_str();

    DrawText("xVel: ", pos.x, pos.y, fontSize, color);
    DrawText(xVelocityStr, pos.x + (4 * fontSize), pos.y, fontSize, color);

    DrawText("yVel: ", pos.x, pos.y + fontSize, fontSize, color);
    DrawText(yVelocityStr, pos.x + (4 * fontSize), pos.y + fontSize, fontSize, color);
};
inline void showCollision(Entity ent, Vector2 pos, int fontSize, Color color) {
    const char* xCollisionStr = std::to_string(ent.checkCollisionX()).c_str();
    if (*xCollisionStr == '7') xCollisionStr = "Left";
    if (*xCollisionStr == '8') xCollisionStr = "Right";
    if (*xCollisionStr == '3') xCollisionStr = "None";
    const char* floorCollisionStr = ent.checkCollisionFloor() ? "true" : "false";
    const char* ceilingCollisionStr = ent.checkCollisionCeiling() ? "true" : "false";

    DrawText("X Col: ", pos.x, pos.y, fontSize, color);
    DrawText("Floor Col: ", pos.x, pos.y + fontSize, fontSize, color);
    DrawText("Ceiling Col: ", pos.x, pos.y + (2 * fontSize), fontSize, color);

    DrawText(xCollisionStr, pos.x + (4 * fontSize), pos.y, fontSize, color);
    DrawText(floorCollisionStr, pos.x + (7 * fontSize), pos.y + fontSize, fontSize, color);
    DrawText(ceilingCollisionStr, pos.x + (7 * fontSize), pos.y + (2 * fontSize), fontSize, color);
};
inline void showEnvFloor (Entity ent, Vector2 pos, int fontSize, Color color) {
    const char* envFlStr = std::to_string(ent.getEnvFloor()).c_str();


    DrawText("Entity Env Floor: ", 0, 240, 20, BLACK);
    DrawText(envFlStr, 190, 240, 20, BLACK);
    if (ent.getEnvFloor() == GetScreenHeight()) {
        const char* envIsScreenHeight = "(Screen Height)";
        DrawText(envIsScreenHeight, 310, 240, 20, BLACK);
    }
}
inline void showEnvCeiling (Entity ent, Vector2 pos, int fontSize, Color color) {
    const char* envClStr = std::to_string(ent.getEnvCeiling()).c_str();


    DrawText("Entity Env Ceiling: ", 0, 260, 20, BLACK);
    DrawText(envClStr, 190, 260, 20, BLACK);
    if (ent.getEnvFloor() == 0) {
        const char* envIsScreenHeight = "(Screen Height)";
        DrawText(envIsScreenHeight, 310, 260, 20, BLACK);
    }
}
inline void showEnvLeft (Entity ent, Vector2 pos, int fontSize, Color color) {
    const char* envClStr = std::to_string(ent.getEnvLeft()).c_str();


    DrawText("Entity Env Left: ", 0, 280, 20, BLACK);
    DrawText(envClStr, 190, 280, 20, BLACK);
}
inline void showEnvRight (Entity ent) {
    const char* envClStr = std::to_string(ent.getEnvRight()).c_str();


    DrawText("Entity Env Right: ", 0, 300, 20, BLACK);
    DrawText(envClStr, 190, 300, 20, BLACK);
}
inline void showDebugEntity(Entity ent, Vector2 pos, int fontSize, Color color) {
    const int lines = 7;


    Rectangle border = Rectangle{pos.x - (float)(fontSize / 4), pos.y - (float)(fontSize / 4),
        (float)(fontSize * 10), (float)((lines * fontSize) + (fontSize / 2))};

    DrawRectangleRec(border, LIGHTGRAY);

    showPosition(ent, pos, fontSize, color);
    pos.y += fontSize * 2;
    showVelocity(ent, pos, fontSize, color);
    pos.y += fontSize * 2;
    showCollision(ent, pos, fontSize, color);

    ent.DrawHitbox();
}


#endif //UTILS_H
