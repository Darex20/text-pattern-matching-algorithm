#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class Node {
private:
    int id; //id of node
    bool reversed;
    string sequence;

public:
    Node(int id, bool reversed, string sequence);
    int getNodeId();
    bool getReversed();
    string getGraphSequence();

};

class Graph {
private:
    
public:
    int overlap; //overlap from .gfa file. we use it when we create nodes. 
    vector<char> graphSequence; //vector of chars which contains all nodes in graph
    vector<vector<int>> inNeighbors; //for node x stores list of nodes that goes to node x
    unordered_map<int, int> nodesMap; //used for normalizing nodes
    vector<int> nodeIndexInGraphSequence; //vector which contains indexes of every start node 
    vector<vector<int>> outNeighbors; //for node x stores list of nodes that goes from node x

    Graph();
    int getNodeStartInSequence(int node);
    int getNodeEndInSequence(int node);
    int getNumberOfNodesInGraph(void);
    void addNode(Node node);
    void createAndAddEdges(string line);
    vector<int> topologicalOrderOfNodesAcyclic(void);
    vector<int> topologicalOrderOfNodesCyclic();
    void topologicalSortUtil(int v, vector<int>& stack, vector<bool>& visited);

};

#endif
