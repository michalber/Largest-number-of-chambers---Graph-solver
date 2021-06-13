#include "Graph.hpp"

Graph::Graph(std::vector<Edge> const &edges, int N)
{
    // resize the vector to hold `N` elements of type vector<Edge>
    adjacency_list_.resize(N);

    // add edges to the directed graph
    for (Edge const &edge : edges)
    {
        adjacency_list_[edge.src].push_back(edge);
    }
}

Graph::AdjacencyList &Graph::GetAdjacencyList()
{
    return this->adjacency_list_;
}

void Graph::AddEdge(const Edge &edge)
{
    adjacency_list_[edge.src].push_back(edge);
}

void Graph::AddEdge(std::vector<Edge> const &edges, int N)
{
    // resize the vector to hold `N` elements of type vector<Edge>
    adjacency_list_.resize(N);

    // add edges to the directed graph
    for (Edge const &edge : edges)
    {
        adjacency_list_[edge.src].push_back(edge);
    }
}

void Graph::SetSize(const int new_size) 
{
    adjacency_list_.resize(new_size);
}
