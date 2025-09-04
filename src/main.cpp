#include "board.h"
#include <iostream>
#include "sidebar.h"
#include "data/constants.h"

// TODO: Make general pattern for clicking
float radius;
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

    if (!sidebar.isInSidebarDomain(mouse, radius) &&
            !board.isInBoardBorderDomain(mouse, radius, GetScreenWidth(), GetScreenHeight())) {
        board.addNode(mouse, radius);
    } else {
        sidebar.handleMouse(mouse);

        if (sidebar.isButtonClicked(LabelNames::CLEAR)) {
            board.clear();
            sidebar.resetClicks();
            return;
        }
        
        if (sidebar.isButtonClicked(LabelNames::BFS)) {
            if (!board.runBFS(startNode)) {
                sidebar.resetClick(LabelNames::BFS);
            }

            return;
        }

        if (sidebar.isButtonClicked(LabelNames::DFS)) {
            if (!board.runDFS(startNode)) {
                sidebar.resetClick(LabelNames::DFS);
            }

            return;
        }

        if (sidebar.isButtonClicked(LabelNames::DIJKSTRA)) {
            if (!board.runDijkstra(startNode)) {
                sidebar.resetClick(LabelNames::DIJKSTRA);
            }

            return;
        }

        if (sidebar.isButtonClicked(LabelNames::BELLMAN_FORD)) {
            if (!board.runBellmanFord(startNode)) {
                sidebar.resetClick(LabelNames::BELLMAN_FORD);
            }

            return;
        }

        if (sidebar.isButtonClicked(LabelNames::WEIGHTED)) {
            sidebar.weightButtonAvailable(board.isGraphEmpty());
            if (!board.isGraphEmpty()) {
                return; // can't flip the graph weight if edges were added.
            } else {
                board.flipGraphWeight();
                if (!board.isGraphWeighted()) {
                    sidebar.flipGraphWeight();
                }

                std::cout << "karoche graph weight is now set to " << (board.isGraphWeighted() ? "true\n" : "false\n");
            }

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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1200, 800, "Graph Visualizer");
    SetTargetFPS(60);

    Board board;
    Sidebar sidebar(GetScreenHeight());
    Vector2 selectedNode = {-1, -1};
    Font myFont = LoadFont("assets/RobotoRegular.ttf");

    if (!myFont.texture.id) {
        std::cerr << "Failed to load font!\n";
    }

    while (!WindowShouldClose()) { 
        Vector2 mouse = GetMousePosition();
        handleLeftClick(board, sidebar, mouse);
        handleRightClick(board, sidebar, mouse);

        // Sync radius with currently selected sidebar option
        radius = sidebar.getSelectedRadius();


        if (IsKeyPressed(KEY_RIGHT)) {
            board.stepForward();
        }

        if (IsKeyPressed(KEY_LEFT)) {
            board.stepBackward();
        }

        if (IsKeyPressed(KEY_R)) {
            board.resetRunning();
            sidebar.resetClicks();
            board.highlightStartingNode(startNode);
        }

        BeginDrawing();
            ClearBackground(ColorConstants::BOARD_BACKGROUND);
            sidebar.draw(myFont); 
            board.drawEdges();
            board.drawNodes();
            if (board.isGraphWeighted()) {
                board.drawWeights(myFont);
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
    
    UnloadFont(myFont);
    CloseWindow();
}
