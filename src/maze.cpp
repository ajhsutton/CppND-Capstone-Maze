#include "maze.h"
#include <cmath>
#include <iostream>
#include "assert.h"

// Static Variable
int Node::_idCnt = 0;

// Constructors
Node::Node(){
    id = _idCnt;
    _idCnt ++;
    p.x = 0;
    p.y = 0;
}

Node::Node(int id_, int px, int py){
    id = _idCnt;
    _idCnt ++;
    p.x = px;
    p.y = py;
}

void Node::printEdges(){
    std::cout << "    Print Node Edges: "<< this << " (" << p.x << "," << p.y << ")"<< std::endl;
    if (edges.size() > 0){
        for (auto edge: this->edges){
            std::cout << "        Edge:" << edge->child << "   " << edge->child->p.x << "," << edge->child->p.y << std::endl;
        }
    } else std::cout << "        Empty" << std::endl<< std::endl;
}

bool Node::isConnectedToNode(Node const *nodeB){
    if (edges.size() > 0){
        int ii = 1;
        for (auto edge: edges){
            // debugging
            // assert(edge->parent == this);
            // std::cout << "        Edge " << ii++ << std::endl;
            // std::cout << "            Parent:" << edge->parent<<" " << edge->parent->p.x << "," << edge->parent->p.y << std::endl;
            // std::cout << "             Child:" << edge->child <<" " <<edge->child->p.x << "," << edge->child->p.y << std::endl;
            if (edge->child == nodeB){
                // std::cout << "                        Connection Found "<< nodeB->p.x << "," << nodeB->p.y << std::endl;
                return true;
            }
        }
    }
    return false;
}

std::vector<Point> Node::getAdjacentPositions(){
    std::vector<Point> out;
    for (int pos =0; pos < 4; pos++){
        switch (pos)
        {
        case 0:
            out.emplace_back(Point{p.x,p.y+1});
            break;
        case 1:
            out.emplace_back(Point{p.x+1,p.y});
            break;
        case 2:
            out.emplace_back(Point{p.x,p.y-1});
            break;
        case 3:
            out.emplace_back(Point{p.x-1,p.y});
            break;
        default:
            break;
        }
    }
    return out;
}

Edge::Edge(EdgeState *state_, Node * child_, Node * parent_){
    state = state_;
    child = child_;
    parent = parent_;
}

EdgeState::EdgeState(Node* node1, Node* node2)
{
    // std::cout << "EdgeState for nodes at:" << node1 <<" and " << node2 << std::endl;
    // Initialize state
    val = 0;
    
    // Initialize Edges
    edge_1 = std::make_shared<Edge>(Edge(this, node1, node2));
    edge_1->parent->edges.emplace_back(edge_1);
    // std::cout << "   Edge 1:" << edge_1->parent << "->" << edge_1->child << std::endl;

    edge_2 = std::make_shared<Edge>(Edge(this, node2, node1));
    edge_2->parent->edges.emplace_back(edge_2);
    // std::cout << "   Edge 2:" << edge_2->parent << "->" << edge_2->child << std::endl;
}

Maze::Maze(){
    _width = 0;
    _height = 0;
}

Maze::Maze(int grid_width, int grid_height){

    // Init area variables and bild grid.
    _width = grid_width;
    _height = grid_height;
    buildGrid();

    // Connect Nodes
    std::cout << "--Connecting Nodes--" << std::endl<< std::endl;

    for (auto& node: nodes){
        std::cout << "Node " << node.get() <<" ("<< node->p.x << ", " << node->p.y <<")"<< std::endl;
        // Get adjacent Node Locations from current Node position.
        std::vector<Point> adjPoints = node->getAdjacentPositions();

        // Test connectivity to the node at each location.
        for (Point px: adjPoints){
            Node * tstNode = getNodeForPoint(px);
            // If the node exists within the maze; check connectivity.
            if (tstNode != nullptr){
                std::cout  << "     TEST " << tstNode <<"    ("<< tstNode->p.x << "," << tstNode->p.y<< ")" <<std::endl;
                // If nodes are not connected, make a new EdgeState, then
                //  install edges to nodes.
                if (!(node->isConnectedToNode(tstNode))){
                    std::cout << "--Connecting Node--" << std::endl;
                    connectNodes(node.get(), tstNode);
                } 
                else { std::cout  << "         [Connected]" << std::endl;}
            }
        }
         std::cout  << "\nEdge Count "<< this->edgestates.size() << std::endl<< std::endl;
    }
    std::cout << "\n\n Nodes Connected" << std::endl;
}
// Generate a grid of nodes for maze. Nodes are unconnected
void Maze::buildGrid(){
    // Generate Cells
    std::cout << "Maze nodes:" << &nodes << std::endl;
    for (int ix = 0; ix < _width; ix++){
        for (int iy = 0; iy < _height; iy++){
            int idx = ix*_height + iy;

            nodes.emplace_back(std::make_unique<Node>(Node(idx, ix, iy)));
            
            Node * nodePtr = nodes.back().get();
            std::cout << nodePtr->p.x << "," <<nodePtr->p.y << " @ " <<nodePtr << std::endl;
            
            // Debugging
            // assert(nodes.back().p.x == ix);
            // assert(nodes.back().p.y == iy);
            // assert(nodes.back().id == idx);

            // std::cout << "    " << ((Node * )nodes[idx])->id << ": " 
            // <<((Node * )nodes[idx])->p.x 
            // << ", " << ((Node * )nodes[idx])->p.y 
            // << std::endl;
            // std::cout << nodes[idx].id << "," << nodes[idx].p.x << "," << nodes[idx].p.y << std::endl;
        }
    }
    std::cout << " Maze Constructor with " << nodes.size() << " nodes" << std::endl;
}

// Initialize an EdgeState to connect nodes
void Maze::connectNodes(Node* nodeA, Node* nodeB){
    // Make a new EdgeState
    edgestates.emplace_back(std::make_unique<EdgeState>(EdgeState(nodeA, nodeB)));
    // assert(edgestates.back()->edges[0].parent == nodeA);
    // assert(edgestates.back()->edges[0].child  == nodeB);
    // assert(edgestates.back()->edges[1].parent == nodeB);
    // assert(edgestates.back()->edges[1].child  == nodeA);
    // std::cout << "Edge Count " << this->edgestates.size() << std::endl;
    // std::cout << "Node A Edges:" << std::endl;
    // nodeA->printEdges();
    // std::cout << "Node B Edges:" << std::endl;
    // nodeB->printEdges();
    }


// Use ordered grid for maze to get the nodes corresponding to a quantized offset.
/* Test Code:
for (int iy = 0; iy < _height; iy++){
    for (int ix = 0; ix < _width; ix++){
        Point tstPt{ix, iy};
        Node * node = getNodeForPoint(tstPt);
        std::cout << "    " << ix << "," << iy <<" ---> " << node->id << ": " 
        << node->p.x << ", " << node->p.y 
        << std::endl;
    }
} */
Node * Maze::getNodeForPoint(Point const p){
    // Generate from a 2D grid index based upon column-row order.
    if ((p.x >= _width) ||(p.x < 0)){
        return nullptr;
    }
    
    if ((p.y >= _height) ||(p.y < 0)){
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