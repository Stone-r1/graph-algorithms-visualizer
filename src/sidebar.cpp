#include "raylib.h"
#include "sidebar.h"
#include <vector>
#include <iostream>
#include <string>

const float margin = 10.0f;
float topButtons;

Sidebar::Sidebar(int screenHeight) :
    x(0), 
    width(250),
    height(800),
    ystart(screenHeight - 6.0f * screenHeight / 7)
{
    float buttonHeight = 60.0f;
    float buttonWidth = width - 2 * margin;
    float buttonX = x + (width - buttonWidth) / 2;
    float yOffset = ystart + 3 * margin;

    std::vector<std::string> labels = {"Clear", "Weighted", "DFS", "BFS", "Dijkstra", "Bellman-Ford"};

    for (const auto& label : labels) {
        buttons.emplace_back(Rectangle{buttonX + margin, yOffset, buttonWidth, buttonHeight}, label);
        yOffset += (buttonHeight + margin);
    }

    topButtons = yOffset;
    yOffset += 2.5 * margin;
    std::vector<std::string> radiusLabels = {"S", "M", "L"};

    buttonWidth = (width - 4 * margin) / 3.0f;  // 3 buttons + 2 gaps = 4 margins
    float totalWidth = 3 * buttonWidth + 2 * margin;
    float baseX = x + (width - totalWidth) / 2 + margin;
    float radiusY = yOffset;

    for (int i = 0; i < radiusLabels.size(); ++i) {
        float buttonX = baseX + i * (buttonWidth + margin);
        radiuses.emplace_back(Rectangle{buttonX, radiusY, buttonWidth, buttonHeight}, radiusLabels[i]);
    }

    yOffset += buttonHeight + margin; 
}

void Sidebar::draw() {
    float sidebarHeight = height - 2 * ystart;
    Rectangle rect = {x + margin, ystart, width, sidebarHeight};

    DrawRectangleRounded(rect, 0.2f, 1, GRAY);
    Rectangle temp = {x + margin + 1, ystart, width - 2, sidebarHeight - 2}; // 1 pixel was playing with my nerves...
    DrawRectangleRoundedLinesEx(temp, 0.2f, 1, 5, DARKGRAY);

    for (const auto& button : buttons) {
        DrawRectangleRec(button.domain, LIGHTGRAY);
        DrawRectangleLinesEx(button.domain, 2, DARKGRAY);
        int textWidth = MeasureText(button.label.c_str(), 18);
        DrawText(button.label.c_str(),
                 button.domain.x + (button.domain.width - textWidth) / 2,
                 button.domain.y + (button.domain.height - 18) / 2,
                 18, BLACK);
    }

    DrawLineEx({x + 3 * margin, topButtons + margin}, {(float)width - margin - margin / 2, topButtons + margin}, 5.0f, DARKGRAY);

    for (const auto& button : radiuses) {
        DrawRectangleRec(button.domain, LIGHTGRAY);
        DrawRectangleLinesEx(button.domain, 2, DARKGRAY);
        int textWidth = MeasureText(button.label.c_str(), 18);
        DrawText(button.label.c_str(),
                 button.domain.x + (button.domain.width - textWidth) / 2,
                 button.domain.y + (button.domain.height - 18) / 2,
                 18, BLACK);
    } 
}

void Sidebar::handleMouse(Vector2 mousePosition) {
    if (mousePosition.x > width || mousePosition.y > height) {
        return;
    }

    for (auto& button : buttons) {
        button.isClicked = false;
    }

    for (auto& button : buttons) {
        const Rectangle& rect = button.domain;
        button.isClicked = false;
        if (mousePosition.x > rect.x && mousePosition.x < rect.x + rect.width &&
            mousePosition.y > rect.y && mousePosition.y < rect.y + rect.height) {
            std::cout << "Is In Domain of " << button.label << '\n';
            button.isClicked = true;
        }
    } 

    for (auto& button : radiuses) {
        const Rectangle& rect = button.domain;
        if (mousePosition.x > rect.x && mousePosition.x < rect.x + rect.width &&
            mousePosition.y > rect.y && mousePosition.y < rect.y + rect.height) {
            std::cout << "Is In Domain of " << button.label << '\n';
            button.isClicked = true;
        }
    }
}

bool Sidebar::isInSidebarDomain(Vector2 mousePosition, float currentRadius) {
    if (mousePosition.x - 2 * currentRadius < width && mousePosition.y - 2 * currentRadius < height) {
        return true;
    }
    return false;
}

bool Sidebar::isButtonClicked(const std::string& label) {
    for (const auto& button : buttons) {
        if (button.label == label && button.isClicked) {
            return true;
        }
    }

    for (const auto& button : radiuses) {
        if (button.label == label && button.isClicked) {
            return true;
        }
    }

    return false;
}

void Sidebar::resetClicks() {
    for (auto& button : radiuses) {
        button.isClicked = false;
    } 

    for (auto& button : buttons) {
        button.isClicked = false;
    }
}
