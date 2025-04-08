//
// Created by oreil on 4/7/2025.
//

#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include "Entity.h"
#include <string>

//main debuggeres
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

    Rectangle border = Rectangle{x - (fontSize / 4), y - (fontSize / 4),
        fontSize * 10, (lines * fontSize) + (fontSize / 2)};

    DrawRectangleLinesEx(border, 3, color);
}

//need to mofidy these to take in positions
inline void showPosition(Entity ent) {
    const char* xStr = std::to_string(ent.getXPos()).c_str();
    const char* yStr = std::to_string(ent.getYPos()).c_str();
    DrawText("xPos: ", 0, 0, 20, BLACK);
    DrawText("yPos: ", 0, 20, 20, BLACK);
    DrawText(xStr, 60, 0, 20, BLACK);
    DrawText(yStr, 60, 20, 20, BLACK);
};
inline void showVelocity(Entity ent) {
    const char* xVelocityStr = std::to_string(ent.getXVelocity()).c_str();
    const char* yVelocityStr = std::to_string(ent.getYVelocity()).c_str();
    DrawText("xVelocity: ", 0, 60, 20, BLACK);
    DrawText("yVelocity: ", 0, 80, 20, BLACK);
    DrawText(xVelocityStr, 110, 60, 20, BLACK);
    DrawText(yVelocityStr, 110, 80, 20, BLACK);
};
inline void showCollision(Entity ent) {
    const char* xCollisionStr = std::to_string(ent.checkCollisionX()).c_str();
    const char* floorCollisionStr = ent.checkCollisionFloor() ? "true" : "false";
    const char* ceilingCollisionStr = ent.checkCollisionCeiling() ? "true" : "false";

    DrawText("X Collision: ", 0, 120, 20, BLACK);
    DrawText("Floor Collision: ", 0, 140, 20, BLACK);
    DrawText("Ceiling Collision: ", 0, 160, 20, BLACK);

    DrawText(xCollisionStr, 115, 120, 20, BLACK);
    DrawText(floorCollisionStr, 165, 140, 20, BLACK);
    DrawText(ceilingCollisionStr, 165, 160, 20, BLACK);
};
inline void showEnvFloor (Entity ent) {
    const char* envFlStr = std::to_string(ent.getEnvFloor()).c_str();


    DrawText("Entity Env Floor: ", 0, 240, 20, BLACK);
    DrawText(envFlStr, 190, 240, 20, BLACK);
    if (ent.getEnvFloor() == GetScreenHeight()) {
        const char* envIsScreenHeight = "(Screen Height)";
        DrawText(envIsScreenHeight, 310, 240, 20, BLACK);
    }
}
inline void showEnvCeiling (Entity ent) {
    const char* envClStr = std::to_string(ent.getEnvCeiling()).c_str();


    DrawText("Entity Env Ceiling: ", 0, 260, 20, BLACK);
    DrawText(envClStr, 190, 260, 20, BLACK);
    if (ent.getEnvFloor() == 0) {
        const char* envIsScreenHeight = "(Screen Height)";
        DrawText(envIsScreenHeight, 310, 260, 20, BLACK);
    }
}
inline void showEnvLeft (Entity ent) {
    const char* envClStr = std::to_string(ent.getEnvLeft()).c_str();


    DrawText("Entity Env Left: ", 0, 280, 20, BLACK);
    DrawText(envClStr, 190, 280, 20, BLACK);
}
inline void showEnvRight (Entity ent) {
    const char* envClStr = std::to_string(ent.getEnvRight()).c_str();


    DrawText("Entity Env Right: ", 0, 300, 20, BLACK);
    DrawText(envClStr, 190, 300, 20, BLACK);
}
inline void debugEntity(Entity ent) {}


#endif //UTILS_H
