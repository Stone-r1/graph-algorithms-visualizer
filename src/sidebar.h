#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "raylib.h"
#include <string>
#include <vector>


struct Button {
    Rectangle domain;
    std::string label;
    bool isClicked;

    Button(Rectangle rect, const std::string& str)
        : domain(rect), label(str), isClicked(false) {}
};


class Sidebar {
private:
    float x;
    float width;
    float height;
    float ystart;

    std::vector<Button> buttons;
    std::vector<Button> radiuses;

public:
    Sidebar(int screenHeight); 

    void draw();
    void handleMouse(Vector2 mousePosition);
    bool isInSidebarDomain(Vector2 mousePosition, float currentRadius);

    bool isButtonClicked(const std::string& label);
    void resetClicks();
};

#endif
