#include <gtest/gtest.h>
#include "assigment.h"
#include "graph.h"
#include "superstring.h"

void CheckCorrectness(std::vector<std::string> data, std::string superstring) {
    for (auto& str : data) {
        EXPECT_NE(superstring.find(str), std::string::npos);
    }
}

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

TEST(GraphTest, CycleCoverTest) {
    std::vector<std::string> data = {
        "cde", "abc", "eab", "fgh", "ghf", "hed" // Sample from the article
    };
    CompleteGraph graph(data.size());
    graph.Fill([&data](size_t i, size_t j) {
        return SuperstringSolver::Overlap(data[i], data[j]);
    });
    auto cover = graph.GetMaximumCycleCover();
    EXPECT_EQ(cover.size(), 3);
    EXPECT_EQ(cover[0].size(), 3);
    EXPECT_EQ(cover[1].size(), 2);
    EXPECT_EQ(cover[2].size(), 1);
}

TEST(GraphTest, LongestPathFromTest) {
    CompleteGraph graph(4);
    graph.GetEdge(0, 1) = 2;
    graph.GetEdge(1, 2) = 2;
    graph.GetEdge(2, 3) = 2;
    std::vector<size_t> path;
    EXPECT_EQ(graph.GetLongestPathFrom(0, path), 6);
    EXPECT_EQ(path.size(), 4);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(path[i], i);
    }
    EXPECT_EQ(graph.GetLongestPathFrom(1, path), 4);
}

TEST(GraphTest, LongestPath) {
    CompleteGraph graph(4);
    graph.GetEdge(0, 1) = 2;
    graph.GetEdge(1, 2) = 2;
    graph.GetEdge(2, 3) = 2;
    std::vector<size_t> path = graph.GetLongestPath();
    EXPECT_EQ(path.size(), 4);
    for (size_t i = 0; i < 4; ++i) {
        EXPECT_EQ(path[i], i);
    }
}

TEST(SuperstringTest, OverlapTest) {
    std::string a = "abb";
    std::string b = "bbc";
    EXPECT_EQ(SuperstringSolver::Overlap(a,b), 2);
    EXPECT_EQ(SuperstringSolver::Overlap(a,a), 0);
    EXPECT_EQ(SuperstringSolver::Overlap(b,a), 0);
}

TEST(SuperstringTest, Approximation4CorrectnessTest) {
    std::vector<std::string> data = {
        "cde", "abc", "eab", "fgh", "ghf", "hed" // Sample from the article
    };
    SuperstringSolver solver(data);
    std::string result = solver.Approximation4();
    CheckCorrectness(data, result);
}

TEST(SuperstringTest, ExactCorrectnessTest) {
    std::vector<std::string> data = {
        "cde", "abc", "eab", "fgh", "ghf", "hed" // Sample from the article
    };
    SuperstringSolver solver(data);
    std::string result = solver.Exact();
    CheckCorrectness(data, result);
}

TEST(SuperstringTest, EmptyTest) {
    SuperstringSolver solver({});
    std::string result = solver.Exact();
    EXPECT_EQ(result.size(), 0);
    result = solver.Approximation4();
    EXPECT_EQ(result.size(), 0);
}

TEST(SuperstringTest, IntersectionFilterTest) {
    // Также тестирует крайний случай 1 строки
    std::vector<std::string> data = {
        "aaa", "aa", "a"
    };
    SuperstringSolver solver(data);
    std::string resultExact = solver.Exact();
    std::string resultApprox = solver.Approximation4();

    CheckCorrectness(data, resultExact);
    CheckCorrectness(data, resultApprox);

    EXPECT_EQ(resultExact.size(), 3);
    EXPECT_EQ(resultApprox.size(), 3);
}

TEST(SuperstringTest, ExtraLagreExactTest) {
    std::vector<std::string>  data(100);
    for (int i = 0; i < 100; ++i) {
        data[i] = std::to_string(i);
    }
    SuperstringSolver solver(data);
    std::string resultExact = solver.Exact();
    std::string resultApprox = solver.Approximation4();

    CheckCorrectness(data, resultExact);
    CheckCorrectness(data, resultApprox);
    // Показывает, что для точного решения запустился плохой алгоритм
    EXPECT_LE(resultApprox.size(), resultExact.size());
}
