#include "raylib.h"
#include "sidebar.h"
#include <vector>
#include <string>

const float margin = 10.0f;

Sidebar::Sidebar(int screenHeight) :
    x(0), 
    width(250),
    height(screenHeight)
{
    float buttonHeight = 60.0f;
    float yOffset = margin;
    std::vector<std::string> labels = {"Run", "Pause", "Stop", "DFS", "BFS", "Dijkstra", "Bellman-Ford"};

    for (const auto& label : labels) {
        buttons.emplace_back(Rectangle{(float)x + margin, yOffset, (float)width - 2 * margin, (float)buttonHeight}, label);
        yOffset += (buttonHeight + margin);
    }
}

void Sidebar::draw() {
    DrawRectangle(x, 0, width, height, GRAY);

    for (const auto& button : buttons) {
        DrawRectangleRec(button.domain, LIGHTGRAY);
        DrawRectangleLinesEx(button.domain, 2, DARKGRAY);
        DrawText(button.label.c_str(), button.domain.x + margin, button.domain.y + margin, 18, BLACK);
    }
}
