#include "GroupingSpawnDecider.h"

#include <cstdio>

#include "RoomGeneration.h"
#include "RoomBody.h"

GroupingSpawnDecider::GroupingSpawnDecider()
{
}


GroupingSpawnDecider::~GroupingSpawnDecider()
{
}

Group GroupingSpawnDecider::WhichGroupSpawns() 
{
	//Integers for the spawn chance, so this integer % chance for success
	const int enemiesSpawnChance = 30;
	const int treasureSpawnChance = 30;
	const int puzzleSpawnChance = 30;
	const int emptySpawnChance = 20;
	const int roll = Roll(enemiesSpawnChance + treasureSpawnChance + puzzleSpawnChance + emptySpawnChance);
	
	if (roll <= enemiesSpawnChance)
	{
		//spawn
		if(DEBUGGING)
		printf("Spawning a group of enemies\n");
		return Group::Enemies;
	}
	if (roll <= enemiesSpawnChance + puzzleSpawnChance)
	{
		//Spawn
		if(DEBUGGING)
		printf("Spawning a puzzle\n");
		return Group::Puzzle;
	}
	if (roll <= enemiesSpawnChance + puzzleSpawnChance + treasureSpawnChance)
	{
		//Spawn
		if (DEBUGGING)
		printf("Spawning a treasure\n");
		return Group::Treasure;
	}
	if (DEBUGGING)
	printf("Empty room\n");
	//empty room matey?	
	//No room
	return Group::Empty;
}
