#ifndef FILEREADER_H
#define FILEREADER_H

#include "Graph.h"
#include <string>

using namespace std;

class FileReader {
public:
    static Graph readGraph(const string& filePath);
    static vector<string> readFastq(const string& filePath);
    static vector<int> loadScores(const string& filePath);
    static int loadTime(const string& filePath);
    static double add_bit_parallel_times(const vector<string>& graphNames);
    static int loadMemory(const string& filePath);
};

#endif