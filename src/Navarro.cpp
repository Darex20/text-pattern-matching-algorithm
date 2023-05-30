#include "Graph.h"
#include "Navarro.h"

#include <iostream>
#include <string>
#include <sstream>

Navarro::Navarro(Graph& graph, const vector<int>& topologicalOrder) {
	this -> graph = graph;
	this -> topologicalOrder = topologicalOrder; //list with node indexes but in topological order
}

// Align acyclic graph with pattern
// returns the smallest edit distance
int Navarro::align(string pattern) {
	
    vector<int> realEditDistances(graph.graphSequence.size(), 0); // Cv
    vector<int> iterationEditDistances(graph.graphSequence.size(), 0); // C'v -> min(Cv, 1+Cu)
    
	// initialize lists of edit distances
    for (int j = 0; j < realEditDistances.size(); j++) {
        char char_in_graph = graph.graphSequence[j];
        realEditDistances[j] = (pattern[0] == char_in_graph) ? 0 : 1;
        iterationEditDistances[j] = (pattern[0] == char_in_graph) ? 0 : 1;
    }

    for (int j = 1; j < pattern.length(); j++) {
        //iterate through nodes
        for (int node : topologicalOrder) {
            this->g(realEditDistances, iterationEditDistances, pattern, j, node);
        }

        // Cv <- C'v
    	vector<int> tmp = realEditDistances;
        realEditDistances = iterationEditDistances;
        iterationEditDistances = tmp;
    }
    //let smallest be maximum int value
    int smallest = INT_MAX;
    for (int realEditDistance : realEditDistances) {
        smallest = std::min(smallest, realEditDistance);
    }
    return smallest; //smallest edit distance
}

// Align cyclic graph with pattern
// returns the smallest edit distance
int Navarro::alignCycle(std::string pattern) {
    vector<int> realEditDistances(graph.graphSequence.size(), 0);
    vector<int> iterationEditDistances(graph.graphSequence.size(), 0);

    for (int j = 0; j < realEditDistances.size(); j++) {
        char char_in_graph = graph.graphSequence[j];
        realEditDistances[j] = (pattern[0] == char_in_graph) ? 0 : 1;
        iterationEditDistances[j] = (pattern[0] == char_in_graph) ? 0 : 1;
    }

    for (int j = 1; j < pattern.length(); j++) {
        for (int i = 0; i < iterationEditDistances.size(); i++)
            iterationEditDistances[i] = pattern.length();

        // iterate through every node
        for (int node : topologicalOrder) {
            this->g(realEditDistances, iterationEditDistances, pattern, j, node);
        }

        // Cv <- C'v
        vector<int> tmp = realEditDistances;
        realEditDistances = iterationEditDistances;
        iterationEditDistances = tmp;

        for (int v = 0; v < graph.getNumberOfNodesInGraph(); v++) {
            for (int u : graph.inNeighbors[v]) {
                propagate(u, v, realEditDistances);
            }
        }
    }

    int smallest = INT_MAX;
    for (int realEditDistance : realEditDistances) {
        smallest = min(smallest, realEditDistance);
    }
    return smallest;
}

// Navarro algorithm, g function
void Navarro::g(vector<int>& realEditDistances, vector<int>& iterationEditDistances, const string& pattern, int j, int node) {
    int start = graph.getNodeStartInSequence(node); //start
    int end = graph.getNodeEndInSequence(node); //end

    // instead of j-1, we take last iteration + 1
    iterationEditDistances[start] = realEditDistances[start] + 1;

    char charInGraph = graph.graphSequence[start];
    bool match = (charInGraph == pattern[j]);

    // set smallest value on maximum int value
    int smallestNeighbor = INT_MAX; 
    for (int neighbor : graph.inNeighbors[node]) {
        int lastInNeighborIndex = graph.getNodeEndInSequence(neighbor) - 1;
        smallestNeighbor = min(smallestNeighbor, realEditDistances[lastInNeighborIndex]);
    }

    // set smallest value on maximum int value
    int smallestIterationNeighbor = INT_MAX;

    // iterate through neighbors
    for (int neighbor : graph.inNeighbors[node]) {
        int lastInNeighborIndex = graph.getNodeEndInSequence(neighbor) - 1;
        smallestIterationNeighbor = std::min(smallestIterationNeighbor, iterationEditDistances[lastInNeighborIndex]);
    }

    if (match) {
        iterationEditDistances[start] = min(iterationEditDistances[start], smallestNeighbor);
    } else {
        iterationEditDistances[start] = 1 + min(realEditDistances[start], min(smallestNeighbor, smallestIterationNeighbor));
    }

    for (int c = start + 1; c < end; c++) {
        charInGraph = graph.graphSequence[c];
        match = (charInGraph == pattern[j]);
        if (match) {
            iterationEditDistances[c] = realEditDistances[c - 1];
        } else {
            iterationEditDistances[c] = 1 + min(realEditDistances[c], std::min(realEditDistances[c - 1], iterationEditDistances[c - 1]));
        }
    }
}

// Navarro algorithm, propagation function
void Navarro::propagate(int u, int v, vector<int>& realEditDistances) {
    int u_end = graph.getNodeEndInSequence(u) - 1;
    int v_start = graph.nodeIndexInGraphSequence[v];
    int v_end = graph.getNodeEndInSequence(v) - 1;
    int new_value = realEditDistances[u_end] + 1;
    if (realEditDistances[v_start] > new_value) {
        realEditDistances[v_start] = new_value;

        // Propagate further inside node
        new_value++;
        for (int c = v_start + 1; c < v_end; c++) {
            if (realEditDistances[c] > new_value) {
                realEditDistances[c] = new_value;
                new_value++;
            } else {
                break;
            }
        }
        for (int z : graph.outNeighbors[v]) {
            propagate(v, z, realEditDistances);
        }
    }
}




