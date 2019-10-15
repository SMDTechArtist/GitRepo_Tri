#include <Clock.h>
#include <Vector3D.h>
namespace Timing
{

	bool Clock::initialize()
	{
		bool b = QueryPerformanceFrequency(&timeFrequency);
		if (!b)
			return false;
		return QueryPerformanceFrequency(&timeLastFrame);
		
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
	float Clock::timeElapsedLastFrame() const
	{
		return deltaTime;
	}
};