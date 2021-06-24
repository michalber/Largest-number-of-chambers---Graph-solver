#include "GraphSolver.hpp"

namespace solver
{
    GraphSolver::GraphSolver(const std::string &input_path, int max_cost) : max_cost_(max_cost)
    {
        this->CreateIncidenceGraph(input_path);
        this->PrintIncidenceGraph();
        this->CreateAdjacenctListFromGraph();

        // Used in directed graph. Now we use undirected
        // this->FindRootVertexOfGraph();
        // this->CheckIfGraphHasOnlyOneEntry();
    }

    GraphSolver::~GraphSolver()
    {
    }

    void GraphSolver::SolveGraph()
    {
        // Find all paths from input vertex to all other vertexes
        for (int i = 0; i < this->GetGraphSize(); i++)
        {
            this->FindAllPaths(0, i);
        }

        this->CalcCostOfPaths();
        this->SortPathsDescendingSize();

        // Print paths
        for (const auto &vec : this->allpaths_)
        {
            std::cout << "Path: {";
            for (const auto &el : vec.first)
            {
                std::cout << el;
                if (el != vec.first.back())
                    std::cout << " -> ";
            }
            std::cout << "} | Cost of path is: " << vec.second;
            std::cout << std::endl;
        }

        FindMaxBombsPlanted();
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
        this->SetGraphSize(this->incidence_graph_.size());

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
        // // to keep track of all previously visited vertices in DFS
        // std::vector<int> departure(this->GetGraphSize(), -1);

        // // to keep track of whether a vertex is discovered or not
        // std::vector<bool> visited(this->GetGraphSize());

        // // find the last starting vertex `v` in DFS
        // int v = 0;
        // int time = 0;
        // for (int i = 0; i < this->GetGraphSize(); i++)
        // {
        //     if (!visited[i])
        //     {
        //         DFS(i, visited, departure, time);
        //         v = i;
        //     }
        // }

        // // reset the visited vertices
        // std::fill(visited.begin(), visited.end(), false);

        // // perform DFS on the graph from the last starting vertex `v`
        // DFS(v, visited, departure, time);

        // // return -1 if all vertices are not reachable from vertex `v`
        // for (int i = 0; i < this->GetGraphSize(); i++)
        // {
        //     if (!visited[i])
        //     {
        //         throw std::invalid_argument("Graph does not have root vertex.");
        //         return;
        //     }
        // }

        // std::cout << "Root vertex of graph is: " << v << std::endl;
        // root_vertex_ = v;
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

    void GraphSolver::FindAllPaths(int s, int d)
    {
        // Mark all the vertices as not visited
        bool visited[this->GetGraphSize()] = {false};

        // Create an array to store paths
        int path[this->GetGraphSize()] = {-1};

        int path_index = 0; // Initialize path[] as empty

        // Initialize all vertices as not visited
        for (int i = 0; i < this->GetGraphSize(); i++)
            visited[i] = false;

        // Call the recursive helper function to print all paths
        FindAllPathsUtil(s, d, visited, path, path_index);
    }

    void GraphSolver::FindAllPathsUtil(int u, int d, bool visited[],
                                       int path[], int &path_index)
    {
        // Mark the current node and store it in path[]
        visited[u] = true;
        path[path_index] = u;
        path_index++;

        // If current vertex is same as destination, then print
        // current path[]
        if (u == d)
        {
            std::vector<int> path_vec;
            for (int i = 0; i < path_index; i++)
            {
                // std::cout << path[i] << " ";
                path_vec.push_back(path[i]);
            }
            this->allpaths_.push_back({path_vec, 0});
            // std::cout << std::endl;
        }
        else // If current vertex is not destination
        {
            // Recur for all the vertices adjacent to current vertex
            for (const auto &node : this->graph_.GetAdjacencyList()[u])
            {
                if (!visited[node.dest])
                    FindAllPathsUtil(node.dest, d, visited, path, path_index);
            }
        }

        // Remove current vertex from path[] and mark it as unvisited
        path_index--;
        visited[u] = false;
    }

    void GraphSolver::CalcCostOfPaths()
    {
        // Print paths
        for (auto &vec : this->allpaths_)
        {
            for (int i = 0; i < vec.first.size() - 1; i++)
            {
                //                                            row-dest  col-source
                vec.second += this->incidence_graph_[vec.first[i + 1]][vec.first[i]].second;
            }
        }
    }

    void GraphSolver::SortPathsDescendingSize()
    {
        std::sort(this->allpaths_.begin(), this->allpaths_.end(),
                  [](const PathWithCost &a, const PathWithCost &b)
                  { return a.first.size() > b.first.size(); });
    }

    void GraphSolver::FindMaxBombsPlanted()
    {
        // Print paths
        for (auto &vec : this->allpaths_)
        {
            if (vec.second <= this->max_cost_)
            {
                std::cout << "\n"
                          << "Max bombs planted is: " << vec.first.size() << std::endl;

                std::cout << "Path: {";
                for (const auto &el : vec.first)
                {
                    std::cout << el;
                    if (el != vec.first.back())
                        std::cout << " -> ";
                }
                std::cout << "} | Cost of path is: " << vec.second;
                std::cout << std::endl;
                return;
            }
        }
    }

} // namespace solver
