#include "CrownPCH.h"
#include "Path_Grid.h"
#include <cmath>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

Path_Grid::Path_Grid(const glm::ivec2& gridsize, const float& noderadius, const bool& diagonal)
{
    gridWorldSize = gridsize;
    nodeRadius = noderadius;
    nodeDiameter = nodeRadius * 2;

    gridSizeX = static_cast<int>(round(gridWorldSize.x/nodeDiameter));
    gridSizeY = static_cast<int>(round(gridWorldSize.y / nodeDiameter));

    grid = new Path_Node * [static_cast<size_t>(gridWorldSize.y)];
    for (auto i = 0; i < static_cast<size_t>(gridWorldSize.y); ++i)
    {
        grid[i] = new Path_Node[static_cast<size_t>(gridWorldSize.x)];
    }

    AllowDiagonal = diagonal;
}

Path_Grid::Path_Grid()
{
    gridWorldSize = glm::ivec2(0, 0);
    gridSizeX = static_cast<int>(gridWorldSize.x);
    gridSizeY = static_cast<int>(gridWorldSize.y);
    nodeRadius = 0.f;
    nodeDiameter = 0.f;
    grid = new Path_Node * [static_cast<size_t>(gridWorldSize.y)];
    for (auto i = 0; i < static_cast<size_t>(gridWorldSize.y); ++i)
        grid[i] = new Path_Node[static_cast<size_t>(gridWorldSize.x)];

    AllowDiagonal = false;
}

void Path_Grid::CreateGrid() const
{
    float GridSpacing = 2.f;
    for(int x = 0; x < gridSizeX; x++)
    {
        for (int y = 0; y < gridSizeY; y++)
        {
        	//change this to use the actual location in the world when we have a level loader
            glm::vec3 worldpoint = glm::vec3(((x+ (GridSpacing*x)) * nodeDiameter + nodeRadius) / 100.0f, ((y+(GridSpacing * y)) * nodeDiameter + nodeRadius) / 100.0f, -15.f );
            bool walkable = true; //still needs implementing, but need a map for this
            int Cost = 100;

            //temporary  temp   setting things to unwalkable
            if (x == 0 || x == gridSizeX-1 || y == 0 || y == gridSizeY-1)
            {
                walkable = false;
            }
            if (x == gridSizeX - 4 && y >= 3 && y <= gridSizeY - 4)
            {
                walkable = false;
            }
            else if (y == gridSizeY - 4 && x <= gridSizeX - 4 && x >= 3)
            {
                walkable = false;
            }
            else if (y == 3 && x >= 6 && x <= gridSizeX - 4)
            {
                walkable = false;
            }


            grid[x][y] = Path_Node(walkable, worldpoint, Cost);
            grid[x][y].gridIndex = glm::ivec2(x, y);
        }
    }

}

Path_Node* Path_Grid::WorldToNode(const glm::vec3& a_WorldPosition) const
{
    //difference between positions is 0.96 with the above calculation, within opengl
	//Since my positions are in the center i'll use half of it which is 0.48
    for (int x = 0; x < gridSizeX; x++)
    {
        if (a_WorldPosition.x < (grid[x][0].worldPosition.x + 0.48f) && a_WorldPosition.x >(grid[x][0].worldPosition.x - 0.48f))
        {
            for (int y = 0; y < gridSizeY; y++)
            {
                if (a_WorldPosition.y < (grid[0][y].worldPosition.y + 0.48f) && a_WorldPosition.y >(grid[0][y].worldPosition.y - 0.48f))
                {
                    return &grid[x][y];
                }
            }
        }
    }
	
    return nullptr;
}

std::vector<Path_Node*> Path_Grid::FindNeighbours(Path_Node* endNode, Path_Node* node) const
{
    std::vector<Path_Node*> neighbours;
    for (int xx = -1; xx < 2; xx += 2)
    {
        if (node->gridIndex.x + xx >= 0 )
        {
            Path_Node* tempNode = &grid[node->gridIndex.x + xx][node->gridIndex.y];
            if ((node->CurrentCost + tempNode->NodeCost + Heuristic(endNode, tempNode)) < tempNode->CurrentCost)
            {
                tempNode->Diagonal = false;
                neighbours.push_back(&grid[node->gridIndex.x + xx][node->gridIndex.y]);
            }
        }
    }
    for (int yy = -1; yy < 2; yy += 2)
    {
        if (node->gridIndex.y + yy >= 0 )
        {
            Path_Node* tempNode = &grid[node->gridIndex.x][node->gridIndex.y + yy];
            if ((node->CurrentCost + tempNode->NodeCost + Heuristic(endNode, tempNode)) < tempNode->CurrentCost)
            {
                tempNode->Diagonal = false;
                neighbours.push_back(&grid[node->gridIndex.x][node->gridIndex.y + yy]);
            }
        }
    }
    if (AllowDiagonal)
    {
        for (int xx = -1; xx < 2; xx += 2)
        {
            for (int yy = -1; yy < 2; yy += 2)
            {
                if (node->gridIndex.x + xx >= 0 && node->gridIndex.y + yy >= 0)
                {
                    Path_Node* tempNode = &grid[node->gridIndex.x + xx][node->gridIndex.y + yy];
                    if ((node->CurrentCost + (tempNode->NodeCost + 40) + Heuristic(endNode, tempNode)) < tempNode->CurrentCost)
                    {
                        tempNode->Diagonal = true;
                        neighbours.push_back(&grid[node->gridIndex.x + xx][node->gridIndex.y + yy]);
                    }
                }
            }
        }
    }

    return neighbours;
}
