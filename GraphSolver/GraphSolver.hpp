#ifndef __GRAPHSOLVER_H__
#define __GRAPHSOLVER_H__

#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <climits>
#include <algorithm>

#include "../Graph/Graph.hpp"

namespace solver
{
    constexpr int kPrintSpacer = 20;

    class GraphSolver
    {
        using IncidenceGraph = std::vector<std::vector<std::pair<int, int>>>;

    public:
        explicit GraphSolver(const std::string &input_path, int max_len);
        ~GraphSolver();
        void SolveGraph();
        void PrintIncidenceGraph();
        void FindLongestDistance();

    private:
        void CreateIncidenceGraph(const std::string &in_file);
        void CheckIfGraphHasOnlyOneEntry();
        void CreateAdjacenctListFromGraph();
        
        int GetGraphSize() const
        {
            return this->incidence_graph_.size();
        }

        void DFS(int v, std::vector<bool> &discovered, std::vector<int> &departure, int &time);

    private:
        IncidenceGraph incidence_graph_;
        Graph graph_;
        int max_length = 0;
    };    
} // namespace solver

#endif // __GRAPHSOLVER_H__