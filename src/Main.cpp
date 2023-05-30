#include "Navarro.h"
#include "FileReader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <windows.h>

using namespace std;

void run_for_one_graph(const string& graphName, const string& fastqName, ofstream& writer);

int main () {
	ofstream writer("../summary.txt", ios::app);
	vector<string> graphNames = {"linear.gfa", "twopath.gfa", "snp.gfa", "tangle.gfa"};
    string fastqName = "simulatedreads.fastq";

	string onlyLinear;
	cout << "Do you want to run the algorithm for only linear graph? (yes/no) ";
	getline(cin, onlyLinear);

	if (onlyLinear == "yes"){
		run_for_one_graph("linear.gfa", fastqName, writer);
	} else if (onlyLinear == "no"){
		for (const auto& graphName : graphNames) {
			run_for_one_graph (graphName, fastqName, writer);
		}
	} else {
		cout << "Invalid input.";
	}

    return 0;
}

void run_for_one_graph(const string& graphName, const string& fastqName, ofstream& writer) {
    auto startTime = chrono::high_resolution_clock::now();
    
    string bit_parallel_file = graphName.substr(0, graphName.length() - 3) + "txt";
    string bit_parallel_scores_file = "../data/bit_parallel_scores/" + bit_parallel_file;
    string bit_parallel_time_file = "../data/bit_parallel_times/" + bit_parallel_file;
    string bit_parallel_memory_file = "../data/bit_parallel_memory_usage/" + bit_parallel_file;
    string graphFilePath = "../data/graphs/" + graphName;
    string fastqFilePath = "../data/graphs/" + fastqName;
	Graph graph = FileReader::readGraph("../data/graphs/" + graphName);
	vector<string> fastqs = FileReader::readFastq("../data/graphs/" + fastqName);
	vector<int> bit_parallel_scores = FileReader::loadScores(bit_parallel_scores_file);
	int bit_parallel_time = FileReader::loadTime(bit_parallel_time_file);
    int bit_parallel_memory_usage = FileReader::loadMemory(bit_parallel_memory_file);
    
    bool hasCycle = false;
	vector<int> topologicalOrder;
	
	try {
	    topologicalOrder = graph.topologicalOrderOfNodesAcyclic();
	} catch (const runtime_error& e) {
	    hasCycle = true;
	    topologicalOrder = graph.topologicalOrderOfNodesCyclic();
	}
	
	Navarro navarro = Navarro(graph, topologicalOrder);
    vector<int> scores;
    int i = 0;
    cout << "Navarro implementation score - Bit-parallel alignment score" << endl;
    
    for (const auto& fastq : fastqs) {
    	int score = 0;
    	if (!hasCycle) {
    		score = navarro.align(fastq);
		} else {
			score = navarro.alignCycle (fastq);
		}
		int difference = bit_parallel_scores[i] - score;
		cout << "Navarro: " << score << " ---- Bit-parallel: " << bit_parallel_scores[i] << " -> difference = " << difference << endl;
	    scores.push_back(score);
	    i++;
	}
	
	auto endTime = chrono::high_resolution_clock::now();

	int correct = 0;
	for (size_t i = 0; i < scores.size(); i++) {
	    if (scores[i] == bit_parallel_scores[i]) {
	        correct++;
	    }
	}
	
	MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    DWORDLONG totalVirtualMem = memInfo.ullTotalPageFile;
    DWORDLONG virtualMemUsed = memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
	DWORDLONG physMemUsed = memInfo.ullTotalPhys - memInfo.ullAvailPhys;
    
	cout << endl;
    cout << "Algorithm was run on: " << graphName << endl;
    cout << "Overall score: " << correct << "/" << scores.size() << endl;
    double bit_parallel_time_in_seconds = bit_parallel_time / 1000000.0;
    double our_time_in_seconds = chrono::duration<double>(endTime - startTime).count();
    double time_ratio = our_time_in_seconds / bit_parallel_time_in_seconds;
    double bit_parallel_memory_usage_in_kb = bit_parallel_memory_usage / 1024.0;
    double navarro_usage_in_kb = static_cast<double>(physMemUsed) / 1024.0;
    double memory_ratio = navarro_usage_in_kb / bit_parallel_memory_usage_in_kb;
    cout << "Bit parallel sequence-to-graph alignment algorithm time was: " << bit_parallel_time_in_seconds << "s" << endl;
    cout << "Navarro algorithm implementation time was: " << our_time_in_seconds << "s" << endl;
    cout << "Bit parallel sequence-to-graph alignment algorithm memory usage was: " << bit_parallel_memory_usage_in_kb << "KB" << endl;
    cout << "Navarro algorithm implementation memory usage was: " << navarro_usage_in_kb << "KB" << endl;
    cout << endl;
	
	try {
		ofstream outputFile;
		outputFile.open("../summary.txt", std::ios_base::app);
		outputFile << "Algorithm was run on: " << graphName << endl;
		outputFile << "Overall score: " << correct << "/" << scores.size() << "\n";
		outputFile << "Bit parallel sequence-to-graph alignment algorithm time was: " << bit_parallel_time_in_seconds << " s\n";
		outputFile << "Navarro algorithm implementation time was: " << our_time_in_seconds << " s\n";
		outputFile << "Bit parallel sequence-to-graph alignment algorithm memory usage was: " << bit_parallel_memory_usage_in_kb << "KB\n";
		outputFile << "Navarro algorithm implementation memory usage was: " << navarro_usage_in_kb << "KB\n" << endl;
		outputFile.close();
	} catch (...) {
		cout << "Error" << endl;
		
	}
	
}

