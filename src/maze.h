#ifndef MAZE_H
#define MAZE_H

#include <vector>

// Structure defining a 2D point
struct Point
{
    int x;
    int y;
};

class Edge;

class Node {
    public:
        int id;
        Point p; 
        Node();
        Node(int id_, int px, int py);
        // TODO: Object Pointer
        bool isConnectedToNode(Node *nodeB);
        std::vector<Point> getAdjacentPositions();
    private:
        std::vector<Edge> _edges;
        static int _idCnt;
};

class Edge{
    public:
        int state;
        Node * child;
        Node * parent;
        Edge(Node *P, Node *C);
};


class Maze {
 public:
  Maze(int grid_width, int grid_height);
  
  void Update();
  std::vector<Node> nodes;
  
 private:
  int _width;
  int _height;
  std::vector<Edge> edges;

  Node * getNodeForPoint(Point const p);
  void connectNodes(Node * nodeA, Node * nodeB);
};

#endif