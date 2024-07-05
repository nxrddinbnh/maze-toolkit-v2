#include "../include/generate-maze.hpp"
#include "../include/generators/depth-first-search.hpp"
#include "../include/generators/kruskal.hpp"
#include "../include/generators/wilson.hpp"
#include "../include/generators/recursive-division.hpp"
#include "../include/generators/binary-tree.hpp"
#include "../include/generators/side-winder.hpp"
#include "../include/generators/random-generator.hpp"

void GenerateMaze(std::vector<std::vector<int>> *maze, int *algorithm)
{
    std::vector<void (*)(std::vector<std::vector<int>> *)> algorithms;

    algorithms.push_back(DepthFirstSearch);
    algorithms.push_back(Kruskal);
    algorithms.push_back(Wilson);
    algorithms.push_back(RecursiveDivision);
    algorithms.push_back(BinaryTree);
    algorithms.push_back(SideWinder);
    algorithms.push_back(RandomGenerator);

    if ((*algorithm - 1) < algorithms.size())
    {
        algorithms[*algorithm - 1](maze);
    }
    else
    {
        algorithms[algorithms.size() - 1](maze);
    }
}
