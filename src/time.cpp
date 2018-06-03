#include "time.hpp"

double Time::startTime = 0;
double Time::tickStartTime = 0;
double Time::lastTickDuration = 0;

void Time::begin() {
	Time::startTime = glfwGetTime();
	Time::tickStartTime = startTime;
}

void Time::beginTick() {
	double now = glfwGetTime();
	Time::lastTickDuration = now - Time::tickStartTime;
	Time::tickStartTime = now;
}

void Time::endTick() {
	Time::lastTickDuration = glfwGetTime() - Time::tickStartTime;
}

double Time::time() {
	return Time::tickStartTime - Time::startTime;
}

double Time::deltaTime() {
	return Time::lastTickDuration;
}