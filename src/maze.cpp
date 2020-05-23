#include "maze.h"
#include "edge.h"
#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>
#include "assert.h"
#include "mazebuilder.h"

Maze::Maze(){
    _width = 0;
    _height = 0;
}

Maze::Maze(int grid_width, int grid_height):_width(grid_width),_height(grid_height){
    // Initialize a MAze object to the argument dimensions.
    // Maze is constructed by a MazeBuilder, and start / finish nodes established.
    
    std::cout << "Build Grid" << std::endl;
    buildGrid();

    // Connect Nodes
    for (auto& node: nodes){
        // Get adjacent Node Locations from current Node position.
        std::vector<Point> adjPoints = node->getAdjacentPositions();

        // Test connectivity to the node at each location.
        for (Point px: adjPoints){
            Node * tstNode = getNodeForPoint(px);
            // If the node exists within the maze; check connectivity.
            if (tstNode != nullptr){
                 //std::cout  << "     TEST " << tstNode <<"    ("<< tstNode->p.x << "," << tstNode->p.y<< ")" <<std::endl;
                // If nodes are not connected, make a new EdgeState, then
                //  install edges to nodes.
                if (!(node->isConnectedToNode(tstNode))){
                    connectNodes(node.get(), tstNode);
                } 
            } //else {
            //     connectNodes(node.get(), nullptr); // Ensure a node is added in every position
            //}
        }
    }

    // Build Maze
    MazeBuilder mb(this);
    Node * startNode = nodes.front().get();
    startNode->printEdges();
    mb.buildMaze(startNode);

    // Select Goal
    goal = this->getNodeForPoint(Point(_width-1,_height-1));
}

// Generate a grid of nodes for maze. Nodes are unconnected
void Maze::buildGrid(){
    // Generate Cells
    for (int ix = 0; ix < _width; ix++){
        for (int iy = 0; iy < _height; iy++){
            int idx = ix*_height + iy;
            nodes.emplace_back(std::make_unique<Node>(Node(idx, ix, iy)));
            Node * nodePtr = nodes.back().get();
        }
    }
}

// Initialize an EdgeState to connect nodes
void Maze::connectNodes(Node* nodeA, Node* nodeB){
    // Make a new EdgeState
    if ((nodeA == nullptr)||(nodeB == nullptr)){
        edgestates.emplace_back(std::unique_ptr<EdgeState>());
        assert(edgestates.back() == nullptr);
    } else {
        edgestates.emplace_back(std::make_unique<EdgeState>(nodeA, nodeB));
    }
}

// Use ordered grid for maze to get the nodes corresponding to a quantized offset.
Node * Maze::getNodeForPoint(Point const p){
    // Generate from a 2D grid index based upon column-row order.
    if ((p.x >= _width) || (p.x < 0)){
        return nullptr;
    }
    if ((p.y >= _height) || (p.y < 0)){
        return nullptr;
    }

    // Look up index
    int idx = p.x*_height + p.y ; // 0 indexed
    
    Node * outNode = nullptr;
    if (idx <= nodes.size()){
        outNode = nodes[idx].get();
        //std::cout <<"      Found @ " << outNode <<"("<<outNode->p.x << "," << outNode->p.y << ")" << std::endl;
    }
    
    return  outNode;
}

// Print all nodes in the maze (debugging)
void Maze::printNodes(){
    int ii = 1;
    for (auto & node : nodes){
        std::cout << ii++ << " " << node.get() <<" (" << node->p.x << "," << node->p.y;
        auto state = (node->state == unvisited)? "unvisited" : "visited";
        std::cout <<")->"<< state << std::endl;
        for (auto & edge : node->edges){
            std::cout << "    " << edge.get() << std::endl; 
        }
    }
    std::cout << std::endl;
}

// Print all edges in the maze (debugging)
void Maze::printEdges(){
    int ii = 1;
    std::cout << "-- Edges --" << std::endl;
    for (auto & edge: edgestates){
        std::cout << ii++ << " " << edge.get() << "    (" << edge->edge_1->parent->p.x << "," << edge->edge_1->parent->p.y <<")-(";
        std::cout << edge->edge_1->child->p.x << "," << edge->edge_1->child->p.y <<")" << std::endl;
        auto state = (edge->isOpen()) ? " OPEN" : "CLOSED";
        std::cout <<"     state " << state << std::endl;
        std::cout << "    " << edge->edge_1 << std::endl;
        std::cout << "    " << edge->edge_2 << std::endl;
    }
    std::cout << std::endl;
}

