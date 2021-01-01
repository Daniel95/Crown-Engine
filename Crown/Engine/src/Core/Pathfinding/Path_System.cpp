#include "CrownPCH.h"
#include "Path_System.h"

void Path_System::SetPathGrid(Path_Grid& path_grid)
{
    pathGrid = path_grid;
}

std::vector<Path_Node*> Path_System::FindPath(Path_Node* start, Path_Node* end)
{
    return PathAlgorithm(pathGrid, start, end);
}

std::vector<Path_Node*> Path_System::FindPath(const glm::vec3& StartPosition, const glm::vec3& EndPosition)
{
    Path_Node* startNode = pathGrid.WorldToNode(StartPosition);
    Path_Node* endNode = pathGrid.WorldToNode(EndPosition);
    return PathAlgorithm(pathGrid, startNode, endNode);
}

Path_System::~Path_System()
{
    Frontier.empty();
    Came_From.clear();
}

void Path_System::ResetNodes()
{
    for (int x = 0; x < pathGrid.gridSizeX; x++)
    {
        for (int y = 0; y < pathGrid.gridSizeY; y++)
        {
            if (pathGrid.grid[x][y].Walkable)
            {
                pathGrid.grid[x][y].CurrentCost = 10000;
                pathGrid.grid[x][y].Diagonal = false;
            }
        }
    }
    Frontier.empty();
    Came_From.clear();
}

std::vector<Path_Node*> Path_System::PathAlgorithm(Path_Grid& path_grid, Path_Node* start, Path_Node* end)
{
    EndFound = false;
    pathGrid = path_grid;
    StartNode = start;
    EndNode = end;
    StartNode->NodeCost = 0;
    StartNode->CurrentCost = 0;
    Frontier.put(StartNode, 0);

    std::vector<Path_Node*> ReturnedPath;

    //Checks all untill it checked all
    while (!Frontier.empty() && !EndFound)
    {
        CurrentNode = Frontier.get();
        AssignNeighbours(CurrentNode);
    }

    if (EndFound)
    {
        Path_Node* tempCurrentNode = Came_From.at(EndNode);
        ReturnedPath.push_back(EndNode);
        ReturnedPath.push_back(tempCurrentNode);

        while (tempCurrentNode != StartNode)
        {
            tempCurrentNode = Came_From.at(tempCurrentNode);
            ReturnedPath.push_back(tempCurrentNode);
        }
    }
    return ReturnedPath;
}

void Path_System::AssignNeighbours(Path_Node* node)
{
    std::vector<Path_Node*> Neighbours = pathGrid.FindNeighbours(EndNode ,node);

    for (int i = 0; i < Neighbours.size(); i++)
    {
        if (Neighbours[i]->gridIndex != StartNode->gridIndex)
        {
            if (Neighbours[i]->Walkable)
            {
                int total_cost = 0;
            	int new_cost = 0;
                if (Neighbours[i]->Diagonal == true)
                {
                    new_cost =  node->CurrentCost + (Neighbours[i]->NodeCost + 40);
                    total_cost = new_cost + (Heuristic(EndNode, Neighbours[i]));
                }
                else
                {
                    new_cost = node->CurrentCost + (Neighbours[i]->NodeCost);
                    total_cost = new_cost + (Heuristic(EndNode, Neighbours[i]));
                }

                if (new_cost < Neighbours[i]->CurrentCost)       //checks if the newcost is lower than the current of the node
                {    
                    Neighbours[i]->CurrentCost = new_cost;
                    Frontier.put(Neighbours[i], total_cost);
                    if (Came_From.count(Neighbours[i]) > 0)
                    {
                        Came_From.at(Neighbours[i]) = node;
                    }
                    else
                    {
                        Came_From.insert(std::make_pair(Neighbours[i], node));
                    }

                    if (Neighbours[i]->gridIndex == EndNode->gridIndex)
                    {
                        EndFound = true;
                        return;
                    }           
                }
            }
        }
    }
}
