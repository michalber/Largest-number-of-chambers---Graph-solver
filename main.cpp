#include "GraphSolver/GraphSolver.hpp"

std::string GetCmdOption(int argc, char* argv[], const std::string& option)
{
    std::string cmd;
     for( int i = 0; i < argc; ++i)
     {
          std::string arg = argv[i];
          if(0 == arg.find(option))
          {
               std::size_t found = arg.find_first_of(option);
               cmd =arg.substr(found + option.size());
               return cmd;
          }
     }
     return cmd;
}

int main(int argc, char** argv) {
    const std::string input_path = GetCmdOption(argc, argv, "-in=");
    
    if(input_path == "")
    {
        throw std::invalid_argument("Please give input CSV file");
    }

    solver::GraphSolver solver(input_path);
    solver.SolveGraph();
}
