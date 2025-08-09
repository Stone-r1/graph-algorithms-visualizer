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

    std::string getButtonLabel() const {
        return label;
    }
};

enum class RadiusSize {
    Small,
    Medium,
    Large,
    None
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

    void draw(const Font& font);
    void handleMouse(Vector2 mousePosition);
    bool isInSidebarDomain(Vector2 mousePosition, float currentRadius);

    bool isButtonClicked(const std::string& label);
    RadiusSize getSelectedRadiusSize() const;
    float getSelectedRadius() const;
    void flipGraphWeight();
    void weightButtonAvailable(const bool& status);
    void resetClicks();
    void resetClick(const std::string& label); // reset only "label" button 
};

#endif
