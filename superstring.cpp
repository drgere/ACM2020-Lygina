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
    // Воспользуемся алгоритмом КМП для строки rhs # lhs
    // Тогда по определению, префикс функция от всей строки -- это
    // длина наибольшоего суффикса строки lhs, которая совпадает с префиксом
    // строки rhs, то мы и ищем
    if (lhs == rhs) {
        return 0;
    }
    // 0 не может встретиться в строке, значит, он -- хороший разделитель
    std::string str = rhs + '\0' + lhs;
    std::vector<int> pi(str.size());
    for (size_t i = 1; i < str.size(); ++i) {
        size_t j = pi[i-1];
        while (j > 0 && str[i] != str[j]) {
            j = pi[j-1];
        }
        if (str[i] == str[j]) {
            j++;
        }
        pi[i] = j;
    }
    return pi.back();
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
