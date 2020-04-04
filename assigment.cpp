#include "assigment.h"

#include <stdexcept>

AssigmentProblemSolver::AssigmentProblemSolver() {
}

AssigmentProblemSolver::AssigmentProblemSolver(Matrix matrix) : matrix_(std::move(matrix)) {
}

void AssigmentProblemSolver::SetMatrix(Matrix matrix) {
    matrix_ = std::move(matrix);
}

AssigmentProblemSolver::Assigment AssigmentProblemSolver::Solve(SolverType type) {
    if (type == SolverType::Greedy) {
        return GreedySolve();
    } else if (type == SolverType::Hungarian) {
        return HungarianSolve();
    }
    throw std::runtime_error("Unknown type of solver");
}

AssigmentProblemSolver::Assigment AssigmentProblemSolver::GreedySolve() {
    Assigment result(matrix_.size(), matrix_.size()); // Filled with invalid values
    Matrix used(matrix_.size(), std::vector<EntryType>(matrix_.size(), 0));
    for (size_t q = 0; q < matrix_.size(); ++q) {
        size_t max_i = 0;
        size_t max_j = 0;
        EntryType max_value = -1; // All elements of matrix should be positive => -1 less than each element
        
        for (size_t i = 0; i < matrix_.size(); ++i) {
            for (size_t j = 0; j < matrix_.size(); ++j) {
                if (used[i][j]) {
                    continue;
                }
                if (matrix_[i][j] > max_value) {
                    max_value = matrix_[i][j];
                    max_i = i;
                    max_j = j;
                }
            }
        }

        if (max_value == -1) {
            throw std::runtime_error("Max value did not set");
        }

        result[max_i] = max_j;
        for (size_t i = 0; i < matrix_.size(); ++i) {
            used[max_i][i] = 1;
            used[i][max_j] = 1;
        }
    }
    return result;
}

AssigmentProblemSolver::Assigment AssigmentProblemSolver::HungarianSolve() {
    // TODO    
}