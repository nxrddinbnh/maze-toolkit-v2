#include "../include/generate-maze.hpp"
#include "../include/solve-maze.hpp"
#include "../include/tools.hpp"
#include <iostream>
#include <vector>

int main()
{
    int algorithm;
    int width, height;
    std::string answer;

    // Choose generator algorithm
    std::cout << "Choose the algorithm to generate the maze :" << std::endl;
    std::cout << "(1) Depth-First Search" << std::endl;
    std::cout << "(2) Kruskal" << std::endl;
    std::cout << "(3) Wilson" << std::endl;
    std::cout << "(4) Binary Tree" << std::endl;
    std::cout << "(5) Side Winder" << std::endl;
    std::cout << "(6) Recursive Division" << std::endl;
    std::cout << "(7) Random Selection" << std::endl;
    std::cin >> algorithm;

    // Set maze dimensions
    std::cout << std::endl;
    std::cout << "Enter the size of the maze (width, height) : ";
    std::cin >> width >> height;

    if (width % 2 == 0 || height % 2 == 0)
    {
        if (width % 2 == 0)
        {
            width++;
        }

        if (height % 2 == 0)
        {
            height++;
        }

        std::cout << "(!) Maze dimensions must be odd. Your maze is set to " << width << "x" << height << std::endl;
    }

    std::cout << std::endl;

    // Generate maze
    std::vector<std::vector<int>> maze(height, std::vector<int>(width, 0));
    std::cout << "Generating maze of size " << width << "x" << height << " using algorithm " << algorithm << "..." << std::endl;
    GenerateMaze(&maze, &algorithm);

    // Print maze
    std::cout << "Maze generated successfully!" << std::endl;
    PrintMaze(&maze);

    // Solve maze
    std::cout << "Would you like me to solve the maze? (Yes / No) : ";
    std::cin >> answer;

    if (answer == "y" || answer == "Y" || answer == "1" || answer == "Yes" || answer == "yes")
    {
        // Choose solver algorithm
        std::cout << std::endl;
        std::cout << "Choose the algorithm to solve the maze :" << std::endl;
        std::cout << "(1) Breadth-First" << std::endl;
        std::cout << "(2) Bidirectional BFS" << std::endl;
        std::cout << "(3) A*" << std::endl;
        std::cout << "(4) Random Selection" << std::endl;
        std::cin >> algorithm;

        std::cout << std::endl;
        std::cout << "Solving maze..." << std::endl;

        SolveMaze(&maze, &algorithm);
        PrintMaze(&maze);
    }
}
