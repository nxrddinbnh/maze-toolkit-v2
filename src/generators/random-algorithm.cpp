#include "../include/generators/random-algorithm.hpp"
#include "../include/generators/depth-first-search.hpp"

void RandomAlgorithm(std::vector<std::vector<int>>* maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::vector<void (*)(std::vector<std::vector<int>> *)> algorithms = 
    {
        DepthFirstSearch,
    };

    std::uniform_int_distribution<int> chooseAlgo(0, algorithms.size() - 1);
    int algoIndex = chooseAlgo(random);

    algorithms[algoIndex](maze);
}
