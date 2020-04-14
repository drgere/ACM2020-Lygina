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
        } 
        if (!cycle.empty()) {
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

std::vector<size_t> CompleteGraph::GetLongestPath() {
    std::vector<size_t> path;
    if (adjacency_.size() > 32) {
        // Spends all memory otherwise
        for (size_t i = 0; i < adjacency_.size(); ++i) {
            path.push_back(i);
        }
        return path;
    }
    
    int path_len = GetLongestPathFrom(0, path);
    for (size_t i = 1; i < adjacency_.size(); ++i) {
        std::vector<size_t> new_path;
        int new_len = GetLongestPathFrom(1, new_path);
        if (new_len > path_len) {
            path_len = new_len;
            path = new_path;
        }
    }
    return path;
}

std::vector<size_t> MaskDecode(size_t mask, size_t max_len=32) {
    std::vector<size_t> res;
    for (size_t i = 0; i < max_len; ++i) {
        if (mask & (1 << i)) {
            res.push_back(i);
        }
    }
    return res;
}


int CompleteGraph::GetLongestPathFrom(size_t s, std::vector<size_t>& path) {
    size_t max_mask = (1 << adjacency_.size());
    std::vector<std::vector<int>> dp(adjacency_.size(), std::vector<int>(max_mask));
    std::vector<std::vector<int>> prev(adjacency_.size(), std::vector<int>(max_mask, -1));
    for (int k = 0; k < adjacency_.size(); ++k) {
        if (k == s) {
            continue;
        }
        dp[k][1 << k] = GetEdge(s, k);
    }
    for (int len = 2; len < adjacency_.size(); ++len) {
        for (size_t mask = 1; mask < max_mask; ++mask) {
            auto indexies = MaskDecode(mask, adjacency_.size());
            if (indexies.size() != len) {
                continue;
            }
            bool has_s = false;
            for (size_t v : indexies) {
                if (v == s) {
                    has_s = true;
                }
            }
            if (has_s) {
                continue;
            }

            for (size_t v : indexies) {
                size_t prev_mask = mask ^ (1 << v); // v-th bit was set
                for (size_t u : indexies) {
                    if (v == u) {
                        continue;
                    }
                    if (dp[u][prev_mask] + GetEdge(u, v) > dp[v][mask]) {
                        dp[v][mask] = dp[u][prev_mask] + GetEdge(u, v);
                        prev[v][mask] = u;
                    }   
                }
            }
        }
    }

    size_t full_mask = (max_mask - 1) ^ (1 << s);
    int opt = 0;
    int opt_v = 0;
    for (int v = 0; v < adjacency_.size(); ++v) {
        if (v == s) {
            continue;
        }
        if (dp[v][full_mask] > opt) {
            opt = dp[v][full_mask];
            opt_v = v;
        }
    }

    // Answer recovery
    path.clear();
    size_t cur_mask = full_mask;
    while (opt_v != -1) {
        path.push_back(opt_v);
        int next_v = prev[opt_v][cur_mask];
        cur_mask = cur_mask ^ (1 << opt_v);
        opt_v = next_v;
    }
    path.push_back(s);
    std::reverse(path.begin(), path.end());
    return opt;
}