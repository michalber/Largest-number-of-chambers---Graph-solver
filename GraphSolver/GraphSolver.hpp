#ifndef __GRAPHSOLVER_H__
#define __GRAPHSOLVER_H__

#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace solver
{
    constexpr int kPrintSpacer = 20;
    
    class GraphSolver
    {
        using IncidenceGraph = std::vector<std::vector<std::pair<int, int>>>;

    public:
        explicit GraphSolver(const std::string &input_path);
        ~GraphSolver();
        void SolveGraph();
        void PrintIncidenceGraph();

    private:
        void CreateIncidenceGraph(const std::string &in_file);
        void CheckIfGraphHasOnlyOneEntry();
        
        int GetGraphSize() const
        {
            return this->incidence_graph_.size();
        }

    private:
        IncidenceGraph incidence_graph_;
    };    
} // namespace solver

#endif // __GRAPHSOLVER_H__