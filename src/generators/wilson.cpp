#include "../../include/generators/wilson.hpp"
#include "../../include/tools.hpp"

void Wilson(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::vector<std::vector<bool>> isIncluded((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
    std::vector<std::pair<int, int>> directions = {{2, 0}, {0, 2}, {-2, 0}, {0, -2}};
    std::vector<std::pair<int, int>> cellList;

    Fill(maze);

    // Collect all the mandatory cells to visit
    for (int y = 1; y < (*maze).size() - 1; y += 2)
    {
        for (int x = 1; x < (*maze)[0].size() - 1; x += 2)
        {
            cellList.push_back({y, x});
        }
    }

    std::pair<int, int> start = RandomStart(maze);
    (*maze)[start.first][start.second] = 0;
    isIncluded[start.first][start.second] = true;

    cellList.erase(std::remove(cellList.begin(), cellList.end(), start), cellList.end());

    while (!cellList.empty())
    {
        std::pair<int, int> currentCell = cellList[random() % cellList.size()];
        std::vector<std::pair<int, int>> path;
        path.push_back(currentCell);

        // Generate a random path
        while (!isIncluded[path.back().first][path.back().second])
        {
            std::shuffle(directions.begin(), directions.end(), random);

            for (auto &dir : directions)
            {
                int nextY = path.back().first + dir.first;
                int nextX = path.back().second + dir.second;
                std::pair<int, int> nextCell = {nextY, nextX};

                if (nextY > 0 && nextY < (*maze).size() - 1 && nextX > 0 && nextX < (*maze)[0].size() - 1)
                {
                    path.push_back(nextCell);
                    break;
                }
            }
        }

        // Find a valid path avoiding loops
        std::queue<std::pair<int, int>> queue;
        std::vector<std::vector<bool>> hasChanged((*maze).size(), std::vector<bool>((*maze)[0].size(), false));
        std::pair<int, int> lastCell = path[0];

        queue.push(path.back());
        hasChanged[path.back().first][path.back().second] = true;

        while (!queue.empty())
        {
            std::pair<int, int> cell = queue.front();
            queue.pop();

            int currentValue = (*maze)[cell.first][cell.second];

            for (auto &dir : directions)
            {
                int nextY = cell.first + dir.first;
                int nextX = cell.second + dir.second;
                std::pair<int, int> nextCell = {nextY, nextX};

                if (std::find(path.begin(), path.end(), nextCell) != path.end() && !hasChanged[nextY][nextX])
                {
                    (*maze)[nextY][nextX] = currentValue - 1;
                    hasChanged[nextY][nextX] = true;
                    queue.push(nextCell);

                    if (nextCell == lastCell)
                    {
                        queue = std::queue<std::pair<int, int>>();
                        break;
                    }
                }
            }
        }

        // Trace and mark the path as part of the maze
        std::pair<int, int> cell = path[0];
        int currentValue = (*maze)[cell.first][cell.second];

        (*maze)[cell.first][cell.second] = 0;
        isIncluded[cell.first][cell.second] = true;

        cellList.erase(std::remove(cellList.begin(), cellList.end(), cell), cellList.end());

        while (currentValue < 0)
        {
            for (auto &dir : directions)
            {
                int nextY = cell.first + dir.first;
                int nextX = cell.second + dir.second;
                std::pair<int, int> nextCell = {nextY, nextX};

                if (std::find(path.begin(), path.end(), nextCell) != path.end() && (*maze)[nextY][nextX] == currentValue + 1)
                {
                    (*maze)[(cell.first + nextY) / 2][(cell.second + nextX) / 2] = 0;
                    isIncluded[(cell.first + nextY) / 2][(cell.second + nextX) / 2] = true;
                    (*maze)[nextY][nextX] = 0;
                    isIncluded[nextY][nextX] = true;

                    cellList.erase(std::remove(cellList.begin(), cellList.end(), nextCell), cellList.end());

                    cell = nextCell;
                    currentValue += 1;
                    break;
                }
            }
        }
    }

    SetEntryExit(maze);
}