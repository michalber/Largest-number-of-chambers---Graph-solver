#ifndef __GRAPH_H__
#define __GRAPH_H__

#include <vector>

// Data structure to store a graph edge
struct Edge
{
    int src, dest, weight;
};

class Graph
{
    using AdjacencyList = std::vector<std::vector<Edge>>;

public:
    Graph() = default;
    Graph(std::vector<Edge> const &edges, int N);
    ~Graph() = default;

    AdjacencyList &GetAdjacencyList();
    void AddEdge(const Edge &edge);
    void AddEdge(std::vector<Edge> const &edges, int N);
    void SetSize(const int new_size);

private:
    AdjacencyList adjacency_list_;
};

#endif // __GRAPH_H__