#include "board.h"
#include <iostream>
#include "raylib.h"
#include "sidebar.h"

const float radius = 50.0f;

int main() {
    int monitor = GetCurrentMonitor();
    InitWindow(1200, 800, "Graph Visualizer");
    SetWindowMinSize(1200, 800);
    SetTargetFPS(60);

    Board board;
    Sidebar sidebar(GetScreenHeight());
    Vector2 selectedNode = {-1, -1};

    while (!WindowShouldClose()) { 
        Vector2 mouse = GetMousePosition();
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            sidebar.handleMouse(mouse);
            if (!sidebar.isInSidebarDomain(mouse, radius)) {
                board.addNode(mouse, radius);
            }
        }

        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            Vector2 nodePosition = board.isInNodeDomain(mouse);
            if (nodePosition.x != 0.0f && nodePosition.y != 0.0f) {
                if (selectedNode.x == -1) {
                    selectedNode.x = nodePosition.x;
                    selectedNode.y = nodePosition.y;
                    std::cout << "Node 1 was chosen successfully\n";
                } else {
                    std::cout << "Added the edge between Node 1 and Node2\n";
                    board.addEdge(selectedNode, nodePosition);
                    selectedNode = {-1, -1};
                }
            }
        }

        BeginDrawing();
            ClearBackground(WHITE);
            sidebar.draw();
            board.drawNodes();
            board.drawEdges();

        EndDrawing();
    }

    CloseWindow();
}

