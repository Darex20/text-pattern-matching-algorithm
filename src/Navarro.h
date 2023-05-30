#include "Graph.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <climits>

class Navarro {
	private:
	    Graph graph;

		// vector that contains indexes of nodes in topological order
	    vector<int> topologicalOrder; 

		// Navarro g function to calculate C values
	    void g(vector<int>& realEditDistances, vector<int>& iterationEditDistances, const string& pattern, int j, int node);
		
	public:
		Navarro(Graph& graph, const vector<int>& topologicalOrder);
		int align(string pattern);
		int alignCycle(string pattern);
		void propagate(int u, int v, vector<int>& realEditDistances);
};
