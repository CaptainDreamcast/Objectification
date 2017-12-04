#pragma once

#include <tari/actorhandler.h>

extern ActorBlueprint ClockHandler;

void resetClock();
int increaseClockAndReturnIfNewDay();