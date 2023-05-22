#ifndef NODEUTILS_H
#define NODEUTILS_H

#include "Graph.h"
#include <string>

using namespace std;

class NodeUtils {
public:
    static Node createNode(const string& line, int overlap);
    static Node createReverseComplementNode(const string& line, int overlap);
    static string getReverseComplement(const string& sequence);
    static vector<vector<int>> copyNeighbors(const vector<vector<int>>& neighbors);
};

#endif