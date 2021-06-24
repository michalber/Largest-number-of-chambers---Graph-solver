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
#include <stack>
#include <list>

#include "../Graph/Graph.hpp"

namespace solver
{
    constexpr int kPrintSpacer = 20;

    class GraphSolver
    {
        using PathWithCost = std::pair<std::vector<int>, int>;

    public:
        explicit GraphSolver(const std::string &input_path, int max_cost);
        ~GraphSolver();
        void SolveGraph();
        void PrintIncidenceGraph();

    private:
        void CreateIncidenceGraph(const std::string &in_file);
        void CheckIfGraphHasOnlyOneEntry();
        void FindRootVertexOfGraph();
        void CreateAdjacenctListFromGraph();

        int GetGraphSize() const
        {
            return this->incidence_graph_.size();
        }

        void SetGraphSize(int size)
        {
            this->graph_size_ = size;
        }

        void FindAllPaths(int s, int d);

        // A recursive function to print all paths from 'u' to 'd'.
        // visited[] keeps track of vertices in current path.
        // path[] stores actual vertices and path_index is current
        // index in path[]
        void FindAllPathsUtil(int u, int d, bool visited[],
                                      int path[], int &path_index);

        void CalcCostOfPaths();
        void SortPathsDescendingSize();
        void FindMaxBombsPlanted();

    private:
        IncidenceGraph incidence_graph_;
        Graph graph_;
        int max_cost_ = 0;
        int root_vertex_ = 0;
        int graph_size_ = 0;

        std::vector<PathWithCost> allpaths_;
    };
} // namespace solver

#endif // __GRAPHSOLVER_H__