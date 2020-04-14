#include "superstring.h"
#include <iostream>

int main() {
    size_t n;
    std::cin >> n;
    std::vector<std::string> data(n);
    for (size_t i = 0; i < n; ++i) {
        std::cin >> data[i];
    }
    SuperstringSolver solver(data);
    std::cout << "Exact superstring: " << solver.Exact() << std::endl;
    std::cout << "Approximation:     " << solver.Approximation4() << std::endl; 
    return 0;
}