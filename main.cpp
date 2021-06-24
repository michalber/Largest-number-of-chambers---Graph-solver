#include "GraphSolver/GraphSolver.hpp"

std::string GetCmdOption(int argc, char *argv[], const std::string &option)
{
    std::string cmd;
    for (int i = 0; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (0 == arg.find(option))
        {
            std::size_t found = arg.find_first_of(option);
            cmd = arg.substr(found + option.size());
            return cmd;
        }
    }
    return cmd;
}

int main(int argc, char **argv)
{
    const std::string input_path = GetCmdOption(argc, argv, "-in=");
    const std::string max_length = GetCmdOption(argc, argv, "-cost=");

    // Uncomment both when using with command line
    // if (max_length == "")
    // {
    //     throw std::invalid_argument("Please give input max cost");
    // }
    // if (input_path == "")
    // {
    //     throw std::invalid_argument("Please give input graph file");
    // }

    // To use with command line
    // solver::GraphSolver graph_solver(input_path, std::stoi(max_length));

    // Debug use
    solver::GraphSolver graph_solver("../InputGraph.csv", 5);

    graph_solver.SolveGraph();
}
