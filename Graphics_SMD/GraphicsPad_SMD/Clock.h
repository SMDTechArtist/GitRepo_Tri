#pragma once
#ifndef ENGINE_CLOCK_H
#define ENGINE_CLOCK_H
#include <Windows.h>
#include <Clock.h>

namespace Timing
{
	class Clock
	{
		LARGE_INTEGER timeFrequency;
		LARGE_INTEGER timeLastFrame;
		LARGE_INTEGER deltaLastFrame;
		float deltaTime;

	public:
		bool initialize();
		bool shutdown();
		void newFrame();
		float timeElapsedLastFrame() const;

	};
}

#endif