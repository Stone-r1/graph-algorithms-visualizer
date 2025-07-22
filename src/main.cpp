#include "board.h"
#include <iostream>
#include "raylib.h"
#include "sidebar.h"

#define SMALL 30.0f
#define MEDIUM 50.0f
#define LARGE 70.0f

float radius = MEDIUM;
Vector2 startNode;

void handleLeftClick(Board& board, Sidebar& sidebar, Vector2 mouse) {
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { 
        if (!sidebar.isInSidebarDomain(mouse, radius)) {
            board.addNode(mouse, radius);
        } else {
            sidebar.handleMouse(mouse);

            if (sidebar.isButtonClicked("Small")) radius = SMALL;
            if (sidebar.isButtonClicked("Medium")) radius = MEDIUM; 
            if (sidebar.isButtonClicked("Large")) radius = LARGE;
            
            if (sidebar.isButtonClicked("BFS")) {
                board.runBFS(startNode);
            }
            if (sidebar.isButtonClicked("DFS")) {
                board.runDFS(startNode);
            }
            sidebar.resetClicks();
        }
    }
}

void handleRightClick(Board& board, Sidebar& sidebar, Vector2 mouse, Vector2& selectedNode) {
    if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        Vector2 nodePosition = board.isInNodeDomain(mouse);
        if (nodePosition.x != 0.0f && nodePosition.y != 0.0f) {
            if (selectedNode.x == -1) {
                selectedNode.x = nodePosition.x;
                selectedNode.y = nodePosition.y;
                std::cout << "Node 1 was chosen successfully\n";
            } else if (selectedNode.x == nodePosition.x && selectedNode.y == nodePosition.y) {
                // so basically If you clicked on the same node twice
                std::cout << "Selected the starting node.\n";
                startNode = selectedNode;
                selectedNode = {-1, -1};
            } else {
                std::cout << "Added the edge between Node 1 and Node2\n";
                board.addEdge(selectedNode, nodePosition);
                selectedNode = {-1, -1};
            }
        }
    }
}

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
        handleLeftClick(board, sidebar, mouse);
        handleRightClick(board, sidebar, mouse, selectedNode);

        if (IsKeyPressed(KEY_RIGHT)) {
            board.stepForward();
        }

        if (IsKeyPressed(KEY_LEFT)) {
            board.stepBackward();
        }

        if (IsKeyPressed(KEY_R)) {
            board.resetRunning();
        }

        BeginDrawing();
            ClearBackground(WHITE);
            sidebar.draw();
            board.drawEdges();
            board.drawNodes();

        EndDrawing();
    }

    CloseWindow();
}
