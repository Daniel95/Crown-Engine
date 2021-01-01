#pragma once

#include "Core/Utilities/Log.h"

#define GLM_FORCE_XYZW_ONLY

#ifdef CROWN_PS4
#endif

#ifdef CROWN_PC
#include "Optick/optick.h"
#endif

#ifdef CROWN_DEBUG
#define CROWN_ENABLE_ASSERTS
#endif

#ifdef CROWN_ENABLE_ASSERTS
#define ASSERT(x, ...) { if(!(x)) { LOG_ERROR("Assertion Failed: " << __VA_ARGS__); __debugbreak(); } }
#define ASSERT_ENGINE(x, ...) { if(!(x)) { LOG_ENGINE_ERROR("Assertion Failed: " << __VA_ARGS__); __debugbreak(); } }
#else
#define ASSERT(x, ...)
#define ASSERT_ENGINE(x, ...)
#endif

#if defined(CROWN_PC) //&& defined(CROWN_DEBUG)
#define CROWN_PROFILE_FRAME(FRAME_NAME) OPTICK_FRAME(FRAME_NAME)
#define CROWN_PROFILE_FUNCTION() OPTICK_EVENT()
#else
#define CROWN_PROFILE_FRAME(FRAME_NAME, ...)
#define CROWN_PROFILE_FUNCTION()
#endif

#define SUB_FN(fn, ...) this, std::bind(&fn, this, __VA_ARGS__)

typedef unsigned long long ullong;
typedef unsigned int uint;

#ifdef __ORBIS__
#define UNUSED(x)x __attribute__((unused))
#else
#define UNUSED(x)x 
#endif
