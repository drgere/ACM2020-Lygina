#include "graph.h"
#include "assigment.h"

CompleteGraph::CompleteGraph(size_t n) : adjacency_(n, std::vector<EntryType>(n)) {
}

CompleteGraph::CompleteGraph(AdjacencyMatrix matrix) : adjacency_(std::move(matrix)) {
}

int& CompleteGraph::GetEdge(size_t from, size_t to) {
    return adjacency_[from][to];
}

int CompleteGraph::GetEdge(size_t from, size_t to) const {
    return adjacency_[from][to];
}

void CompleteGraph::Fill(std::function<EntryType(size_t, size_t)> filler) {
    for (size_t i = 0; i < adjacency_.size(); ++i) {
        for (size_t j = 0; j < adjacency_.size(); ++j) {
            adjacency_[i][j] = filler(i,j);
        }
    }
}

std::vector<std::vector<size_t>> CompleteGraph::GetMaximumCycleCover() {
    AssigmentProblemSolver solver(adjacency_);
    std::vector<size_t> assigment = solver.Solve(SolverType::Greedy);

    std::vector<std::vector<size_t>> result;

    std::vector<int> used(adjacency_.size(), 0);
    std::vector<size_t> cycle;

    size_t it = 0;
    
    while (it < adjacency_.size()) {
        if (cycle.empty()) {
            if (!used[it]) {
                cycle.push_back(it);
                used[it] = 1;
            }
            it++;
        } else {
            size_t next = assigment[cycle.back()];
            if (next == cycle.front()) {
                result.push_back(cycle);
                cycle.clear();
            } else {
                cycle.push_back(next);
                used[next] = 1;
            }
        }
    }
    return result;
}