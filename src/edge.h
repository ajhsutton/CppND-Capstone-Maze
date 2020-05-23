#ifndef EDGE_H
#define EDGE_H

#include <vector>
#include <memory>
#include "bot.h"

// Structure defining a 2D point for a spatial data graph
struct Point
{
    int x;
    int y;
    Point():x(0),y(0){};
    Point(int x_, int y_): x(x_),y(y_){};
};

class EdgeState;
class Node;

// Directed Graph Edge; edges always exit parent node.
// Edge state is synchronized through the common edge state object.
class Edge{
    public:
    EdgeState * state;
    Node * child;
    Node * parent;

    Edge(EdgeState *state, Node * child, Node * parent);
    Edge(Edge & edge);
    Edge(Edge && edge);
    Edge& operator=(Edge & edge);
    Edge& operator=(Edge && edge);

    Bot::Direction getDirection();

    void openEdge();
    bool isOpen();
    void print();
};

// EdgeState maintains the state of an Edge in a Graph
class EdgeState
{
    public:
        std::unique_ptr<bool> open;
        std::shared_ptr<Edge>  edge_1;
        std::shared_ptr<Edge>  edge_2;
        EdgeState(Node* node1, Node* node2);
        EdgeState(EdgeState & edgestate) = delete;
        EdgeState& operator=(EdgeState & edge) = delete;

        bool isOpen(){return *open;};
        void openEdgeState(){*open = true;};
};

// Node class for graph
enum NodeStates {unvisited, visited, visible};

// class Bot;

// TODO: Template class for use with generic data object @ node
class Node {
    public:
        int id;
        Point p; 
        NodeStates state;
        std::vector<std::shared_ptr<Edge>> edges;
        std::unique_ptr<Bot> bot;

        Node();
        Node(int id_, int px, int py);

        bool isConnectedToNode(Node const *nodeB);
        bool isValidMove(Node const *nodeB);
        void printEdges();
        Point getLocation(){return Point(p.x, p.y);};
        std::vector<Edge *> getValidEdges();
        std::vector<Point> getAdjacentPositions();
        void moveBot(Node *newNode);
        bool hasBot() {return bot != nullptr;};
    private:
        static int _idCnt;
};
#endif