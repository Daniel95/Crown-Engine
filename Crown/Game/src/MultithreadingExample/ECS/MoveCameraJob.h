#pragma once
#include "Core/ECS/Job/Job.h"

struct MoveCameraComponent
{
	float speed = 1;
};

class MoveCameraJob : public Crown::Job
{
public:
	virtual void OnUpdate(int a_ThreadId) override;
};
