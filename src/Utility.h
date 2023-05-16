#ifndef UTILITY_H
#define UTILITY_H

#include "Graph.h"
#include <string>

using namespace std;

class Utility {
public:
    static Graph readGraph(const string& filePath);
    static vector<string> readFastq(const string& filePath);
    static vector<int> loadScores(const string& filePath);
    static Node createNode(const std::string& line, int overlap);
    static Node createReverseComplementNode(const std::string& line, int overlap);
    static std::string getReverseComplement(const std::string& sequence);
    static int loadTime(const std::string& filePath);
    static double add_bit_parallel_times(const std::vector<std::string>& graphNames);
    static int loadMemory(const std::string& filePath);
    static std::vector<std::vector<int>> copyNeighbors(const std::vector<std::vector<int>>& neighbors);
};

#endif