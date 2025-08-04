#include "board.h"
#include <iostream>
#include <optional>
#include "raylib.h"
#include "sidebar.h"

#define SMALL 30.0f
#define MEDIUM 50.0f
#define LARGE 70.0f

float radius = MEDIUM;
static std::optional<Vector2> firstNode = std::nullopt;
static std::optional<Vector2> lastNode = std::nullopt;
static Vector2 startNode;
static bool weightBox = false;

void handleLeftClick(Board& board, Sidebar& sidebar, Vector2 mouse) {
    if (!IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) { 
        return;
    }

    if (weightBox || board.isAlgorithmRunning()) {
        return;
    }

    std::optional<Vector2> nodePosition = board.isInNodeDomain(mouse);
    if (nodePosition) {
        std::cout << "deleted a node\n";
        board.removeNode(*nodePosition);
        return;
    }

    if (!sidebar.isInSidebarDomain(mouse, radius)) {
        board.addNode(mouse, radius);
    } else {
        sidebar.handleMouse(mouse);

        if (sidebar.isButtonClicked("Clear")) {
            board.clear();
            return;
        }

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

        if (sidebar.isButtonClicked("Dijkstra")) {
            board.runDijkstra(startNode);
            sidebar.resetClicks();
            return;
        }

        if (sidebar.isButtonClicked("Bellman-Ford")) {
            board.runBellmanFord(startNode);
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

    if (weightBox || board.isAlgorithmRunning()) {
        return;
    }

    std::optional<Vector2> nodePosition = board.isInNodeDomain(mouse);
    if (!nodePosition) {
        return;
    } 
    
    if (!firstNode) {
        firstNode = nodePosition;
        std::cout << "Node 1 was chosen successfully\n";
    } else if (firstNode->x == nodePosition->x && firstNode->y == nodePosition->y) {
        // so basically If you clicked on the same node twice
        std::cout << "Selected the starting node.\n";
        startNode = *firstNode;
        firstNode = std::nullopt;
        board.highlightStartingNode(startNode);
    } else {
        lastNode = nodePosition;
        if (board.isGraphWeighted()) {
            weightBox = true;
        } else {
            std::cout << "Added the edge between Node 1 and Node2\n";
            board.addEdge(*firstNode, *lastNode, 1);
            firstNode = std::nullopt; 
            lastNode = std::nullopt;
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
            if (board.isGraphWeighted()) {
                board.drawWeights();
            }

            if (weightBox) {
                board.askForWeight();
                if (board.isWeightReady()) {
                    int weight = board.getCurrentWeight();
                    board.addEdge(*firstNode, *lastNode, weight);

                    weightBox = false;
                    firstNode = std::nullopt;
                    lastNode = std::nullopt;
                    board.setWeightReady();
                }
            }

        EndDrawing();
    }

    CloseWindow();
}
