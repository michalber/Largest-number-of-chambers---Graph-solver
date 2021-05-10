#include "GraphSolver.hpp"

namespace solver
{
    GraphSolver::GraphSolver(const std::string &input_path)
    {
        this->CreateIncidenceGraph(input_path);
        this->PrintIncidenceGraph();
        this->CheckIfGraphHasOnlyOneEntry();
    }

    GraphSolver::~GraphSolver()
    {
    }

    void GraphSolver::SolveGraph()
    {
    }

    template <typename T>
    void PrintElement(T t)
    {
        std::cout << std::left << std::setw(kPrintSpacer) << std::setfill(' ') << t << std::right << "|";
    }

    void GraphSolver::PrintIncidenceGraph()
    {
        int cnt = 0;
        PrintElement("");
        for (auto _ : this->incidence_graph_)
        {
            PrintElement("Edge : Weight " + std::to_string(++cnt));
        }
        std::cout << std::endl;
        cnt = 0;
        for (auto &line : this->incidence_graph_)
        {
            PrintElement("State " + std::to_string(++cnt));
            for (auto &el : line)
            {
                PrintElement(std::to_string(el.first) + " : " + std::to_string(el.second));
            }
            std::cout << std::endl;
        }
    }

    void GraphSolver::CreateIncidenceGraph(const std::string &in_file)
    {
        std::ifstream input_file;
        input_file.open(in_file);
        if (!input_file.is_open())
        {
            std::cerr << "Could not open file " << in_file << "\n";
            throw std::invalid_argument("File not opened.");
        }

        int l = 0;

        while (input_file)
        {
            l++;
            std::string s;
            if (!getline(input_file, s))
                break;
            if (s[0] != '#')
            {
                std::istringstream ss(s);
                std::vector<std::pair<int, int>> record;

                while (ss)
                {
                    std::string line;
                    if (!std::getline(ss, line, ';'))
                        break;
                    try
                    {
                        int j = 0;
                        std::stringstream line_stream(line);
                        std::string el;
                        while (std::getline(line_stream, el, ','))
                        {
                            std::stringstream element_stream(el);
                            std::string edge_internal, edge_weight_internal;

                            std::getline(element_stream, edge_internal, '?');
                            std::getline(element_stream, edge_weight_internal, '?');
                            
                            record.push_back({stof(edge_internal), stof(edge_weight_internal)});
                        }
                    }
                    catch (const std::invalid_argument e)
                    {
                        std::cout << "NaN found in file " << in_file << " line " << l
                                  << std::endl;
                        e.what();
                    }
                }
                this->incidence_graph_.push_back(record);
            }
        }

        if (!input_file.eof())
        {
            std::cerr << "Could not read file " << in_file << "\n";
            throw std::invalid_argument("File not found.");
        }
    }
    
    void GraphSolver::CheckIfGraphHasOnlyOneEntry() 
    {
        for(int i = 0; i < this->GetGraphSize(); i++)
        {
            int cnt = 0;
            for(int j = 0; j < this->GetGraphSize(); j++)
            {
                cnt += this->incidence_graph_[j][i].first;
            }
            if(cnt == 1)
                return;
        }
        throw std::invalid_argument("Input graph does not have single entrance");
    }

} // namespace solver
