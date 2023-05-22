#include "Graph.h"
#include "Utility.h"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

//one node in graph
Node::Node(int id, bool reversed, string sequence){
    this->id = id;
    this->reversed = reversed;
    this->sequence = sequence;
}

//getters
int Node::getNodeId(void) {
    return id;
}

bool Node::getReversed() {
    return reversed;
}

string Node::getGraphSequence() {
    return sequence;
}

//graph which contains nodes and edges
Graph::Graph() {
}

//getters
int Graph::getNodeStartInSequence(int node) {
    return nodeIndexInGraphSequence[node]; //returns index of starting node in graph sequence
}

int Graph::getNodeEndInSequence(int node) {
    if (node == nodeIndexInGraphSequence.size() - 1)
        return graphSequence.size();
    return nodeIndexInGraphSequence[node + 1]; //returns index od end node + 1 in graph sequence
}

int Graph::getNumberOfNodesInGraph(void){
    return nodeIndexInGraphSequence.size(); //returns total number of nodes in graph
}

//adds node in graph, adds sequence of node in 'graphSequence', adds starting node in list
void Graph::addNode(Node node) {
    nodesMap[node.getNodeId()] = getNumberOfNodesInGraph();
    nodeIndexInGraphSequence.push_back(graphSequence.size());
    inNeighbors.push_back(std::vector<int>());
    outNeighbors.push_back(std::vector<int>());
    for (char c : node.getGraphSequence()) {
        graphSequence.push_back(c);
    }
}

//Util sort function for topologicalOrderOfNodesCyclic
void Graph::topologicalSortUtil(int v, vector<int>& stack, vector<bool>& visited) {
    visited[v] = true;
    vector<int>& neighbors = outNeighbors[v];
    for (int i = 0; i < neighbors.size(); i++) {
        if (!visited[neighbors[i]])
            topologicalSortUtil(neighbors[i], stack, visited);
    }
    stack.push_back(v);
}

//DFS algorithm 
//returns topological orfer of nodes in graph
vector<int> Graph::topologicalOrderOfNodesCyclic() {
    vector<int> stack;
    vector<bool> visited(getNumberOfNodesInGraph(), false); //all nodes are not visited

    for (int i = 0; i < getNumberOfNodesInGraph(); i++) {
        if (!visited[i])
            topologicalSortUtil(i, stack, visited);
    }

    reverse(stack.begin(), stack.end());
    return stack;
}

//Kahn's sorting algorithm
//https://www.geeksforgeeks.org/topological-sorting-indegree-based-solution/
vector<int> Graph::topologicalOrderOfNodesAcyclic() {
    vector<int> L;
    vector<int> S;
    vector<vector<int>> inNeighborsCopy = Utility::copyNeighbors(inNeighbors);
    vector<vector<int>> outNeighborsCopy = Utility::copyNeighbors(outNeighbors);

    for (int i = 0; i < inNeighbors.size(); i++) {
        if (inNeighbors[i].size() == 0)
            S.push_back(i);
    }

    while (!S.empty()) {
        int n = S[0];
        S.erase(S.begin());
        L.push_back(n);
        for (int m : outNeighbors[n]) {
            outNeighborsCopy[n].erase(remove(outNeighborsCopy[n].begin(), outNeighborsCopy[n].end(), m), outNeighborsCopy[n].end());
            inNeighborsCopy[m].erase(remove(inNeighborsCopy[m].begin(), inNeighborsCopy[m].end(), n), inNeighborsCopy[m].end());
            if (inNeighborsCopy[m].empty())
                S.push_back(m);
        }
    }

    for (const auto& neighbors : inNeighborsCopy) {
        if (!neighbors.empty()) {
            throw runtime_error("Graph is not acylic.");
        }
    }

    return L;
}

void Graph::createAndAddEdges(string line) {
    vector<string> arr;
    stringstream ss(line);
    string token;

    while (getline(ss, token, '\t')) {
        arr.push_back(token);
    }

    int from_node_original_id = stoi(arr[1]);
    string from_sign = arr[2];
    int to_node_original_id = stoi(arr[3]);
    string to_sign = arr[4];
    int from_normal, from_reverse, to_normal, to_reverse;

    if (from_sign == "+") {
        from_normal = from_node_original_id * 2;
        from_reverse = from_node_original_id * 2 + 1;
    } else {
        from_normal = from_node_original_id * 2 + 1;
        from_reverse = from_node_original_id * 2;
    }
    if (to_sign == "+") {
        to_normal = to_node_original_id * 2;
        to_reverse = to_node_original_id * 2 + 1;
    } else {
        to_normal = to_node_original_id * 2 + 1;
        to_reverse = to_node_original_id * 2;
    }

    int from_node = nodesMap[from_normal];
    int to_node = nodesMap[to_normal];

    if (find(this->inNeighbors[to_node].begin(), this->inNeighbors[to_node].end(), from_node) == this->inNeighbors[to_node].end()) {
        this->inNeighbors[to_node].push_back(from_node);
    }
    if (find(this->outNeighbors[from_node].begin(), this->outNeighbors[from_node].end(), to_node) == this->outNeighbors[from_node].end()) {
        this->outNeighbors[from_node].push_back(to_node);
    }

    from_node = nodesMap[to_reverse];
    to_node = nodesMap[from_reverse];

    if (find(this->inNeighbors[to_node].begin(), this->inNeighbors[to_node].end(), from_node) == this->inNeighbors[to_node].end()) {
        this->inNeighbors[to_node].push_back(from_node);
    }
    if (find(this->outNeighbors[from_node].begin(), this->outNeighbors[from_node].end(), to_node) == this->outNeighbors[from_node].end()) {
        this->outNeighbors[from_node].push_back(to_node);
    }
}
