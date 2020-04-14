#include <vector>
#include <string>

class SuperstringSolver {
public:
    SuperstringSolver(std::vector<std::string> input);

    std::string Approximation4();

    std::string Exact();

    static int Overlap(const std::string& lhs, const std::string& rhs);

private:
    void Filter();

    std::vector<std::string> data_;
};