// Maze Traversal, a std::vector of Edges leading to open nodes.
void Maze::getOpenEdges(Node const *node, std::vector<Edge *> *openEdges){
    for (auto edge: node->edges){
        if ((edge != nullptr)&(edge->child->state == unvisited)){
            openEdges->push_back(edge.get());
        }
    }
    
    // Shuffle order of output edges
    std::random_device rd;
    std::mt19937_64 g(rd());
    std::shuffle(openEdges->begin(), openEdges->end(),g);
}

// Add Bot to Maze
void Maze::addBot(){
    this->nodes.front()->bot = std::make_unique<Bot>(Bot());
    bot = this->nodes.front()->bot.get();
    this->nodes.front()->bot->setNode(this->nodes.front().get());
}

// Find boundary nodes.
// Returns a vector of Nodes, representing the unexplored boundary. 
// A node is unexplored if it is adjacennt to non-visible nodes.
std::vector<Node *> Maze::getBoundaryNodes(){
    std::vector<Node *> outlist;
    for (auto & node : nodes){
        if (node->state == NodeStates::visible){
            for (auto & edge: node->edges){
                if ((edge->isOpen()) && !(edge->child->state == NodeStates::visible)){
                    outlist.push_back(edge->child);
                }
            }
         }
    }
    return outlist;
}

// Print all nodes on the boundary.
void Maze::printBoundary(){
    auto bdry = getBoundaryNodes();
    for (auto node : bdry){
        std::cout << node->p.x << " , " << node->p.y << "(d = " << distanceToGoal(node) << ")"<< std::endl;
    }
}

// Search for path between nodes
std::vector<Node *> Maze::findPathToNode(Node * current, const Node * target, const Node * parent){
    std::vector<Node *> path;
    if (current == target){
        // std::cout << "Path Found" << std::endl;
        path.push_back(current);
    } else {
        // For each edge;
        for (auto & edge : current->edges){
            // if edge-> child isn't the originating parent (acyclic graph)
            // get subtree distances.
            if ((edge->isOpen()) &&(edge->child != parent)){
                // edge->print();
                Node * childNode = edge->child;
                auto subpath = findPathToNode(childNode, target, current);
                if (subpath.size() > 0){
                    subpath.push_back(current);
                    return subpath;
                }
            }
        }
    }
    return path;    
}
// Return the manhattan distance to the goal node
int Maze::distanceToGoal(Node * node){
    int px = node->p.x;
    int py = node->p.y;
    int gx = goal->p.x;
    int gy = goal->p.y;
    return abs(px - gx) + abs(py-gy);
}

// subtree or path of nodes, contains a target node.
bool Maze::containsnode(std::vector<Node *> nodes, Node * testnode){
    for (auto node : nodes){
        if (node == testnode){
            return true;
        }
    }
    return false;
}


// Move Semantics
// Move Constructor
Maze::Maze(Maze && maze){
    // Variables
    _width = maze._width;
    _height = maze._height;
    
    // Nodes
    for (auto & node: maze.nodes){
        nodes.emplace_back(std::move(node));
    }
    //EdgeStates
    for (auto & edgestate: maze.edgestates){
        edgestates.emplace_back(std::move(edgestate));
    }

    goal = maze.goal;
    maze.goal = nullptr;

    // Invalidate Source
    maze._width = 0;
    maze._height = 0;
    maze.edgestates.clear();
    maze.nodes.clear();
}

// Copy Operators
// Maze& Maze::operator=(Maze &maze){
// }
 // Move Assignment Opterator
Maze& Maze::operator=(Maze &&maze){
    if (this == &maze)
        return *this;

    nodes.clear();
    edgestates.clear();

    // Variables
    _width = maze._width;
    _height = maze._height;

    // Nodes
    for (auto & node: maze.nodes){
        nodes.emplace_back(std::move(node));
    }
    //EdgeStates
    for (auto & edgestate: maze.edgestates){
        edgestates.emplace_back(std::move(edgestate));
    }

    goal = maze.goal;
    maze.goal = nullptr;

    // Invalidate Source
    maze._width = 0;
    maze._height = 0;
    maze.edgestates.clear();
    maze.nodes.clear();

    return *this;
}

// Destructor
Maze::~Maze(){
    nodes.clear();
    edgestates.clear();
}