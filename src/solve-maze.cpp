#include "../include/solve-maze.hpp"
#include "../include/solvers/breadth-first.hpp"
#include "../include/solvers/random-solver.hpp"
#include "../include/solvers/bidirectional-bfs.hpp"

void SolveMaze(std::vector<std::vector<int>> *maze, int *algorithm)
{
    std::vector<void (*)(std::vector<std::vector<int>> *)> algorithms;

    algorithms.push_back(BreadthFirst);
    algorithms.push_back(BidirectionalBFS);
    algorithms.push_back(RandomSolver);

    if ((*algorithm - 1) < algorithms.size())
    {
        algorithms[*algorithm - 1](maze);
    }
    else
    {
        algorithms[algorithms.size() - 1](maze);
    }
}
