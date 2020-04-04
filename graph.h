#pragma once
#include <vector>
#include <functional>

class CompleteGraph {
public:
    using EntryType = int;
    using AdjacencyMatrix = std::vector<std::vector<EntryType>>;

    // Create complete matrix with T{} weights
    CompleteGraph(size_t n_vertices);

    // Copy matrix
    CompleteGraph(AdjacencyMatrix matrix);

    // Get reference to edge weight
    int& GetEdge(size_t from, size_t to);

    // Get value of edge weight
    int GetEdge(size_t from, size_t to) const;

    // Fill adjacency matrix via specific function
    void Fill(std::function<EntryType(size_t, size_t)> filler);

    // Find approximate maximal cycle cover using assigment problem solver 
    std::vector<std::vector<size_t>> GetMaximumCycleCover();

private:
    AdjacencyMatrix adjacency_;
};