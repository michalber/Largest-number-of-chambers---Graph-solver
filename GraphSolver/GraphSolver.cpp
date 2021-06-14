#include "GraphSolver.hpp"

namespace solver
{
    GraphSolver::GraphSolver(const std::string &input_path, int max_cost) : max_cost_(max_cost)
    {
        this->CreateIncidenceGraph(input_path);
        this->PrintIncidenceGraph();
        this->CreateAdjacenctListFromGraph();
        this->FindRootVertexOfGraph();
        this->CheckIfGraphHasOnlyOneEntry();
    }

    GraphSolver::~GraphSolver()
    {
    }

    void GraphSolver::SolveGraph()
    {
        this->FindLongestDistance();
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
            PrintElement("Edge : Weight " + std::to_string(cnt++));
        }
        std::cout << std::endl;
        cnt = 0;
        for (auto &line : this->incidence_graph_)
        {
            PrintElement("State " + std::to_string(cnt++));
            for (auto &el : line)
            {
                PrintElement(std::to_string(el.first) + " : " + std::to_string(el.second));
            }
            std::cout << std::endl;
        }
    }

    void GraphSolver::FindLongestDistance()
    {
        // departure[] stores vertex number having its departure
        // time equal to the index of it
        std::vector<int> departure(this->GetGraphSize(), -1);

        // to keep track of whether a vertex is discovered or not
        std::vector<bool> discovered(this->GetGraphSize());
        int time = 0;

        // perform DFS on all undiscovered vertices
        for (int i = 0; i < this->GetGraphSize(); i++)
        {
            if (!discovered[i])
            {
                DFS(i, discovered, departure, time);
            }
        }

        std::vector<int> cost(this->GetGraphSize(), INT_MAX);
        cost[root_vertex_] = 0;

        std::vector<int> paths(this->GetGraphSize(), 1);

        // Process the vertices in topological order, i.e., in order
        // of their decreasing departure time in DFS
        for (int i = this->GetGraphSize() - 1; i >= 0; i--)
        {
            // for each vertex in topological order,
            // relax the cost of its adjacent vertices
            int v = departure[i];
            for (Edge e : this->graph_.GetAdjacencyList()[v])
            {
                // edge `e` from `v` to `u` having weight `w`
                int u = e.dest;
                int w = e.weight * -1; // make edge weight negative

                // if the distance to destination `u` can be shortened by
                // taking edge `v —> u`, then update cost to the new lower value
                if (cost[v] != INT_MAX && cost[v] + w < cost[u])
                {
                    cost[u] = cost[v] + w;
                    paths[u]++;
                }
            }
        }

        for (int i = 0; i < this->GetGraphSize(); i++)
            cost[i] *= -1;

        // print the longest paths
        results_.resize(this->GetGraphSize());

        for (int i = 0; i < this->GetGraphSize(); i++)
        {
            std::cout << "dist(" << root_vertex_ << ", " << i << ") = " << std::setw(2) << cost[i] << std::endl;
            const auto len = FindAllPaths(this->root_vertex_, i);
            results_[i] = {cost[i], len};
        }

        std::sort(results_.begin(), results_.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
                  { return a.first > b.first; });

        if (results_[0].first <= max_cost_)
        {
            std::sort(results_.begin(), results_.end(), [](const std::pair<int, int> &a, const std::pair<int, int> &b)
                      { return a.second > b.second; });
            std::cout << "Having budget of " << this->max_cost_ << ", largest number of chambers is: " << results_[0].second << std::endl;
        }
        else
        {
            for (const auto &el : results_)
            {
                if(el.first <= max_cost_)
                {
                    std::cout << "Having budget of " << this->max_cost_ << ", largest number of chambers is: " << el.second << std::endl;
                    return;
                }
            }
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
        for (int i = 0; i < this->GetGraphSize(); i++)
        {
            int cnt = 0;
            for (int j = 0; j < this->GetGraphSize(); j++)
            {
                cnt += this->incidence_graph_[i][j].first;
            }
            if (cnt == 0)
                return;
        }
        throw std::invalid_argument("Input graph does not have single entrance");
    }

    void GraphSolver::FindRootVertexOfGraph()
    {
        // to keep track of all previously visited vertices in DFS
        std::vector<int> departure(this->GetGraphSize(), -1);

        // to keep track of whether a vertex is discovered or not
        std::vector<bool> visited(this->GetGraphSize());

        // find the last starting vertex `v` in DFS
        int v = 0;
        int time = 0;
        for (int i = 0; i < this->GetGraphSize(); i++)
        {
            if (!visited[i])
            {
                DFS(i, visited, departure, time);
                v = i;
            }
        }

        // reset the visited vertices
        std::fill(visited.begin(), visited.end(), false);

        // perform DFS on the graph from the last starting vertex `v`
        DFS(v, visited, departure, time);

        // return -1 if all vertices are not reachable from vertex `v`
        for (int i = 0; i < this->GetGraphSize(); i++)
        {
            if (!visited[i])
            {
                return;
            }
        }

        std::cout << "Root vertex of graph is: " << v << std::endl;
        root_vertex_ = v;
    }

    void GraphSolver::CreateAdjacenctListFromGraph()
    {
        this->graph_.SetSize(this->GetGraphSize());

        for (int i = 0; i < this->GetGraphSize(); i++)
        {
            for (int j = 0; j < this->GetGraphSize(); j++)
            {
                if (this->incidence_graph_[j][i].first == 1)
                {
                    this->graph_.AddEdge(Edge{.src = i,
                                              .dest = j,
                                              .weight = this->incidence_graph_[j][i].second});
                }
            }
        }
    }

    void GraphSolver::DFS(int v, std::vector<bool> &discovered, std::vector<int> &departure, int &time)
    {
        // mark the current node as discovered
        discovered[v] = true;

        // set arrival time – not needed
        // time++;

        // do for every edge `v —> u`
        for (Edge e : this->graph_.GetAdjacencyList()[v])
        {
            int u = e.dest;
            // if `u` is not yet discovered
            if (!discovered[u])
            {
                DFS(u, discovered, departure, time);
            }
        }

        // ready to backtrack
        // set departure time of vertex `v`
        departure[time] = v;
        time++;
    }

} // namespace solver
