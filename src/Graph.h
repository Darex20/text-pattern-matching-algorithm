#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class Node {
private:
    int id;
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
    int overlap;
    vector<char> graphSequence;
    vector<vector<int>> inNeighbors;
    unordered_map<int, int> nodesMap;
    vector<int> nodeIndexInGraphSequence;
    vector<vector<int>> outNeighbors;

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
