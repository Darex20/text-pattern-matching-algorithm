#include "FileReader.h"
#include "NodeUtils.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

using namespace std;

// Read graph from .gfa file
Graph FileReader::readGraph(const string& filePath) {
    try {
        ifstream file(filePath);
        vector<string> lines;
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        Graph graph;
        int overlap = 0;

        for (const string& line : lines) {
            if (line[0] == 'L') {
                string overlap_label = line.substr(line.find_last_of('\t') + 1);
                overlap_label = overlap_label.substr(0, overlap_label.length() - 1);
                int overlap_value = overlap_label.empty() ? 0 : stoi(overlap_label);
                // assert(graph.overlap == 0 || graph.overlap == overlap_value);
                graph.overlap = overlap_value;
            }
        }

        for (const string& line : lines) {
            if (line[0] == 'S') {
                Node first_node = NodeUtils::createNode(line, graph.overlap);
                Node reverse_complement_node = NodeUtils::createReverseComplementNode(line, graph.overlap);
                graph.addNode(first_node);
                graph.addNode(reverse_complement_node);
            }
        }

        for (const string& line : lines) {
            if (line[0] == 'L') {
                graph.createAndAddEdges(line);
            }
        }

        return graph;

    } catch (const exception& e) {
        cerr << e.what() << endl;
        return Graph();  // Return an empty graph on error
    }
}

// Read fastq sequence from file
// returns vector<string> of fastq sequences
vector<string> FileReader::readFastq(const string& filePath) {
    vector<string> fastqs;
    try {
        ifstream file(filePath);
        vector<string> lines;
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        int i = 0;
        while (i < lines.size()) {
            string line = lines[i];
            if (line[0] != '@') {
                i++;
                continue;
            }
            i++;
            line = lines[i];
            string sequence = line;
            i += 2;
            fastqs.push_back(sequence);
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return vector<string>();
    }
    return fastqs;
}

// Read bit-parallel scores, save it in vector
// returns vector of scores
vector<int> FileReader::loadScores(const string& filePath) {
    vector<int> scores;
    try {
        ifstream file(filePath);
        vector<string> lines;
        string line;
        while (getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        for (const string& line : lines) {
            int score = stoi(line);
            scores.push_back(score);
        }
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return vector<int>();  // Return an empty vector on error
    }
return scores;
}

// Read bit-parallel times (ms) 
// returns bit-parallel times (ms) 
int FileReader::loadTime(const string& filePath) {
    try {
        ifstream file(filePath);
        string line;
        getline(file, line);
        file.close();

        return stoi(line);
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 0;  // Return 0 on error
    }
}

// Sum bit-parallel times
double FileReader::add_bit_parallel_times(const vector<string>& graphNames) {
    double sum = 0.0;
    for (const string& graphName : graphNames) {
        sum += loadTime("../bit_parallel_times/" + graphName.substr(0, graphName.length() - 3) + "txt");
    }
    return sum / 1000000;
}

// Reads memory usage
int FileReader::loadMemory(const string& filePath) {
    try {
        ifstream file(filePath);
        string line;
        getline(file, line);
        file.close();

        return stoi(line);
    } catch (const exception& e) {
        cerr << e.what() << endl;
        return 0;  // Return 0 on error
    }
}
