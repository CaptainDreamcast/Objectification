#pragma once

#include <tari/actorhandler.h>
#include <tari/geometry.h>

extern ActorBlueprint GirlBarHandler;

void setGirlBarHandlerSingleGirl(int tGirlfriendID);
void setGirlBarHandlerAllGirls();
void setGirlBarHandlerBasePosition(Position tPosition);
void updateBarSizeCall(int i);