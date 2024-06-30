#include <iostream>
#include <vector>
#include "../include/generate-maze.hpp"
#include "../include/solve-maze.hpp"
#include "../include/tools.hpp"

int main()
{
    int algorithm;
    int width, height;
    char answer;

    // Choose generator algorithm
    std::cout << "Choose the algorithm to generate the maze:" << std::endl;
    std::cout << "(1) Depth-First Search" << std::endl;
    std::cout << "(2) Recursive Division" << std::endl;
    std::cout << "(3) Kruskal" << std::endl;
    std::cout << "(4) Wilson" << std::endl;
    std::cout << "(5) Random Selection" << std::endl;
    std::cin >> algorithm;

    // Set maze dimensions
    std::cout << std::endl << "Enter the size of the labyrinth (width, height): ";
    do
    {
        std::cin >> width >> height;
        if (!(width % 2) || !(height % 2))
        {
            std::cout << "Please enter odd values for width and height." << std::endl;
        }
    } while (!(width % 2) || !(height % 2));

    // Generate maze
    std::vector<std::vector<int>> maze(height, std::vector<int>(width, 0));
    std::cout << "Generating maze of size " << width << "x" << height << " using algorithm " << algorithm << "..." << std::endl;
    GenerateMaze(&maze, &algorithm);

    // Print maze
    std::cout << "Maze generated successfully!" << std::endl;
    PrintMaze(&maze);

    // Solve maze
    std::cout << "Would you like me to solve the maze? (y/n): ";
    std::cin >> answer;

    if (answer == 'y' || answer == 'Y')
    {
        std::cout << "Solving maze..." << std::endl << std::endl;

        // Choose solver algorithm
        std::cout << "Choose the algorithm to solve the maze:" << std::endl;
        std::cout << "(1) Breadth-First" << std::endl;
        std::cout << "(2) Bidirectional BFS" << std::endl;
        std::cout << "(3) A*" << std::endl;
        std::cout << "(4) Random Selection" << std::endl;
        std::cin >> algorithm;

        SolveMaze(&maze, &algorithm);
        PrintMaze(&maze);
    }

    return 0;
}
