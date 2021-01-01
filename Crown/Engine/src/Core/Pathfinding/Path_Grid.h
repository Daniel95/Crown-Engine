#pragma once

#include <vector>
#include "Path_Node.h"

class Path_Grid
{
public: 
    glm::ivec2 gridWorldSize;
    float nodeRadius;
    Path_Node** grid;
    int gridSizeX, gridSizeY;
    bool AllowDiagonal;
private:
    float nodeDiameter;
    inline int Heuristic(Path_Node* a, Path_Node* b) const
    {
        return static_cast<int>(abs(a->worldPosition.x - b->worldPosition.x) + abs(a->worldPosition.y - b->worldPosition.y));
    }

public: 
    Path_Grid(const glm::ivec2& gridsize, const float& noderadius, const bool& diagonal);
    Path_Grid();
    ~Path_Grid() = default;
    void CreateGrid() const;
    Path_Node* WorldToNode(const glm::vec3& a_WorldPosition) const;

    std::vector<Path_Node*> FindNeighbours(Path_Node* endNode, Path_Node* node) const;
};

