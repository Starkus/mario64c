#pragma once

#include <GLFW/glfw3.h>

class Time {
	static double startTime;
	static double tickStartTime;
	static double lastTickDuration;

public:
	static void begin();
	static void beginTick();
	static void endTick();
	static double time();
	static double deltaTime();
};