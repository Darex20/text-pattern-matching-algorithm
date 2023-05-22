#include "Aligner.h"
#include "FileReader.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

void run_for_one_graph(const string& graphName, const string& fastqName, ofstream& writer);

//print solutions, read files 
int main () {
	ofstream writer("../summary.txt", ios::app);
	vector<string> graphNames = {"ref10000_linear.gfa", "ref10000_snp.gfa", "ref10000_tangle.gfa",
                                 "ref10000_twopath.gfa"};
    string fastqName = "ref10000_simulatedreads.fastq";
    auto startTime = chrono::high_resolution_clock::now();
	
	for (const auto& graphName : graphNames) {
		run_for_one_graph (graphName, fastqName, writer);
	}
	auto endTime = chrono::high_resolution_clock::now();
	
	double total_bit_parallel_time_in_seconds = FileReader::add_bit_parallel_times(graphNames);
    double our_total_time =  (endTime - startTime).count() / 1000000000;
    double ratio = our_total_time / total_bit_parallel_time_in_seconds;
    
    cout << "Total:" << endl;
    cout << "bit-parallel took: " << total_bit_parallel_time_in_seconds << "s" << endl;
    cout << "Navarro implementation took: " << our_total_time << "s" << endl;
    cout << "time ratio: " << ratio << endl;
    cout << endl;
    
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
	
	Aligner aligner = Aligner(graph, topologicalOrder);
    vector<int> scores;
    int i = 0;
    cout << "out score/bit-parallel score" << endl;
    
    for (const auto& fastq : fastqs) {
    	int score = 0;
    	if (!hasCycle) {
    		score = aligner.align(fastq);
		} else {
			score = aligner.alignCycle (fastq);
		}
		cout << score << "/" << bit_parallel_scores[i] << endl;
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
	

	
	cout << "graph file: " << graphName << endl;
	cout << "score: " << correct << "/" << scores.size() << endl;
	double bit_parallel_time_in_seconds = bit_parallel_time / 1000000.0;
	double our_time_in_seconds = chrono::duration<double>(endTime - startTime).count();
	double time_ratio = our_time_in_seconds / bit_parallel_time_in_seconds;
	double bit_parallel_memory_usage_in_kb = bit_parallel_memory_usage / 1024.0;
	double navarro_usage_in_kb = static_cast<double>(1024.0) / 1024.0;
	double memory_ratio = navarro_usage_in_kb / bit_parallel_memory_usage_in_kb;
	cout << "bit parallel took: " << bit_parallel_time_in_seconds << "s" << endl;
	cout << "Navarro implementation took: " << our_time_in_seconds << "s" << endl;
	cout << "time ratio: " << time_ratio << endl;
	cout << "bit parallel memory usage: " << bit_parallel_memory_usage_in_kb << "KB" << endl;
	cout << "Navarro memory usage: " << navarro_usage_in_kb << "KB" << endl;
	cout << "memory ratio: " << memory_ratio << endl;
	cout << endl;
	
	try {
		ofstream outputFile;
		outputFile.open("../summary.txt", std::ios_base::app);
		outputFile << graphName << ":\n";
		outputFile << "score: " << correct << "/" << scores.size() << "\n";
		outputFile << "bit parallel took " << bit_parallel_time_in_seconds << " s\n";
		outputFile << "Navarro took " << our_time_in_seconds << " s\n";
		outputFile << "time ratio: " << time_ratio << "\n";
		outputFile << "bit parallel memory usage: " << bit_parallel_memory_usage_in_kb << "KB\n";
		outputFile << "Navarro memory usage: " << navarro_usage_in_kb << "KB\n";
		outputFile << "memory ratio: " << memory_ratio << "\n\n";
		outputFile.close();
	} catch (...) {
		cout << "Error" << endl;
		
	}
	
}

