#ifndef SIDEBAR_H
#define SIDEBAR_H

#include "raylib.h"
#include <string>
#include <vector>


struct Button {
    Rectangle domain;
    std::string label;
    bool clicked;

    Button(Rectangle rect, const std::string& str)
        : domain(rect), label(str), clicked(false) {}
};


class Sidebar {
private:
    int x;
    int width;
    int height;

    std::vector<Button> buttons;

public:
    Sidebar(int screenHeight);
    
    int getSidebarX() const;
    int getSidebarWidth() const;
    int getSidebarHeight() const;

    void draw();
    void handleMouse(Vector2 mousePosition);

    bool isButtonClicked(const std::string& label);
    void resetClick();
};

#endif
