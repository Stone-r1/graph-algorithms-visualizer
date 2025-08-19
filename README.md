# Graph Algorithms Visualizer

## Introduction
Graph Algorithms Visualizer is an advanced tool developed entirely in C++ that demonstrates a variety of graph algorithms through visual representation. This tool is mainly designed as a learning resource. 

---

## Algorithms
Visualizer supports the following algorithms:

- **Depth First Search**
- **Breadth First Search**
- **Dijkstra**
- **Bellman-Ford**

TODO - Add such Algorithms as A* and Floyd-Warshall.

---

## Features

### Step-by-Step Algorithm Execution
**Purpose**: The user controls the algorithm one atomic step at a time. Each step performs exactly one logical action of the chosen algorithm. The user can move forward or backward through those atomic actions.

**Controls**:
- Right Arrow Key: advance one step.
- Left Arrow Key: go back one step.
- R key (Reset): Reset clears all visitation/highlighting/algorithm data but keeps the current graph topology and chosen default weights/radius settings.

**Behavior**:
On each step highlight the current node/edge that is being processed. 
Colorscheme:

- Blue Node: Unvisited Node
- Orange Node: Node from which traversal starts
- Red Node: Visited Node

- Green Edge: Univisited Path
- Blue Edge: Visited Path

### Simple UI
**Interaction Rules**:
- Add node: Left click once on an empty area of the canvas -> new node placed at the pointer with current node radius.
- Delete node: Left click on an existing node -> node is removed (and its incident edges).
- Choose starting node: Right-click twice on a node (double-right-click) -> set that node as the start node. The start node is highlighted (and used by algorithms that require a source).
- Add edge: Right-click on node A, then right-click on node B -> create an edge between A and B. If graph is weighted (you can set that option via sidebar), user has to input weight.
- Reset algorithm: Press R (or Reset button) -> clear all algorithm state/highlights and clear active algorithm selection so the user can pick another algorithm or edit the graph. The underlying nodes/edges and GUI defaults (weights, radius) remain.
- Step controls: Use Left/Right arrow keys to step through algorithm actions. 

---

## Requirements

- C++17(20, 24) compiler
- CMake 3.10
- Raylib

---

## How to Compile and Build

1. **Clone the Repository**
   ```bash
   git clone https://github.com/Stone-r1/graph-algorithms-visualizer.git
   cd graph-algorithms-visualizer
   ```

2. **Run Python Script**
   ```bash
   python build.py
   ```

3. **Run**
    - Windows:
    ```bash
    ./graph.exe
    ```
    - Linux:
    ```bash
    ./graph
    ```
---
