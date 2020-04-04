#include <gtest/gtest.h>
#include "assigment.h"
#include "graph.h"

TEST(AssigmentTest, SimpleGreedyTest) {
    std::vector<std::vector<int>> matrix = {
        {0, 1, 1},
        {1, 0, 3},
        {2, 1, 0}
    };

    AssigmentProblemSolver solver(matrix);
    std::vector<size_t> result = solver.Solve(SolverType::Greedy);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 2);
    EXPECT_EQ(result[2], 0);
}

TEST(GraphTest, SimpleFillTest) {
    auto filler = [](size_t i, size_t j) {
        return i*i + j*j;
    };
    CompleteGraph graph(3);
    graph.Fill(filler);
    EXPECT_EQ(graph.GetEdge(0,0), 0);
    EXPECT_EQ(graph.GetEdge(0,1), 1);
    EXPECT_EQ(graph.GetEdge(1,0), 1);
    EXPECT_EQ(graph.GetEdge(1,1), 2);
    EXPECT_EQ(graph.GetEdge(2,2), 8);
}
