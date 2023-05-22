#include "NodeUtils.h"
#include "Aligner.h"

#include <iostream>
#include <fstream>
#include <cassert>
#include <sstream>

//creates node with overlap
//returns new Node
Node NodeUtils::createNode(const std::string& line, int overlap) {
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

//get reverse complement of sequence: A->T, C->G, G->C, A->T
Node NodeUtils::createReverseComplementNode(const std::string& line, int overlap) {
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

//creates reverse complement node with overlap
//returns new Node
std::string NodeUtils::getReverseComplement(const std::string& sequence) {
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

//returns copy of neighbors
std::vector<std::vector<int>> NodeUtils::copyNeighbors(const std::vector<std::vector<int>>& neighbors) {
    std::vector<std::vector<int>> newList;
    for (const std::vector<int>& nodeNeighbors : neighbors) {
        newList.push_back(nodeNeighbors);
    }
    return newList;
}
