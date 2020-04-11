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

bool Node::isConnectedToNode(Node *nodeB){
    for (Edge edge: _edges){
        if ((edge.child == nodeB) || (edge.parent == nodeB)){
            return true;
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





Edge::Edge(Node *P, Node *C){
    child = C;
    parent = P;
}

Maze::Maze(int grid_width, int grid_height){

    // Init area
    _width = grid_width;
    _height = grid_height;

    // Generate Cells
    int id = 0;
    for (int ix = 0; ix < _width; ix++){
        for (int iy = 0; iy < _height; iy++){
            id++;
            int idx = ix*_height + iy;
            nodes.push_back(Node(idx, ix, iy));
            
            // Debugging
            assert(nodes.back().p.x == ix);
            assert(nodes.back().p.y == iy);
            // assert(nodes.back().id == idx);

            // std::cout << "    " << ((Node * )nodes[idx])->id << ": " 
            // <<((Node * )nodes[idx])->p.x 
            // << ", " << ((Node * )nodes[idx])->p.y 
            // << std::endl;
            std::cout << nodes[idx].id << "," << nodes[idx].p.x << "," << nodes[idx].p.y << std::endl;
        }
    }
    // std::cout << " Maze Constructor with " << nodes.size() << " nodes" << std::endl;

    // Testing
    // for (int iy = 0; iy < _height; iy++){
    //     for (int ix = 0; ix < _width; ix++){
    //         Point tstPt{ix, iy};
    //         Node * node = getNodeForPoint(tstPt);
    //         std::cout << "    " << ix << "," << iy <<" ---> " << node->id << ": " 
    //         << node->p.x << ", " << node->p.y 
    //         << std::endl;
    //     }
    // }
    std::cout << " Connecting Nodes" << std::endl;

    // Connect Nodes

    for (auto node: nodes){
        std::cout << "Connecting " << node.p.x << ", " << node.p.y << std::endl;

        std::vector<Point> adjPoints = node.getAdjacentPositions();
        for (Point px: adjPoints){
            Node * tstNode = getNodeForPoint(px);
            std::cout << "     " << px.x << ", " << px.y ;

            if (tstNode != nullptr){
                std::cout  << "    ("<< tstNode->p.x << "," << tstNode->p.y<< ")";
                if (!node.isConnectedToNode(tstNode)){
                    Edge e = Edge(&node, tstNode);
                    edges.push_back();
                } else
                {
                    std::cout  << "[Connected]";
                }
            }
            std::cout  <<std::endl;
        }
    }
    std::cout << " Nodes Connected" << std::endl;
}

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
    //std::cout <<"   " << idx <<"("<<p.x << "," << p.y << ")" << std::endl;
    return (idx <= nodes.size()) ? &nodes[idx]: nullptr;
}

// Initialize an edge to connect nodes
void  Maze::connectNodes(Node * nodeA, Node * nodeB){


}