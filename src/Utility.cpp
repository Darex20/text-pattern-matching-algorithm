#include "Utility.h"
#include "Aligner.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

//read graph from .gfa file
Graph Utility::readGraph(const std::string& filePath) {
    //try-catch block, try reading file
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
                std::string overlap_label = line.substr(line.find_last_of('\t') + 1);
                overlap_label = overlap_label.substr(0, overlap_label.length() - 1);
                int overlap_value = overlap_label.empty() ? 0 : std::stoi(overlap_label);
                // assert(graph.overlap == 0 || graph.overlap == overlap_value);
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

        //catch exception
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return Graph();  // Return an empty graph on error
    }
}

//read fastq sequence from file
//return vector<string> of fastq sequences
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

//read bit-parallel scores, save it in vector
//returns vector of scores
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

//creates node with overlap
//returns new Node
Node Utility::createNode(const std::string& line, int overlap) {
    std::vector<std::string> arr;
    std::string token;
    std::stringstream ss(line);
    while (std::getline(ss, token, '\t')) {
        arr.push_back(token);
    }

    int id = std::stoi(arr[1]);
    std::string sequence = arr[2];
    //for every node create 2 new nodes
    //first node is normal (id*2)
    return Node(id * 2, false, sequence.substr(0, sequence.length() - overlap));
}

//creates reverse complement node with overlap
//returns new Node
Node Utility::createReverseComplementNode(const std::string& line, int overlap) {
    std::vector<std::string> arr;
    std::string token;
    std::stringstream ss(line);
    while (std::getline(ss, token, '\t')) {
        arr.push_back(token);
    }

    int id = std::stoi(arr[1]);
    std::string sequence = arr[2];
    //for every node create 2 new nodes
    //second node is reverse complement (id*2+1)
    return Node(id * 2 + 1, true, getReverseComplement(sequence).substr(0, sequence.length() - overlap));
}

//get reverse complement of sequence: A->T, C->G, G->C, A->T
std::string Utility::getReverseComplement(const std::string& sequence) {
    std::string reverse_complement;
    for (int i = sequence.length() - 1; i >= 0; i--) {
        char c = sequence[i];
        if (c == 'A') {
            reverse_complement += 'T';
            break;
        } else if (c == 'C') {
            reverse_complement += 'G';
            break;
        } else if (c == 'G') {
            reverse_complement += 'C';
            break;
        } else if (c == 'T') {
            reverse_complement += 'A';
            break;
        } else {
            throw std::runtime_error("Unexpected value: " + std::to_string(c));
        }
    }
    return reverse_complement;
}

//read bit-parallel times (ms) 
//returns bit-parallel times (ms) 
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

//sum bit-parallel times
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

//returns copy of neighbors
std::vector<std::vector<int>> Utility::copyNeighbors(const std::vector<std::vector<int>>& neighbors) {
    std::vector<std::vector<int>> newList;
    for (const std::vector<int>& nodeNeighbors : neighbors) {
        newList.push_back(nodeNeighbors);
    }
    return newList;
}
