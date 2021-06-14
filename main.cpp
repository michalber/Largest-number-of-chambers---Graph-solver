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
    // const std::string input_path = GetCmdOption(argc, argv, "-in=");
    // const std::string max_length = GetCmdOption(argc, argv, "-len=");

    // if (input_path == "")
    // {
    //     throw std::invalid_argument("Please give input CSV file");
    // }
    // if (max_length == "")
    // {
    //     throw std::invalid_argument("Please give input max length");
    // }

    // solver::GraphSolver solver(input_path, std::stoi(max_length));
    solver::GraphSolver solver("../InputGraph.csv", 8);
    solver.SolveGraph();
}
