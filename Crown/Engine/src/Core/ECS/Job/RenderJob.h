#pragma once

#include "Core/ECS/Job/Job.h"

namespace Crown
{
	class RenderJob : public Job
	{
	public:
		virtual void OnUpdate(int a_ThreadId) override;
	};
}