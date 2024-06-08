#include "../../include/generators/random-generator.hpp"
#include "../../include/generators/depth-first-search.hpp"

void RandomGenerator(std::vector<std::vector<int>>* maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::vector<void (*)(std::vector<std::vector<int>> *)> generatorsAlgorithms = 
    {
        DepthFirstSearch,
    };

    std::uniform_int_distribution<int> chooseAlgo(0, generatorsAlgorithms.size() - 1);
    int algoIndex = chooseAlgo(random);

    generatorsAlgorithms[algoIndex](maze);
}
