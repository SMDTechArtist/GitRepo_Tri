#include "Clock.h"

namespace Timing
{
	bool Clock::initialize()
	{
		bool b = QueryPerformanceFrequency(&timeFrequency) != 0;
		if (!b)
			return false;
		return QueryPerformanceCounter(&timeLastFrame) != 0;
	}

	bool Clock::shutdown() 
	{ 
		return false; 
	}



	void Clock::newFrame()
	{
		LARGE_INTEGER thisTime;
		QueryPerformanceCounter(&thisTime);
		LARGE_INTEGER delta;
		delta.QuadPart = thisTime.QuadPart - timeLastFrame.QuadPart;
		deltaTime = ((float)delta.QuadPart) / timeFrequency.QuadPart;
		timeLastFrame.QuadPart = thisTime.QuadPart;
	}

	//void Clock::stop()
	//{
	//	LARGE_INTEGER thisTime;
	//	QueryPerformanceCounter(&thisTime);
	//	LARGE_INTEGER delta;
	//	delta.QuadPart = thisTime.QuadPart - lastStartTime.QuadPart;
	//	deltaTime = ((float)delta.QuadPart) / timeFrequency.QuadPart;
	//	deltaLastLap.QuadPart = thisTime.QuadPart;
	//}
	//
	//
	//void Clock::lap()
	//{
	//	stop();
	//	start();
	//}

	float Clock::timeElapsedLastFrame() const
	{
		return deltaTime;
	}
}