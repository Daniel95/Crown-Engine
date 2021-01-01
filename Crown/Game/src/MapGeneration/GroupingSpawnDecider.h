#pragma once
enum class Group
{
	Enemies = 1,
	Treasure = 2,
	Puzzle = 3,
	Empty = 4,
	start = 10
};

class GroupingSpawnDecider
{
public:
	GroupingSpawnDecider();
	~GroupingSpawnDecider();
	static Group WhichGroupSpawns();
	
};
