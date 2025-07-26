#include "board.h"
#include <iostream>
#include "raylib.h"
#include "sidebar.h"

#define SMALL 30.0f
#define MEDIUM 50.0f
#define LARGE 70.0f

float radius = MEDIUM;
static Vector2 firstNode = {-1, -1}, lastNode = {-1, -1};
static Vector2 startNode;
static bool weightBox = false;

void handleLeftClick(Board& board, Sidebar& sidebar, Vector2 mouse) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { 
        return;
    }

    if (weightBox) {
        return;
    }

    if (!sidebar.isInSidebarDomain(mouse, radius)) {
        board.addNode(mouse, radius);
    } else {
        sidebar.handleMouse(mouse);

        if (sidebar.isButtonClicked("Small")) {
            radius = SMALL;
            sidebar.resetClicks();
            return;
        }

        if (sidebar.isButtonClicked("Medium")) {
            radius = MEDIUM; 
            sidebar.resetClicks();
            return;
        }

        if (sidebar.isButtonClicked("Large")) {
            radius = LARGE;
            sidebar.resetClicks();
            return;
        }
        
        if (sidebar.isButtonClicked("BFS")) {
            board.runBFS(startNode);
            sidebar.resetClicks();
            return;
        }

        if (sidebar.isButtonClicked("DFS")) {
            board.runDFS(startNode);
            sidebar.resetClicks();
            return;
        }

        if (sidebar.isButtonClicked("Weighted")) {
            if (!board.isGraphEmpty()) {
                return; // can't flip the graph weight if edges were added.
            } else {
                board.flipGraphWeight();
                std::cout << "karoche graph weight is now set to " << (board.isGraphWeighted() ? "true\n" : "false\n");
            }

            sidebar.resetClicks();
            return;
        }
    } 
}

void handleRightClick(Board& board, Sidebar& sidebar, Vector2 mouse) {
    if (!IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
        return; 
    }

    if (weightBox) {
        return;
    }

    Vector2 nodePosition = board.isInNodeDomain(mouse);
    if (nodePosition.x == 0.0f && nodePosition.y == 0.0f) {
        return;
    }

    if (firstNode.x == -1) {
        firstNode = nodePosition;
        std::cout << "Node 1 was chosen successfully\n";
    } else if (firstNode.x == nodePosition.x && firstNode.y == nodePosition.y) {
        // so basically If you clicked on the same node twice
        std::cout << "Selected the starting node.\n";
        startNode = firstNode;
        board.highlightStartingNode(startNode);
        firstNode = {-1, -1};
    } else {
        lastNode = nodePosition;
        if (board.isGraphWeighted()) {
            weightBox = true;
        } else {
            std::cout << "Added the edge between Node 1 and Node2\n";
            board.addEdge(firstNode, lastNode, 1);
            firstNode = {-1, -1}; 
            lastNode = firstNode;
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
        handleRightClick(board, sidebar, mouse);

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

            if (weightBox) {
                board.askForWeight();
                if (board.isWeightReady()) {
                    int weight = board.getCurrentWeight();
                    board.addEdge(firstNode, lastNode, weight);

                    weightBox = false;
                    firstNode = {-1, -1};
                    lastNode = firstNode;
                    board.setWeightReady();
                }
            }

        EndDrawing();
    }

    CloseWindow();
}
