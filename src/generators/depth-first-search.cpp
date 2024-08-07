#include "../../include/generators/depth-first-search.hpp"
#include "../../include/tools.hpp"

void DepthFirstSearch(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::stack<std::pair<int, int>> stack;
    std::vector<std::vector<bool>> isVisited((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::pair<int, int>> directions = {{-2, 0}, {0, 2}, {2, 0}, {0, -2}};
    std::pair<int, int> start = RandomCell(maze);

    ClearCellOrder();
    Fill(maze);

    (*maze)[start.first][start.second] = 0;
    isVisited[start.first][start.second] = true;
    stack.push(start);
    AddCellToOrder(start.first, start.second);

    while (!stack.empty())
    {
        std::pair<int, int> current = stack.top();
        bool isFound = false;

        std::shuffle(directions.begin(), directions.end(), random);

        for (auto &dir : directions)
        {
            int nextY = current.first + dir.first;
            int nextX = current.second + dir.second;

            if (InLimits(maze, {nextY, nextX}) && !isVisited[nextY][nextX])
            {
                (*maze)[(current.first + nextY) / 2][(current.second + nextX) / 2] = 0;
                isVisited[(current.first + nextY) / 2][(current.second + nextX) / 2] = true;
                AddCellToOrder((current.first + nextY) / 2, (current.second + nextX) / 2);

                (*maze)[nextY][nextX] = 0;
                isVisited[nextY][nextX] = true;
                AddCellToOrder(nextY, nextX);

                stack.push(std::make_pair(nextY, nextX));
                isFound = true;

                break;
            }
        }

        if (!isFound)
        {
            stack.pop();
        }
    }
}
