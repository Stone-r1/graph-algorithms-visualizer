#include "raylib.h"

class Node {
private:
    int nodeNumber;
    Vector2 nodePosition;
    bool isActive;

public:
    Node();
    
    int getNode() const;
    bool isNodeActive() const;

    void setNodePosition(Vector2 position);
    Vector2 getNodePosition(Vector2 position);

    int isInRadiusDomain(Vector2 mouse);
    void drawNode();
};
