#pragma once

#include "Core/Application.h"
#define VisualLeakDetector
#ifdef VisualLeakDetector
#ifndef __ORBIS__
#include "../ext/Visual Leak Detector/include/vld.h"
#include "../ext/Visual Leak Detector/include/vld_def.h"
#endif
#endif

extern Crown::Application* Crown::CreateApplication();

int main()
{
	Crown::Application* application = Crown::CreateApplication();
	application->Run();
	delete application;
	return 1;
}