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
        using IncidenceGraph = std::vector<std::vector<std::pair<int, int>>>;

    public:
        explicit GraphSolver(const std::string &input_path, int max_cost);
        ~GraphSolver();
        void SolveGraph();
        void PrintIncidenceGraph();
        void FindLongestDistance();

    private:
        void CreateIncidenceGraph(const std::string &in_file);
        void CheckIfGraphHasOnlyOneEntry();
        void FindRootVertexOfGraph();
        void CreateAdjacenctListFromGraph();

        int GetGraphSize() const
        {
            return this->incidence_graph_.size();
        }

        void DFS(int v, std::vector<bool> &discovered, std::vector<int> &departure, int &time);

        int FindAllPaths(int src, int dst)
        {
            // Clear previously stored paths if any
            allpaths.clear();
            std::vector<int> path;

            // As the path begins with source, push the source node into the path
            path.push_back(src);
            std::cout << "Path: ";
            DFS_PathSearch(src, dst, path);
            // Print all paths in the graph
            // Print();
            std::sort(allpaths.begin(), allpaths.end(), [](const std::vector<int> &a, const std::vector<int> &b)
                      { return a.size() > b.size(); });

            for (const auto &node : allpaths[0])
                std::cout << node << " ";
            std::cout << "\n"
                      << std::endl;

            return allpaths[0].size();
        }

        void Print()
        {
            // Print all the paths
            for (const auto &path : allpaths)
            {
                std::cout << "Path : ";
                for (const auto &node : path)
                    std::cout << node << " ";
                std::cout << std::endl;
            }
        }

        void DFS_PathSearch(int src, int dst, std::vector<int> &path)
        {

            if (src == dst)
            {
                allpaths.push_back(path);
            }
            else
            {
                for (const auto &adjnode : this->graph_.GetAdjacencyList()[src])
                {
                    path.push_back(adjnode.dest);
                    DFS_PathSearch(adjnode.dest, dst, path);
                    path.pop_back();
                }
            }
        }

    private:
        IncidenceGraph incidence_graph_;
        Graph graph_;
        int max_cost_ = 0;
        int root_vertex_ = 0;

        std::vector<std::vector<int>> allpaths;
        std::vector<std::pair<int, int>> results_;
    };
} // namespace solver

#endif // __GRAPHSOLVER_H__