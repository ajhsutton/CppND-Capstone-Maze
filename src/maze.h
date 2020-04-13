#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <array>
#include <memory>

// Structure defining a 2D point for a spatial data graph
struct Point
{
    int x;
    int y;
};

class EdgeState;
class Node;

// Directed Graph Edge; edges always exit parent node.
// Edge state is synchronized through the common edge state object.
struct Edge{
    EdgeState * state;
    Node * child;
    Node * parent;
    Edge(EdgeState *state, Node * child, Node * parent);
};

// EdgeState maintains the state of an Edge in a Graph
class EdgeState
{
    public:
        int val;
        std::shared_ptr<Edge>  edge_1;
        std::shared_ptr<Edge>  edge_2;
        EdgeState(Node* node1, Node* node2);
};

// Node class for graph
// TODO: Template class for use with generic data object @ node
class Node {
    public:
        int id;
        Point p; 
        Node();
        Node(int id_, int px, int py);

        // TODO: Object Pointer
        bool isConnectedToNode(Node const *nodeB);
        void printEdges();
        std::vector<Point> getAdjacentPositions();
        std::vector<std::shared_ptr<Edge>> edges;
    private:
        static int _idCnt;
};

class Maze {
 public:
  Maze();
  Maze(int grid_width, int grid_height); // Constructor
  Maze(const Maze & maze) = delete; // Copy Constructor
  Maze(Maze && maze); // Move Constructor
  Maze &operator=(Maze &maze) = delete; // Copy Operator
  Maze &operator=(Maze &&maze); // Move Assignment Opterator
  ~Maze();

  std::vector<std::unique_ptr<Node>> nodes;
  std::vector<std::unique_ptr<EdgeState>> edgestates;

 private:
  int _width;
  int _height;
  
  Node * getNodeForPoint(Point const p);
  void buildGrid();
  void connectNodes(Node* nodeA, Node* nodeB);
};

#endif