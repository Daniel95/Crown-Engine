#pragma once
#include "Core/ECS/Job/Job.h"

struct CrowdComponent
{
	float rotateSpeed = 1;
};

class CrowdJob : public Crown::Job
{
public:
	virtual void OnUpdate(int a_ThreadId) override;
	
};
