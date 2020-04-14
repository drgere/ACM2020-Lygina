#include "superstring.h"
#include "graph.h"

SuperstringSolver::SuperstringSolver(std::vector<std::string> input) : data_(std::move(input)) {
    Filter();
}

std::string SuperstringSolver::Approximation4() {
    if (data_.size() == 0) {
        return "";
    }
    if (data_.size() == 1) {
        return data_[0];
    }

    CompleteGraph overlap_graph(data_.size());
    overlap_graph.Fill([&](size_t i, size_t j) {
        return Overlap(data_[i], data_[j]);
    });
    auto cycle_cover = overlap_graph.GetMaximumCycleCover();
    std::string superstring = "";
    for (auto& cycle : cycle_cover) {
        int start_index = -1;
        int cut_value = 2'000'000'000; // ~ infinity
        int len = cycle.size();
        for (size_t i = 0; i < cycle.size(); ++i) {
            int overlap = overlap_graph.GetEdge(cycle[i], cycle[(i+1) % len]);
            if (cut_value > overlap) {
                cut_value = overlap;
                start_index = i;
            }
        }
        std::rotate(cycle.begin(), cycle.begin() + start_index, cycle.end());
        for (size_t i = 0; i < len-1; ++i) {
            int overlap = overlap_graph.GetEdge(cycle[i], cycle[(i+1) % len]);
            size_t it = cycle[i];
            superstring += std::string_view(data_[it].c_str(), data_[it].size() - overlap);
        }
        superstring += data_[cycle.back()];
    }
    return superstring;
}

std::string SuperstringSolver::Exact() {
    // Выбор последовательности склейки строк = выбор пути максимального веса в overlap графе
    if (data_.size() == 0) {
        return "";
    }
    if (data_.size() == 1) {
        return data_[0];
    }

    CompleteGraph overlap_graph(data_.size());
    overlap_graph.Fill([&](size_t i, size_t j) {
        return Overlap(data_[i], data_[j]);
    });
    auto path = overlap_graph.GetLongestPath();
    size_t len = path.size();
    std::string superstring = "";
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int overlap = overlap_graph.GetEdge(path[i], path[(i+1) % len]);
        size_t it = path[i];
        superstring += std::string_view(data_[it].c_str(), data_[it].size() - overlap);
    }
    superstring += data_[path.back()];
    return superstring;
}

int SuperstringSolver::Overlap(const std::string& lhs, const std::string& rhs) {
    if (lhs == rhs) {
        return 0;
    }
    int res = 0;
    for (size_t len = 1; len < lhs.size(); ++len) {
        std::string_view suff{lhs.c_str() + lhs.size() - len, len};
        std::string_view pref{rhs.c_str(), len};
        if (suff == pref) {
            res = len;
        }
    }
    return res;
}

void SuperstringSolver::Filter() {
    std::vector<std::string> new_data;
    for (size_t i = 0; i < data_.size(); ++i) {
        bool choose = true;
        for (size_t j = 0; j < data_.size(); ++j) {
            if (data_[j] == data_[i]) {
                if (i > j) {
                    choose = false;
                }
            } else if (data_[j].find(data_[i]) != std::string::npos) {
                choose = false;
            }
        }
        if (choose) {
            new_data.push_back(data_[i]);
        }
    }
    data_ = new_data;
}