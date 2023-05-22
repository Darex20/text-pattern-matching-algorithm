#include "Graph.h"

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <climits>

class Aligner {
	private:
	    Graph graph; //graph
	    vector<int> topologicalOrder; //vector that contains indexes of nodes in topological order
	    void f(vector<int>& realEditDistances, vector<int>& iterationEditDistances, const string& pattern, int j, int node);
	public:
		Aligner(Graph& graph, const vector<int>& topologicalOrder);
		int align(string pattern);
		int alignCycle(string pattern);
		void propagate(int u, int v, vector<int>& realEditDistances);
};
