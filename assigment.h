#pragma once
#include <vector>

enum class SolverType {
    Greedy,
    Hungarian,
};

class AssigmentProblemSolver {
public:
    using EntryType = int;
    using Matrix = std::vector<std::vector<EntryType>>;
    using Assigment = std::vector<size_t>;

    AssigmentProblemSolver();

    AssigmentProblemSolver(Matrix matrix);

    // Set new matrix to problem
    void SetMatrix(Matrix matrix);

    Assigment Solve(SolverType type);

private:
    // Solve using greedy algorithm
    Assigment GreedySolve();

    // Solve using Hungarian algorithm
    Assigment HungarianSolve();

    Matrix matrix_;
};