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

Maze::Maze(int grid_width, int grid_height){

    // Init area variables and bild grid.
    _width = grid_width;
    _height = grid_height;
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
    //std::cout << "Build Maze [Start]" << std::endl;
    // Build Maze
    MazeBuilder mb(this);
    Node * startNode = nodes.front().get();
    startNode->printEdges();
    mb.buildMaze(startNode);
    //std::cout << "Build Maze [Complete]" << std::endl;
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

// Maze Traversal
// return a std::vector of Edges leading to open nodes.
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

    // Invalidate Source
    maze._width = 0;
    maze._height = 0;
    maze.edgestates.clear();
    maze.nodes.clear();
}

// Copy Operator
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