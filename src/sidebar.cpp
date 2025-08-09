#include "raylib.h"
#include "sidebar.h"
#include "constants.h"
#include <vector>
#include <iostream>
#include <string>
#include <map>

inline constexpr float SMALL = 30.0f;
inline constexpr float MEDIUM = 50.0f;
inline constexpr float LARGE = 70.0f;

const float margin = 10.0f;
const int fontSize = 30;
float topButtons;

static const std::map<RadiusSize, float> RadiusValues = {
    {RadiusSize::Small, SMALL},
    {RadiusSize::Medium, MEDIUM},
    {RadiusSize::Large, LARGE}
};

static RadiusSize labelToRadiusEnum(const std::string& label) {
    if (label == SidebarLabelNames::Small) return RadiusSize::Small;
    if (label == SidebarLabelNames::Medium) return RadiusSize::Medium;
    if (label == SidebarLabelNames::Large) return RadiusSize::Large;
    return RadiusSize::None;
}

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

    std::vector<std::string> labels = {
        SidebarLabelNames::Clear,
        SidebarLabelNames::Weighted,
        SidebarLabelNames::DFS,
        SidebarLabelNames::BFS,
        SidebarLabelNames::Dijkstra,
        SidebarLabelNames::BellmanFord
    };

    for (const auto& label : labels) {
        buttons.emplace_back(Rectangle{buttonX + margin, yOffset, buttonWidth, buttonHeight}, label);
        yOffset += (buttonHeight + margin);
    }

    topButtons = yOffset;
    yOffset += 2.5 * margin;
    std::vector<std::string> radiusLabels = {
        SidebarLabelNames::Small,
        SidebarLabelNames::Medium,
        SidebarLabelNames::Large
    };

    buttonWidth = (width - 4 * margin) / 3.0f;  // 3 buttons + 2 gaps = 4 margins
    float totalWidth = 3 * buttonWidth + 2 * margin;
    float baseX = x + (width - totalWidth) / 2 + margin;
    float radiusY = yOffset;

    for (int i = 0; i < radiusLabels.size(); ++i) {
        float buttonX = baseX + i * (buttonWidth + margin);
        radiuses.emplace_back(Rectangle{buttonX, radiusY, buttonWidth, buttonHeight}, radiusLabels[i]);
        if (radiusLabels[i] == SidebarLabelNames::Medium) {
            // default value
            radiuses[i].isClicked = true;
        }
    }

    yOffset += buttonHeight + margin; 
}

void Sidebar::draw(const Font& font) {
    float sidebarHeight = height - 2 * ystart;
    Rectangle rect = {x + margin, ystart, width, sidebarHeight};

    DrawRectangleRounded(rect, 0.2f, 1, GRAY);
    Rectangle temp = {x + margin + 1, ystart, width - 2, sidebarHeight - 2}; // 1 pixel was playing with my nerves...
    DrawRectangleRoundedLinesEx(temp, 0.2f, 1, 5, DARKGRAY);

    for (const auto& button : buttons) {
        DrawRectangleRec(button.domain, isButtonClicked(button.getButtonLabel()) ? GREEN : LIGHTGRAY);
        DrawRectangleLinesEx(button.domain, 2, DARKGRAY);

        Vector2 textSize = MeasureTextEx(font, button.label.c_str(), fontSize, 1);
        Vector2 pos = {
            button.domain.x + (button.domain.width - textSize.x) / 2,
            button.domain.y + (button.domain.height - textSize.y) / 2
        };
        DrawTextEx(font, button.label.c_str(), pos, fontSize, 1, BLACK); 
    }

    DrawLineEx({x + 3 * margin, topButtons + margin}, {(float)width - margin - margin / 2, topButtons + margin}, 5.0f, DARKGRAY);

    for (const auto& button : radiuses) {
        DrawRectangleRec(button.domain, isButtonClicked(button.getButtonLabel()) ? GREEN : LIGHTGRAY);
        DrawRectangleLinesEx(button.domain, 2, DARKGRAY);

        Vector2 textSize = MeasureTextEx(font, button.label.c_str(), fontSize, 1);
        Vector2 pos = {
            button.domain.x + (button.domain.width - textSize.x) / 2,
            button.domain.y + (button.domain.height - textSize.y) / 2
        };
        DrawTextEx(font, button.label.c_str(), pos, fontSize, 1, BLACK);
    }
}

void Sidebar::handleMouse(Vector2 mousePosition) {
    if (mousePosition.x > width || mousePosition.y > height) {
        return;
    }

    for (auto& button : buttons) {
        const Rectangle& rect = button.domain;

        if (button.getButtonLabel() != SidebarLabelNames::Weighted) {
            button.isClicked = false;
        }

        if (CheckCollisionPointRec(mousePosition, rect)) {
            std::cout << "Is In Domain of " << button.label << '\n';
            button.isClicked = true;
            return;
        }
    }

    for (auto& button : radiuses) {
        const Rectangle& rect = button.domain;
        button.isClicked = false;
        if (CheckCollisionPointRec(mousePosition, rect)) {
            std::cout << "Is In Domain of " << button.label << '\n';

            for (auto& b : radiuses) {
                b.isClicked = false;
            }

            button.isClicked = true;
            return;
        }
    }
}

void Sidebar::weightButtonAvailable(const bool& status) {
    for (auto& button : buttons) {
        if (button.getButtonLabel() == SidebarLabelNames::Weighted) {
            button.isClicked = status;
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

RadiusSize Sidebar::getSelectedRadiusSize() const {
    for (const auto& button : radiuses) {
        if (button.isClicked) {
            return labelToRadiusEnum(button.getButtonLabel());
        }
    }

    return RadiusSize::None;
}

void Sidebar::flipGraphWeight() {
    for (auto& button : buttons) {
        if (button.getButtonLabel() == SidebarLabelNames::Weighted) {
            button.isClicked = !button.isClicked;
            return;
        }
    }
}

float Sidebar::getSelectedRadius() const {
    RadiusSize selected = getSelectedRadiusSize();
    auto it = RadiusValues.find(selected);
    return (it != RadiusValues.end() ? it->second : 0.0f);
}

void Sidebar::resetClicks() {
    for (auto& button : radiuses) {
        if (button.getButtonLabel() == SidebarLabelNames::Medium) {
            button.isClicked = true;
            continue;
        }

        button.isClicked = false;
    } 

    for (auto& button : buttons) {
        button.isClicked = false;
    }
}

void Sidebar::resetClick(const std::string& label) {
    for (auto& button : buttons) {
        if (button.getButtonLabel() == label) {
            button.isClicked = false;
            break;
        }
    }
}
