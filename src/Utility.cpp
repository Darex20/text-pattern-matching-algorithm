#include "Utility.h"
#include "Aligner.h"

#include <iostream>
#include <fstream>
#include <assert.h>
#include <sstream>


Graph Utility::readGraph(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        Graph graph;
        int overlap = 0;

        for (const std::string& line : lines) {
            if (line[0] == 'L') {
                std::string overlap_oznaka = line.substr(line.find_last_of('\t') + 1);
                overlap_oznaka = overlap_oznaka.substr(0, overlap_oznaka.length() - 1);
                int overlap_value = overlap_oznaka.empty() ? 0 : std::stoi(overlap_oznaka);
                assert(graph.overlap == 0 || graph.overlap == overlap_value);
                graph.overlap = overlap_value;
            }
        }

        for (const std::string& line : lines) {
            if (line[0] == 'S') {
                Node first_node = createNode(line, graph.overlap);
                Node reverse_complement_node = createReverseComplementNode(line, graph.overlap);
                graph.addNode(first_node);
                graph.addNode(reverse_complement_node);
            }
        }

        for (const std::string& line : lines) {
            if (line[0] == 'L') {
                graph.createAndAddEdges(line);
            }
        }

        return graph;
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return Graph();  // Return an empty graph on error
    }
}

std::vector<std::string> Utility::readFastq(const std::string& filePath) {
    std::vector<std::string> fastqs;
    try {
        std::ifstream file(filePath);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        int i = 0;
        while (i < lines.size()) {
            std::string line = lines[i];
            if (line[0] != '@') {
                i++;
                continue;
            }
            i++;
            line = lines[i];
            std::string sequence = line;
            i += 2;
            fastqs.push_back(sequence);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return std::vector<std::string>();  // Return an empty vector on error
    }
    return fastqs;
}

std::vector<int> Utility::loadScores(const std::string& filePath) {
    std::vector<int> scores;
    try {
        std::ifstream file(filePath);
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();

        for (const std::string& line : lines) {
            int score = std::stoi(line);
            scores.push_back(score);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return std::vector<int>();  // Return an empty vector on error
    }
return scores;
}

Node Utility::createNode(const std::string& line, int overlap) {
std::vector<std::string> arr;
std::string token;
std::stringstream ss(line);
while (std::getline(ss, token, '\t')) {
    arr.push_back(token);
}

int id = std::stoi(arr[1]);
std::string sequence = arr[2];
return Node(id * 2, false, sequence.substr(0, sequence.length() - overlap));
}

Node Utility::createReverseComplementNode(const std::string& line, int overlap) {
    std::vector<std::string> arr;
    std::string token;
    std::stringstream ss(line);
    while (std::getline(ss, token, '\t')) {
        arr.push_back(token);
    }

    int id = std::stoi(arr[1]);
    std::string sequence = arr[2];
    return Node(id * 2 + 1, true, getReverseComplement(sequence).substr(0, sequence.length() - overlap));
}

std::string Utility::getReverseComplement(const std::string& sequence) {
    std::string reverse_complement;
    for (int i = sequence.length() - 1; i >= 0; i--) {
        char c = sequence[i];
        switch (c) {
            case 'A':
                reverse_complement += 'T';
                break;
            case 'C':
                reverse_complement += 'G';
                break;
            case 'G':
                reverse_complement += 'C';
                break;
            case 'T':
                reverse_complement += 'A';
                break;
            default:
                throw std::runtime_error("Unexpected value: " + std::to_string(c));
        }
    }
    return reverse_complement;
}

int Utility::loadTime(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        std::string line;
        std::getline(file, line);
        file.close();

        return std::stoi(line);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;  // Return 0 on error
    }
}

double Utility::add_bit_parallel_times(const std::vector<std::string>& graphNames) {
    double sum = 0.0;
    for (const std::string& graphName : graphNames) {
        sum += loadTime("../bit_parallel_times/" + graphName.substr(0, graphName.length() - 3) + "txt");
    }
    return sum / 1000000;
}

int Utility::loadMemory(const std::string& filePath) {
    try {
        std::ifstream file(filePath);
        std::string line;
        std::getline(file, line);
        file.close();

        return std::stoi(line);
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 0;  // Return 0 on error
    }
}

std::vector<std::vector<int>> Utility::copyNeighbors(const std::vector<std::vector<int>>& neighbors) {
    std::vector<std::vector<int>> newList;
    for (const std::vector<int>& nodeNeighbors : neighbors) {
        newList.push_back(nodeNeighbors);
    }
    return newList;
}
