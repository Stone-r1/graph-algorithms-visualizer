#include "raylib.h"
#include "sidebar.h"
#include "data/constants.h"
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>

float topButtons;

// TODO: Replace this ugly block later
// TODO: Replace UI and Functionality
static const std::unordered_map<RadiusSize, float> RadiusValues = {
    {RadiusSize::Small, NodeConstants::SMALL_RADIUS},
    {RadiusSize::Medium, NodeConstants::MEDIUM_RADIUS},
    {RadiusSize::Large, NodeConstants::LARGE_RADIUS}
};

static RadiusSize labelToRadiusEnum(const std::string& label) {
    if (label == LabelNames::SMALL) return RadiusSize::Small;
    if (label == LabelNames::MEDIUM) return RadiusSize::Medium;
    if (label == LabelNames::LARGE) return RadiusSize::Large;
    return RadiusSize::None;
}

Sidebar::Sidebar(int screenHeight) :
    x(0), 
    width(UIConstants::SIDEBAR_WIDTH),
    height(UIConstants::SIDEBAR_HEIGHT),
    ystart(screenHeight - 6.0f * screenHeight / 7)
{
    using namespace UIConstants;

    float buttonHeight = 60.0f;
    float buttonWidth = width - 2 * MARGIN;
    float buttonX = x + (width - buttonWidth) / 2;
    float yOffset = ystart + 3 * MARGIN;

    std::vector<std::string> labels = {
        LabelNames::CLEAR,
        LabelNames::WEIGHTED,
        LabelNames::DFS,
        LabelNames::BFS,
        LabelNames::DIJKSTRA,
        LabelNames::BELLMAN_FORD
    };

    for (const auto& label : labels) {
        buttons.emplace_back(Rectangle{buttonX + MARGIN, yOffset, buttonWidth, buttonHeight}, label);
        yOffset += (buttonHeight + MARGIN);
    }

    topButtons = yOffset;
    yOffset += 2.5 * MARGIN;
    std::vector<std::string> radiusLabels = {
        LabelNames::SMALL,
        LabelNames::MEDIUM,
        LabelNames::LARGE
    };

    buttonWidth = (width - 4 * MARGIN) / 3.0f;  // 3 buttons + 2 gaps = 4 margins
    float totalWidth = 3 * buttonWidth + 2 * MARGIN;
    float baseX = x + (width - totalWidth) / 2 + MARGIN;
    float radiusY = yOffset;

    for (int i = 0; i < radiusLabels.size(); ++i) {
        float buttonX = baseX + i * (buttonWidth + MARGIN);
        radiuses.emplace_back(Rectangle{buttonX, radiusY, buttonWidth, buttonHeight}, radiusLabels[i]);
        if (radiusLabels[i] == LabelNames::MEDIUM) {
            // default value
            radiuses[i].isClicked = true;
        }
    }

    yOffset += buttonHeight + MARGIN; 
}

void Sidebar::draw(const Font& font) {
    using namespace UIConstants;
    using namespace ColorConstants;

    float sidebarHeight = height - 2 * ystart;
    Rectangle rect = {x + MARGIN, ystart, width, sidebarHeight};

    DrawRectangleRounded(rect, 0.2f, 1, SIDEBAR_BACKGROUND);
    Rectangle temp = {x + MARGIN + 1, ystart, width - 2, sidebarHeight - 2}; // 1 pixel was playing with my nerves...
    DrawRectangleRoundedLinesEx(temp, 0.2f, 1, 5, SIDEBAR_BORDER);

    for (const auto& button : buttons) {
        DrawRectangleRec(button.domain, isButtonClicked(button.getButtonLabel()) ? SIDEBAR_BUTTON_ACTIVE : SIDEBAR_BUTTON_INACTIVE);
        DrawRectangleLinesEx(button.domain, 2, SIDEBAR_BORDER);

        Vector2 textSize = MeasureTextEx(font, button.label.c_str(), FONT_SIZE_SIDEBAR, 1);
        Vector2 pos = {
            button.domain.x + (button.domain.width - textSize.x) / 2,
            button.domain.y + (button.domain.height - textSize.y) / 2
        };
        DrawTextEx(font, button.label.c_str(), pos, FONT_SIZE_SIDEBAR, 1, TEXT); 
    }

    DrawLineEx({x + 3 * MARGIN, topButtons + MARGIN}, {(float)width - MARGIN - MARGIN / 2, topButtons + MARGIN}, 5.0f, SIDEBAR_BORDER);

    for (const auto& button : radiuses) {
        DrawRectangleRec(button.domain, isButtonClicked(button.getButtonLabel()) ? SIDEBAR_BUTTON_ACTIVE : SIDEBAR_BUTTON_INACTIVE);
        DrawRectangleLinesEx(button.domain, 2, SIDEBAR_BORDER);

        Vector2 textSize = MeasureTextEx(font, button.label.c_str(), FONT_SIZE_SIDEBAR, 1);
        Vector2 pos = {
            button.domain.x + (button.domain.width - textSize.x) / 2,
            button.domain.y + (button.domain.height - textSize.y) / 2
        };
        DrawTextEx(font, button.label.c_str(), pos, FONT_SIZE_SIDEBAR, 1, TEXT);
    }
}

void Sidebar::handleMouse(Vector2 mousePosition) {
    if (mousePosition.x > width || mousePosition.y > height) {
        return;
    }

    for (auto& button : buttons) {
        const Rectangle& rect = button.domain;

        if (button.getButtonLabel() != LabelNames::WEIGHTED) {
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
        if (button.getButtonLabel() == LabelNames::WEIGHTED) {
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
        if (button.getButtonLabel() == LabelNames::WEIGHTED) {
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
        if (button.getButtonLabel() == LabelNames::MEDIUM) {
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
