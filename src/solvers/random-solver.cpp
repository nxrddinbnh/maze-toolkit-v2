#include "../../include/solvers/random-solver.hpp"
#include "../../include/solvers/breadth-first.hpp"

void RandomSolver(std::vector<std::vector<int>>* maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::vector<void (*)(std::vector<std::vector<int>> *)> solversAlgorithms = 
    {
        BreadthFirst,
    };

    std::uniform_int_distribution<int> chooseAlgo(0, solversAlgorithms.size() - 1);
    int algoIndex = chooseAlgo(random);

    solversAlgorithms[algoIndex](maze);
}
