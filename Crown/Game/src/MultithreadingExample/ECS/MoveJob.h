#pragma once
#include "Core/ECS/Job/Job.h"

struct MoveComponent
{
	float speed = 1;
};

class MoveJob : public Crown::Job
{
public:
	virtual void OnUpdate(int a_ThreadId) override;
};
