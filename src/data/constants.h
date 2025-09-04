#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
#include "raylib.h"

namespace UIConstants {
    inline constexpr float MARGIN = 10.0f;
    inline constexpr int FONT_SIZE_EDGE = 30;
    inline constexpr int FONT_SIZE_SIDEBAR = 30;
    inline constexpr int FONT_SIZE_NODE = 40;
    inline constexpr float SIDEBAR_WIDTH = 250.0f;
    inline constexpr float SIDEBAR_HEIGHT = 800.0f;
    inline constexpr float BUTTON_HEIGHT = 60.0f;
    inline constexpr float WEIGHT_OFFSET = 20.0f;

    inline constexpr float MARGIN_FROM_NODE = 3.0f;
    inline constexpr float MARGIN_FROM_SIDEBAR = 2.0f;
    inline constexpr float MARGIN_FROM_BORDER = 1.5f;
}

namespace ColorConstants {
    // Graph 
    inline constexpr Color NODE_DEFAULT = BLUE;
    inline constexpr Color NODE_START = ORANGE;
    inline constexpr Color NODE_VISITED = RED;
    inline constexpr Color EDGE_DEFAULT = GREEN;
    inline constexpr Color EDGE_HIGHLIGHT = DARKBLUE;

    // Sidebar
    inline constexpr Color SIDEBAR_BUTTON_ACTIVE = GREEN;
    inline constexpr Color SIDEBAR_BUTTON_INACTIVE = LIGHTGRAY;
    inline constexpr Color SIDEBAR_BORDER = DARKGRAY;
    inline constexpr Color SIDEBAR_BACKGROUND = GRAY;

    inline constexpr Color TEXT = BLACK;
    inline constexpr Color WEIGHT_BOX_BORDER = DARKGRAY;
    inline constexpr Color WEIGHT_BOX_BACKGROUND = {254, 248, 221, 255};
    inline constexpr Color BOARD_BACKGROUND = WHITE;
}

namespace NodeConstants {
    inline constexpr int MAX_NODES = 1000;
    inline constexpr float SMALL_RADIUS = 30.0f;
    inline constexpr float MEDIUM_RADIUS = 50.0f;
    inline constexpr float LARGE_RADIUS = 70.0f;
}

namespace LabelNames {
    // Algorithm Labels
    inline const std::string CLEAR = "Clear";
    inline const std::string WEIGHTED = "Weighted";
    inline const std::string DFS = "DFS";
    inline const std::string BFS = "BFS";
    inline const std::string DIJKSTRA = "Dijkstra";
    inline const std::string BELLMAN_FORD = "Bellman-Ford";
    
    // Size Labels
    inline const std::string SMALL = "S";
    inline const std::string MEDIUM = "M";
    inline const std::string LARGE = "L";
}

#endif
