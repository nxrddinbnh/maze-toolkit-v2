#include "../include/tools.hpp"

std::vector<std::pair<int, int>> cellOrder;

void AddCellToOrder(int y, int x)
{
    cellOrder.push_back(std::make_pair(y, x));
}

int GetCellOrderSize()
{
    return cellOrder.size();
}

std::pair<int, int> GetCellOrder(int index)
{
    return cellOrder[index];
}

void ClearCellOrder()
{
    cellOrder.clear();
}

bool InLimits(std::vector<std::vector<int>> *maze, std::pair<int, int> cell)
{
    return cell.first > 0 &&
           cell.first < (*maze).size() &&
           cell.second > 0 &&
           cell.second < (*maze)[0].size();
}

std::pair<int, int> RandomCell(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::uniform_int_distribution<int> distributionX(1, (*maze)[0].size() - 2);
    std::uniform_int_distribution<int> distributionY(1, (*maze).size() - 2);

    std::pair<int, int> start;

    do
    {
        start = {distributionY(random), distributionX(random)};
    } while (start.first % 2 == 0 || start.second % 2 == 0);

    return start;
}

void SetEntryExit(std::vector<std::vector<int>> *maze)
{
    std::random_device seed;
    std::mt19937 random(seed());

    std::uniform_int_distribution<int> randomEntryExit(1, (*maze).size() - 2);
    int entry, exit;

    do
    {
        entry = randomEntryExit(random);
    } while (entry % 2 == 0 && (*maze)[entry][1] == CellType::WALL);

    do
    {
        exit = randomEntryExit(random);
    } while (exit % 2 == 0 && (*maze)[exit][(*maze)[0].size() - 2] == CellType::WALL);

    (*maze)[entry][1] = CellType::ENTRY;
    (*maze)[exit][(*maze)[0].size() - 2] = CellType::EXIT;
}

std::pair<std::pair<int, int>, std::pair<int, int>> FindEntryExit(std::vector<std::vector<int>> *maze)
{
    std::pair<int, int> entry = {-1, -1};
    std::pair<int, int> exit = {-1, -1};

    for (int y = 1; y < (*maze).size() - 1; ++y)
    {
        for (int x = 1; x < (*maze)[0].size() - 1; ++x)
        {
            if ((*maze)[y][x] == CellType::ENTRY)
            {
                entry = {y, x};
            }
            else if ((*maze)[y][x] == CellType::EXIT)
            {
                exit = {y, x};
            }
        }
    }

    return {entry, exit};
}

void Enclose(std::vector<std::vector<int>> *maze)
{
    for (int y = 0; y < (*maze).size(); y++)
    {
        for (int x = 0; x < (*maze)[0].size(); x++)
        {
            bool isBorder = x == 0 || x == (*maze)[0].size() - 1 ||
                            y == 0 || y == (*maze).size() - 1;
            (*maze)[y][x] = isBorder ? CellType::WALL : CellType::EMPTY;
        }
    }
}

void Grid(std::vector<std::vector<int>> *maze)
{
    for (int y = 0; y < (*maze).size(); y++)
    {
        for (int x = 0; x < (*maze)[0].size(); x++)
        {
            (*maze)[y][x] = (y % 2 == 0 || x % 2 == 0) ? CellType::WALL : CellType::EMPTY;
        }
    }
}

void Fill(std::vector<std::vector<int>> *maze)
{
    for (int y = 0; y < (*maze).size(); y++)
    {
        for (int x = 0; x < (*maze)[0].size(); x++)
        {
            (*maze)[y][x] = CellType::WALL;
        }
    }
}

void PrintMaze(std::vector<std::vector<int>> *maze)
{
    std::cout << std::endl;

    for (int y = 0; y < (*maze).size(); y++)
    {
        for (int x = 0; x < (*maze)[0].size(); x++)
        {
            if ((*maze)[y][x] == CellType::WALL)
            {
                std::cout << "# ";
            }
            else if ((*maze)[y][x] == CellType::PATH)
            {
                std::cout << ". ";
            }
            else
            {
                std::cout << "  ";
            }
        }

        std::cout << std::endl;
    }

    std::cout << std::endl;
}
